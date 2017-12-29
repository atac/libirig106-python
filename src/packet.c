
#include <Python.h>
#include "structmember.h"

#include "libirig106/src/irig106ch10.h"
#include "libirig106/src/i106_decode_1553f1.h"

#include "c10.h"
#include "packet.h"


static void Packet_dealloc(Packet *self){
    free(self->body);
    Py_DECREF(self->parent);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *Packet_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    Packet *self = (Packet *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int Packet_init(Packet *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->parent = (C10 *)tmp;
    Py_INCREF(self->parent);

    // Note file offset
    if ((status = I106C10GetPos(self->parent->handle, &self->offset))){
        PyErr_Format(PyExc_RuntimeError, "I106C10GetPos: %s", I106ErrorString(status));
        return -1;
    }

    // Read packet header
    I106C10Header header;
    if ((status = I106C10ReadNextHeader(self->parent->handle, &header))){
        if (status == I106_EOF)
            // TODO: StopIteration should be called in C10.next
            PyErr_Format(PyExc_StopIteration, "EOF");
        else
            PyErr_Format(PyExc_RuntimeError, "I106C10ReadNextHeader: %s", I106ErrorString(status));
        return -1;
    }

    // Copy header values to our Packet instance
    memcpy(&self->SyncPattern, &header, sizeof(I106C10Header));

    // Load the payload into a buffer
    self->body = malloc(GetDataLength(&header));
    if ((status = I106C10ReadData(self->parent->handle, GetDataLength(&header), self->body))){
        if (status == I106_EOF)
            PyErr_Format(PyExc_EOFError, "End of file");
        else
            PyErr_Format(PyExc_RuntimeError, "I106C10ReadData: %s", I106ErrorString(status));
        return -1;
    }

    // Read Channel Specific Data Word (CSDW) and first message
    switch (self->DataType){
        case I106CH10_DTYPE_1553_FMT_1:
            if ((status = I106_Decode_First1553F1(&header, self->body, &self->MS1553_MSG))){
                PyErr_Format(PyExc_RuntimeError, "I106DecodeFirst1553: %s", I106ErrorString(status));
                return -1;
            }
    }

    return 0;
}


static PyObject *Packet_get_rtc(Packet *self, void *closure){
    int64_t rtc = 0L;
    memcpy(&rtc, &((Packet *)self)->RTC[0], 6);
    return Py_BuildValue("l", rtc);
}


static PyObject *Packet_len(Packet *self){
    long len = 0;

    switch (self->DataType){
        case I106CH10_DTYPE_1553_FMT_1:
            len = self->MS1553_MSG.CSDW->MessageCount;
            break;
        default:
            len = 0;
    }

    return Py_BuildValue("l", len);
}


// Iterate over packet messages (depending on data type)
static PyObject *Packet_next(Packet *self){
    I106Status status;
    MS1553F1_Message *msg1553 = malloc(sizeof(MS1553F1_Message));

    switch (self->DataType){
        case I106CH10_DTYPE_1553_FMT_1:
            memcpy(msg1553, &self->MS1553_MSG, sizeof(MS1553F1_Message));
            if ((status = I106_Decode_Next1553F1(&self->MS1553_MSG))){
                if (status == I106_NO_MORE_DATA)
                    PyErr_Format(PyExc_StopIteration, "No more data");
                else
                    PyErr_Format(PyExc_RuntimeError, "Decode_Next1553F1: %s", I106ErrorString(status));
                break;
            }
            return (PyObject *)msg1553;
    }

    printf("Iterate...\n");

    return NULL;
}


static PyObject *Packet_bytes(Packet *self){
    int head_size = GetHeaderLength((I106C10Header *)&self->SyncPattern);
    void *buffer = malloc(self->PacketLength);
    memcpy(buffer, &self->SyncPattern, head_size);

    // Track our last position and state so we can restore the file cursor if needed.
    off_t offset;
    I106FileState state = handles[self->parent->handle].File_State;
    I106Status status;
    if ((status = I106C10GetPos(self->parent->handle, &offset))){
        PyErr_Format(PyExc_RuntimeError, "I106C10GetPos: %s", I106ErrorString(status));
        return NULL;
    }

    if (offset != (self->offset + head_size)){
        if ((status = I106C10SetPos(self->parent->handle, self->offset + head_size))){
            PyErr_Format(PyExc_RuntimeError, "I106C10SetPos: %s", I106ErrorString(status));
            return NULL;
        }

        handles[self->parent->handle].File_State = I106_READ_DATA;
    }

    if ((status = I106C10ReadData(self->parent->handle, self->PacketLength - head_size, buffer + head_size))){
        PyErr_Format(PyExc_RuntimeError, "I106C10ReadData: %s", I106ErrorString(status));
        return NULL;
    }

    // Restore prior state if applicable.
    if (offset != (self->offset + head_size)){
        if ((status = I106C10SetPos(self->parent->handle, offset))){
            PyErr_Format(PyExc_RuntimeError, "I106C10SetPos: %s", I106ErrorString(status));
            return NULL;
        }
        handles[self->parent->handle].File_State = state;
    }

    PyObject *result = Py_BuildValue("y#", (char *)buffer, self->PacketLength);
    free(buffer);

    return result;
}


static PyMemberDef Packet_members[] = {
    {"parent", T_OBJECT_EX, offsetof(Packet, parent), 0, "Parent C10 object"},
    {"offset", T_LONG, offsetof(Packet, offset), 0, "Absolute byte offset within file"},

    {"sync_pattern", T_USHORT, offsetof(Packet, SyncPattern), 0, "Sync pattern"},
    {"channel_id", T_USHORT, offsetof(Packet, ChannelID), 0, "Channel ID"},
    {"packet_length", T_UINT, offsetof(Packet, PacketLength), 0, "Packet Length (bytes)"},
    {"data_length", T_UINT, offsetof(Packet, DataLength), 0, "Data Length (bytes)"},
    {"header_version", T_UBYTE, offsetof(Packet, HeaderVersion), 0, "Header version"},
    {"sequence_number", T_UBYTE, offsetof(Packet, SequenceNumber), 0, "Sequence number"},
    {"packet_flags", T_UBYTE, offsetof(Packet, PacketFlags), 0, "Packet flags"},
    {"data_type", T_UBYTE, offsetof(Packet, DataType), 0, "Data type"},
    {"checksum", T_USHORT, offsetof(Packet, Checksum), 0, "Header checksum"},

    // @TODO: implement time from secondary header
    {"secondary_checksum", T_USHORT, offsetof(Packet, SecondaryChecksum), 0, "Secondary header checksum"},

    {NULL}
};


static PyMethodDef Packet_methods[] = {
    {"__bytes__", (PyCFunction)Packet_bytes, METH_NOARGS, "Return the packet as raw bytes"},
    {NULL}  /* Sentinel */
};


static PyGetSetDef Packet_getset[] = {
    {"rtc", (getter)Packet_get_rtc, NULL, "10Mhz RTC clock"},
    {NULL},
};


static PyTypeObject Packet_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.Packet",             /* tp_name */
    sizeof(Packet), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Packet_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Chapter 10 packet object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    PyObject_SelfIter,                         /* tp_iter */
    (iternextfunc)Packet_next,                         /* tp_iternext */
    Packet_methods,             /* tp_methods */
    Packet_members,             /* tp_members */
    Packet_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Packet_init,      /* tp_init */
    0,                         /* tp_alloc */
    Packet_new,                 /* tp_new */
};


PyObject *New_Packet(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &Packet_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_packet_class(PyObject *module){
	if (PyType_Ready(&Packet_Type) < 0)
        return;

    Py_INCREF(&Packet_Type);
    PyModule_AddObject(module, "Packet", (PyObject *)&Packet_Type);
}

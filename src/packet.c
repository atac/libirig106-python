
#include <Python.h>
#include "structmember.h"

#include "libirig106/src/irig106ch10.h"

#include "c10.h"
#include "packet.h"


static void Packet_dealloc(Packet *self){
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *Packet_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    Packet *self = (Packet *)type->tp_alloc(type, 0);
    /* if (self != NULL) */
    /*     self->parent = NULL; */

    return (PyObject *)self;
}


static int Packet_init(Packet *self, PyObject *args, PyObject *kwargs){
    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->parent = (C10 *)tmp;

    I106C10Header header;
    I106Status status = I106C10ReadNextHeader(self->parent->handle, &header);
    if (status){
        PyErr_Format(PyExc_RuntimeError, "I106C10ReadNextHeader: %s", I106ErrorString(status));
        return -1;
    }

    // Copy header values to our Packet instance
    memcpy(&self->SyncPattern, &header, sizeof(I106C10Header));

    return 0;
}


static PyObject *Packet_get_rtc(Packet *self, void *closure){
    int64_t rtc = 0L;
    memcpy(&rtc, &((Packet *)self)->RTC[0], 6);
    return Py_BuildValue("l", rtc);
}


static PyObject *Packet_next(Packet *self){
    // Iterate over packet messages (depending on data type)
    return NULL;
}


static PyMemberDef Packet_members[] = {
    {"parent", T_OBJECT_EX, offsetof(Packet, parent), 0, "Parent C10 object"},
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
    /* {"test", (PyCFunction)Packet_test, METH_NOARGS | METH_CLASS, "Test the Packet class."}, */
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
    return p;
}


void add_packet_class(PyObject *module){
	if (PyType_Ready(&Packet_Type) < 0)
        return;

    Py_INCREF(&Packet_Type);
    PyModule_AddObject(module, "Packet", (PyObject *)&Packet_Type);
}

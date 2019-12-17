
#include <Python.h>
#include "structmember.h"

#include "libirig106/src/irig106ch10.h"
#include "libirig106/src/i106_decode_ethernet.h"

#include "c10.h"
#include "ethernet.h"


static void EthernetMsg_dealloc(EthernetMsg *self){
    Py_DECREF(self->packet);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *EthernetMsg_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    EthernetMsg *self = (EthernetMsg *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int EthernetMsg_init(EthernetMsg *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->packet = (Packet *)tmp;
    self->msg = *(EthernetF0_Message *)self->packet->cur_msg;
    self->cur_byte = 0;
    Py_INCREF(self->packet);

    return 0;
}


static Py_ssize_t EthernetMsg_len(PyObject *self){
    return (Py_ssize_t)((EthernetMsg *)self)->msg.Length;
}


static PyObject *EthernetMsg_next(EthernetMsg *self){
    if (self->cur_byte >= self->msg.Length)
        return NULL;

    uint8_t w = *(self->msg.Data + self->cur_byte);
    PyObject *val = Py_BuildValue("i", w);
    self->cur_byte += 1;
    return val;
}


static PyObject *EthernetMsg_bytes(EthernetMsg *self){
    int buf_size = sizeof(EthernetF0_IPH) + self->msg.IPH->Length;
    void *buffer = malloc(buf_size);
    memcpy(buffer, self->msg.IPH, buf_size);

    PyObject *result = Py_BuildValue("y#", (char *)buffer, buf_size);
    free(buffer);

    return result;
}

/* static PyObject *EthernetMsg_get_bus(EthernetMsg *self, void *closure){ */
/*     return Py_BuildValue("i", self->msg.IPH->BusID); */
/* } */

/* int EthernetMsg_set_bus(EthernetMsg *self, PyObject *args, void *closure){ */
/*     int val; */
/*     if (!PyArg_Parse(args, "i", &val)) */
/*         return -1; */

/*     self->msg.IPH->BusID = val; */
/*     return 0; */
/* } */

static PyObject *EthernetMsg_get_length(EthernetMsg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->Length);
}

static PyObject *EthernetMsg_get_rtc(EthernetMsg *self, void *closure){
    return Py_BuildValue("l", self->msg.IPH->IPTS);
}

static PyMemberDef EthernetMsg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(EthernetMsg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef EthernetMsg_methods[] = {
    {"__bytes__", (PyCFunction)EthernetMsg_bytes, METH_NOARGS, "Return the message as raw bytes"},
    {NULL}  /* Sentinel */
};


static PyGetSetDef EthernetMsg_getset[] = {
    {"rtc", (getter)EthernetMsg_get_rtc, NULL, "RTC from IPTS"},
    /* {"bus", (getter)EthernetMsg_get_bus, (setter)EthernetMsg_set_bus, "Bus ID"}, */
    {"length", (getter)EthernetMsg_get_length, NULL, "Length"},
    {NULL}
};


static PyObject *EthernetMsg_GetItem_seq(EthernetMsg *self, Py_ssize_t i){
    uint8_t byte = *(self->msg.Data + (uint8_t)i);
    return Py_BuildValue("i", byte);
}


static PyObject *EthernetMsg_GetItem(EthernetMsg *self, PyObject *key){
    if (!PySlice_Check(key)){
        int offset;
        PyArg_Parse(key, "i", &offset);
        return EthernetMsg_GetItem_seq(self, (Py_ssize_t)offset);
    }

    Py_ssize_t length, start, stop, step;
    PySlice_GetIndicesEx(key, EthernetMsg_len((PyObject *)self), &start, &stop, &step, &length);

    PyObject *t = PyTuple_New(length);
    for (Py_ssize_t i=0; i<(length); i++){
        PyTuple_SetItem(t, i, EthernetMsg_GetItem_seq(self, i + start));
    }

    return t;
} 


static PyMappingMethods EthernetMsg_map = {
    EthernetMsg_len, // sq_length
    (binaryfunc)EthernetMsg_GetItem, // sq_item
};


static PySequenceMethods EthernetMsg_seq = {
    EthernetMsg_len, // sq_length
    0, // sq_concat
    0, // sq_repeat
    (ssizeargfunc)EthernetMsg_GetItem_seq, // sq_item
    0, // sq_ass_item
    0, // sq_contains
    0, // sq_inplace_concat
    0, // sq_inplace_repeat
};


static PyTypeObject EthernetMsg_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.EthernetMsg",             /* tp_name */
    sizeof(EthernetMsg), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)EthernetMsg_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    &EthernetMsg_seq,            /* tp_as_sequence */
    &EthernetMsg_map,            /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Ethernet message object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    PyObject_SelfIter,                         /* tp_iter */
    (iternextfunc)EthernetMsg_next,                         /* tp_iternext */
    EthernetMsg_methods,             /* tp_methods */
    EthernetMsg_members,             /* tp_members */
    EthernetMsg_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)EthernetMsg_init,      /* tp_init */
    0,                         /* tp_alloc */
    EthernetMsg_new,                 /* tp_new */
};


PyObject *New_EthernetF0Msg(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &EthernetMsg_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_ethernet_class(PyObject *module){
	if (PyType_Ready(&EthernetMsg_Type) < 0)
        return;

    Py_INCREF(&EthernetMsg_Type);
    PyModule_AddObject(module, "EthernetMsg", (PyObject *)&EthernetMsg_Type);
}

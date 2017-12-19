
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
    if (!PyArg_ParseTuple(args, "O", &tmp))
        return -1;

    self->parent = (C10 *)tmp;

    /* if (status){ */
    /*     PyErr_Format(PyExc_RuntimeError, "I106PacketOpen: %s", I106ErrorString(status)); */
    /*     return -1; */
    /* } */

    return 0;
}


static PyObject *Packet_next(Packet *self){
    // Iterate over packet messages (depending on data type)
    return NULL;
}


static PyMemberDef Packet_members[] = {
    {"parent", T_OBJECT_EX, offsetof(Packet, parent), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef Packet_methods[] = {
    {NULL}  /* Sentinel */
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
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Packet_init,      /* tp_init */
    0,                         /* tp_alloc */
    Packet_new,                 /* tp_new */
};


void add_packet_class(PyObject *module){
	Packet_Type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Packet_Type) < 0)
        return;

    Py_INCREF(&Packet_Type);
    PyModule_AddObject(module, "Packet", (PyObject *)&Packet_Type);
}

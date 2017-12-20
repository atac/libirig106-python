
#include <Python.h>
#include "structmember.h"

#include "libirig106/src/irig106ch10.h"

#include "c10.h"
#include "packet.h"


static void C10_dealloc(C10 *self){
    I106C10Close(self->handle);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *C10_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    C10 *self = (C10 *)type->tp_alloc(type, 0);
    if (self != NULL)
        self->handle = -1;

    return (PyObject *)self;
}


static int C10_init(C10 *self, PyObject *args, PyObject *kwargs){
    if (!PyArg_ParseTuple(args, "s", &self->filename))
        return -1;

    I106Status status = I106C10Open(&self->handle, self->filename, READ);
    if (status){
        PyErr_Format(PyExc_RuntimeError, "I106C10Open: %s", I106ErrorString(status));
        return -1;
    }

    return 0;
}


static PyObject *C10_next(PyObject *self){
    PyObject *packet = New_Packet(self);

    return packet;
}


static PyMemberDef C10_members[] = {
    {"filename", T_STRING, offsetof(C10, filename), 0, "filename"},
    {"handle", T_INT, offsetof(C10, handle), 0, "handle"},
    {NULL}
};


static PyMethodDef C10_methods[] = {
    {NULL}  /* Sentinel */
};


static PyTypeObject C10_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.C10",             /* tp_name */
    sizeof(C10), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)C10_dealloc,                         /* tp_dealloc */
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
    "Chapter 10 file object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    PyObject_SelfIter,                         /* tp_iter */
    (iternextfunc)C10_next,                         /* tp_iternext */
    C10_methods,             /* tp_methods */
    C10_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)C10_init,      /* tp_init */
    0,                         /* tp_alloc */
    C10_new,                 /* tp_new */
};


void add_c10_class(PyObject *module){
	if (PyType_Ready(&C10_Type) < 0)
        return;

    Py_INCREF(&C10_Type);
    PyModule_AddObject(module, "C10", (PyObject *)&C10_Type);
}

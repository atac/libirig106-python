
#include <Python.h>
#include "structmember.h"

#include "libirig106/src/irig106ch10.h"
#include "libirig106/src/i106_decode_1553f1.h"

#include "c10.h"
#include "1553.h"


static void MS1553Msg_dealloc(MS1553Msg *self){
    Py_DECREF(self->packet);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *MS1553Msg_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    MS1553Msg *self = (MS1553Msg *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int MS1553Msg_init(MS1553Msg *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->packet = (Packet *)tmp;
    self->msg = *self->packet->MS1553_MSG;
    Py_INCREF(self->packet);

    return 0;
}


static Py_ssize_t MS1553Msg_len(PyObject *self){
    return (Py_ssize_t)((MS1553Msg *)self)->msg.WordCount;
}


// TODO: iterate over data words
static PyObject *MS1553Msg_next(MS1553Msg *self){
    return NULL;
}


static PyMemberDef MS1553Msg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(MS1553Msg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef MS1553Msg_methods[] = {
    {NULL}  /* Sentinel */
};


static PyGetSetDef MS1553Msg_getset[] = {
    {NULL},
};


static PySequenceMethods MS1553Msg_seq = {
    MS1553Msg_len, // sq_length
    0, // sq_concat
    0, // sq_repeat
    0, // sq_item
    0, // sq_ass_item
    0, // sq_contains
    0, // sq_inplace_concat
    0  // sq_inplace_repeat
};


static PyTypeObject MS1553Msg_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.MS1553Msg",             /* tp_name */
    sizeof(MS1553Msg), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)MS1553Msg_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    &MS1553Msg_seq,            /* tp_as_sequence */
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
    0,                         /* tp_iternext */
    MS1553Msg_methods,             /* tp_methods */
    MS1553Msg_members,             /* tp_members */
    MS1553Msg_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)MS1553Msg_init,      /* tp_init */
    0,                         /* tp_alloc */
    MS1553Msg_new,                 /* tp_new */
};


PyObject *New_MS1553Msg(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &MS1553Msg_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_1553_class(PyObject *module){
	if (PyType_Ready(&MS1553Msg_Type) < 0)
        return;

    Py_INCREF(&MS1553Msg_Type);
    PyModule_AddObject(module, "MS1553Msg", (PyObject *)&MS1553Msg_Type);
}

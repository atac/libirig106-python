
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
    self->cur_word = 0;
    Py_INCREF(self->packet);

    return 0;
}


static Py_ssize_t MS1553Msg_len(PyObject *self){
    return (Py_ssize_t)((MS1553Msg *)self)->msg.WordCount;
}


static PyObject *MS1553Msg_next(MS1553Msg *self){
    if (self->cur_word >= self->msg.WordCount)
        return NULL;

    uint16_t w = *(self->msg.Data + self->cur_word);
    PyObject *val = Py_BuildValue("i", w);
    self->cur_word += 1;
    return val;
}


static PyObject *MS1553Msg_get_we(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->WordError);
}

static PyObject *MS1553Msg_get_se(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->SyncError);
}

static PyObject *MS1553Msg_get_wc(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->WordCountError);
}

static PyObject *MS1553Msg_get_timeout(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->Timeout);
}

static PyObject *MS1553Msg_get_fe(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->FormatError);
}

static PyObject *MS1553Msg_get_rt2rt(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->RT2RT);
}

static PyObject *MS1553Msg_get_me(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->MessageError);
}

static PyObject *MS1553Msg_get_bus(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->BusID);
}

static PyObject *MS1553Msg_get_gap(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->GapTime1);
}

static PyObject *MS1553Msg_get_length(MS1553Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->Length);
}

static PyMemberDef MS1553Msg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(MS1553Msg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef MS1553Msg_methods[] = {
    {NULL}  /* Sentinel */
};


static PyGetSetDef MS1553Msg_getset[] = {
    {"we", (getter)MS1553Msg_get_we, NULL, "Word error"},
    {"se", (getter)MS1553Msg_get_se, NULL, "Sync error"},
    {"le", (getter)MS1553Msg_get_wc, NULL, "Word count error"},
    {"timeout", (getter)MS1553Msg_get_timeout, NULL, "Timeout"},
    {"fe", (getter)MS1553Msg_get_fe, NULL, "Format error"},
    {"rt2rt", (getter)MS1553Msg_get_rt2rt, NULL, "RT to RT flag"},
    {"me", (getter)MS1553Msg_get_me, NULL, "Message error"},
    {"bus", (getter)MS1553Msg_get_bus, NULL, "Bus ID"},
    {"gap_time", (getter)MS1553Msg_get_gap, NULL, "Gap time"},
    {"length", (getter)MS1553Msg_get_length, NULL, "Length"},
    {NULL}
};


static PyObject *MS1553Msg_GetItem_seq(MS1553Msg *self, Py_ssize_t i){
    uint16_t w = *(self->msg.Data + (uint16_t)i);
    return Py_BuildValue("i", w);
}


static PyObject *MS1553Msg_GetItem(MS1553Msg *self, PyObject *key){
    if (!PySlice_Check(key)){
        int offset;
        PyArg_Parse(key, "i", &offset);
        return MS1553Msg_GetItem_seq(self, (Py_ssize_t)offset);
    }

    Py_ssize_t length, start, stop, step;
    PySlice_GetIndicesEx(key, MS1553Msg_len((PyObject *)self), &start, &stop, &step, &length);

    PyObject *t = PyTuple_New(length);
    for (Py_ssize_t i=0; i<(length); i++){
        PyTuple_SetItem(t, i, MS1553Msg_GetItem_seq(self, i + start));
    }

    return t;
} 


static PyMappingMethods MS1553Msg_map = {
    MS1553Msg_len, // sq_length
    (binaryfunc)MS1553Msg_GetItem, // sq_item
};


static PySequenceMethods MS1553Msg_seq = {
    MS1553Msg_len, // sq_length
    0, // sq_concat
    0, // sq_repeat
    (ssizeargfunc)MS1553Msg_GetItem_seq, // sq_item
    0, // sq_ass_item
    0, // sq_contains
    0, // sq_inplace_concat
    0, // sq_inplace_repeat
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
    &MS1553Msg_map,            /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "1553 message object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    PyObject_SelfIter,                         /* tp_iter */
    (iternextfunc)MS1553Msg_next,                         /* tp_iternext */
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

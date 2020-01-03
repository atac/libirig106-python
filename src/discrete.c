
#include <Python.h>
#include "structmember.h"

#include "c10.h"
#include "discrete.h"


static void DiscreteMsg_dealloc(DiscreteMsg *self){
    Py_DECREF(self->packet);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *DiscreteMsg_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    DiscreteMsg *self = (DiscreteMsg *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int DiscreteMsg_init(DiscreteMsg *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->packet = (Packet *)tmp;
    self->msg = *(DiscreteF1_Message *)self->packet->cur_msg;
    self->cur_index = 0;
    Py_INCREF(self->packet);

    return 0;
}


static Py_ssize_t DiscreteMsg_len(PyObject *self){
    return (Py_ssize_t)32;
}


static PyObject *DiscreteMsg_next(DiscreteMsg *self){
    if (self->cur_index >= 32)
        return NULL;

    int b = (self->msg.Data >> self->cur_index) & 0x1;
    PyObject *value = Py_BuildValue("i", b);
    self->cur_index += 1;
    return value;
}


static PyObject *DiscreteMsg_bytes(DiscreteMsg *self){
    int buf_size = 12;
    void *buffer = malloc(buf_size);
    memcpy(buffer, self->msg.IPTS, buf_size);

    PyObject *result = Py_BuildValue("y#", (char *)buffer, buf_size);
    free(buffer);

    return result;
}

static PyObject *DiscreteMsg_get_length(DiscreteMsg *self, void *closure){
    return Py_BuildValue("i", 12);
}

static PyMemberDef DiscreteMsg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(DiscreteMsg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef DiscreteMsg_methods[] = {
    {"__bytes__", (PyCFunction)DiscreteMsg_bytes, METH_NOARGS, "Return the message as raw bytes"},
    {NULL}  /* Sentinel */
};


static PyGetSetDef DiscreteMsg_getset[] = {
    // TODO: some way to set RTC
    /* {"rtc", (getter)DiscreteMsg_get_rtc, NULL, "RTC from IPTS"}, */
    /* {"length", (getter)DiscreteMsg_get_length, (setter)DiscreteMsg_set_length, "Length"}, */
    /* {"le", (getter)DiscreteMsg_get_le, (setter)DiscreteMsg_set_le, "Length error flag"}, */
    /* {"dce", (getter)DiscreteMsg_get_dce, (setter)DiscreteMsg_set_dce, "Data CRC error flag"}, */
    /* {"netid", (getter)DiscreteMsg_get_netid, (setter)DiscreteMsg_set_netid, "Network ID"}, */
    /* {"speed", (getter)DiscreteMsg_get_speed, (setter)DiscreteMsg_set_speed, "Bitrate for current Network ID"}, */
    /* {"content", (getter)DiscreteMsg_get_content, (setter)DiscreteMsg_set_content, "Content indicator"}, */
    /* {"fe", (getter)DiscreteMsg_get_fe, (setter)DiscreteMsg_set_fe, "Frame Error flag"}, */
    /* {"fce", (getter)DiscreteMsg_get_fce, (setter)DiscreteMsg_set_fce, "Frame CRC Error flag"}, */
    {NULL}
};


static PyObject *DiscreteMsg_GetItem_seq(DiscreteMsg *self, Py_ssize_t i){
    int b = (self->msg.Data >> i) & 0x1;
    return Py_BuildValue("i", b);
}


static PyObject *DiscreteMsg_GetItem(DiscreteMsg *self, PyObject *key){
    if (!PySlice_Check(key)){
        int offset;
        PyArg_Parse(key, "i", &offset);
        return DiscreteMsg_GetItem_seq(self, (Py_ssize_t)offset);
    }

    Py_ssize_t length, start, stop, step;
    PySlice_GetIndicesEx(key, DiscreteMsg_len((PyObject *)self), &start, &stop, &step, &length);

    PyObject *t = PyTuple_New(length);
    for (Py_ssize_t i=0; i<(length); i++){
        PyTuple_SetItem(t, i, DiscreteMsg_GetItem_seq(self, i + start));
    }

    return t;
} 


static PyMappingMethods DiscreteMsg_map = {
    DiscreteMsg_len, // sq_length
    (binaryfunc)DiscreteMsg_GetItem, // sq_item
};


static PySequenceMethods DiscreteMsg_seq = {
    DiscreteMsg_len, // sq_length
    0, // sq_concat
    0, // sq_repeat
    (ssizeargfunc)DiscreteMsg_GetItem_seq, // sq_item
    0, // sq_ass_item
    0, // sq_contains
    0, // sq_inplace_concat
    0, // sq_inplace_repeat
};


static PyTypeObject DiscreteMsg_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.DiscreteMsg",             /* tp_name */
    sizeof(DiscreteMsg), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)DiscreteMsg_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    &DiscreteMsg_seq,            /* tp_as_sequence */
    &DiscreteMsg_map,            /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Discrete message object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    PyObject_SelfIter,                         /* tp_iter */
    (iternextfunc)DiscreteMsg_next,                         /* tp_iternext */
    DiscreteMsg_methods,             /* tp_methods */
    DiscreteMsg_members,             /* tp_members */
    DiscreteMsg_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)DiscreteMsg_init,      /* tp_init */
    0,                         /* tp_alloc */
    DiscreteMsg_new,                 /* tp_new */
};


PyObject *New_DiscreteF1Msg(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &DiscreteMsg_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_discrete_class(PyObject *module){
	if (PyType_Ready(&DiscreteMsg_Type) < 0)
        return;

    Py_INCREF(&DiscreteMsg_Type);
    PyModule_AddObject(module, "DiscreteMsg", (PyObject *)&DiscreteMsg_Type);
}


#include <Python.h>
#include "structmember.h"

#include "c10.h"
#include "video.h"


static void VideoF0Msg_dealloc(VideoF0Msg *self){
    Py_DECREF(self->packet);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *VideoF0Msg_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    VideoF0Msg *self = (VideoF0Msg *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int VideoF0Msg_init(VideoF0Msg *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->packet = (Packet *)tmp;
    self->msg = *(VideoF0_Message *)self->packet->cur_msg;
    Py_INCREF(self->packet);

    return 0;
}


static Py_ssize_t VideoF0Msg_len(PyObject *self){
    Py_ssize_t msg_size = 188;
    if (((VideoF0Msg *)self)->msg.CSDW->ET)
        msg_size += sizeof(VideoF0_IPH);
    return msg_size;
}


static PyObject *VideoF0Msg_bytes(VideoF0Msg *self){
    int buf_size = 188;
    if (self->msg.CSDW->ET)
        buf_size += sizeof(VideoF0_IPH);

    void *buffer = malloc(buf_size);
    if (self->msg.CSDW->IPH)
        memcpy(buffer, self->msg.IPH, buf_size);
    else
        memcpy(buffer, self->msg.Data, buf_size);

    PyObject *result = Py_BuildValue("y#", (char *)buffer, buf_size);
    free(buffer);

    return result;
}


static PyMemberDef VideoF0Msg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(VideoF0Msg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef VideoF0Msg_methods[] = {
    {"__bytes__", (PyCFunction)VideoF0Msg_bytes, METH_NOARGS, "Return the message as raw bytes"},
    {NULL}  /* Sentinel */
};


static PyGetSetDef VideoF0Msg_getset[] = {
    /* {"rtc", (getter)VideoF0Msg_get_rtc, NULL, "RTC from IPTS"}, */
    {NULL}
};


static PyObject *VideoF0Msg_GetItem_seq(VideoF0Msg *self, Py_ssize_t i){
    uint16_t w = *(self->msg.Data + (uint16_t)i);
    return Py_BuildValue("i", w);
}


static PyObject *VideoF0Msg_GetItem(VideoF0Msg *self, PyObject *key){
    if (!PySlice_Check(key)){
        int offset;
        PyArg_Parse(key, "i", &offset);
        return VideoF0Msg_GetItem_seq(self, (Py_ssize_t)offset);
    }

    Py_ssize_t length, start, stop, step;
    PySlice_GetIndicesEx(key, VideoF0Msg_len((PyObject *)self), &start, &stop, &step, &length);

    PyObject *t = PyTuple_New(length);
    for (Py_ssize_t i=0; i<(length); i++){
        PyTuple_SetItem(t, i, VideoF0Msg_GetItem_seq(self, i + start));
    }

    return t;
} 


static PyMappingMethods VideoF0Msg_map = {
    VideoF0Msg_len, // sq_length
    (binaryfunc)VideoF0Msg_GetItem, // sq_item
};


static PySequenceMethods VideoF0Msg_seq = {
    VideoF0Msg_len, // sq_length
    0, // sq_concat
    0, // sq_repeat
    (ssizeargfunc)VideoF0Msg_GetItem_seq, // sq_item
    0, // sq_ass_item
    0, // sq_contains
    0, // sq_inplace_concat
    0, // sq_inplace_repeat
};


static PyTypeObject VideoF0Msg_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.VideoF0Msg",             /* tp_name */
    sizeof(VideoF0Msg), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)VideoF0Msg_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    &VideoF0Msg_seq,            /* tp_as_sequence */
    &VideoF0Msg_map,            /* tp_as_mapping */
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
    0,                         /* tp_iternext */
    VideoF0Msg_methods,             /* tp_methods */
    VideoF0Msg_members,             /* tp_members */
    VideoF0Msg_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)VideoF0Msg_init,      /* tp_init */
    0,                         /* tp_alloc */
    VideoF0Msg_new,                 /* tp_new */
};


PyObject *New_VideoF0Message(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &VideoF0Msg_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_videof0_class(PyObject *module){
	if (PyType_Ready(&VideoF0Msg_Type) < 0)
        return;

    Py_INCREF(&VideoF0Msg_Type);
    PyModule_AddObject(module, "VideoF0Msg", (PyObject *)&VideoF0Msg_Type);
}

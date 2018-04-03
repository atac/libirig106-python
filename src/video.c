
#include <Python.h>
#include "structmember.h"

#include "libirig106/src/irig106ch10.h"
#include "libirig106/src/i106_decode_video.h"

#include "c10.h"
#include "video.h"


static void VideoMsg_dealloc(VideoMsg *self){
    Py_DECREF(self->packet);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *VideoMsg_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    VideoMsg *self = (VideoMsg *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int VideoMsg_init(VideoMsg *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->packet = (Packet *)tmp;
    self->msg = *self->packet->Video_MSG;
    Py_INCREF(self->packet);

    return 0;
}


static Py_ssize_t VideoMsg_len(PyObject *self){
    Py_ssize_t msg_size = 188;
    if (((VideoMsg *)self)->msg.CSDW->ET)
        msg_size += sizeof(VideoF0_IPH);

    return (Py_ssize_t)((VideoMsg *)self)->packet->DataLength / msg_size;
}


static PyObject *VideoMsg_bytes(VideoMsg *self){
    int buf_size = 188;
    if (self->msg.CSDW->ET)
        buf_size += sizeof(VideoF0_IPH);

    void *buffer = malloc(buf_size);
    memcpy(buffer, self->msg.IPH, buf_size);

    PyObject *result = Py_BuildValue("y#", (char *)buffer, buf_size);
    free(buffer);

    return result;
}


static PyMemberDef VideoMsg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(VideoMsg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef VideoMsg_methods[] = {
    {"__bytes__", (PyCFunction)VideoMsg_bytes, METH_NOARGS, "Return the message as raw bytes"},
    {NULL}  /* Sentinel */
};


static PyGetSetDef VideoMsg_getset[] = {
    /* {"rtc", (getter)VideoMsg_get_rtc, NULL, "RTC from IPTS"}, */
    {NULL}
};


static PyObject *VideoMsg_GetItem_seq(VideoMsg *self, Py_ssize_t i){
    uint16_t w = *(self->msg.Data + (uint16_t)i);
    return Py_BuildValue("i", w);
}


static PyObject *VideoMsg_GetItem(VideoMsg *self, PyObject *key){
    if (!PySlice_Check(key)){
        int offset;
        PyArg_Parse(key, "i", &offset);
        return VideoMsg_GetItem_seq(self, (Py_ssize_t)offset);
    }

    Py_ssize_t length, start, stop, step;
    PySlice_GetIndicesEx(key, VideoMsg_len((PyObject *)self), &start, &stop, &step, &length);

    PyObject *t = PyTuple_New(length);
    for (Py_ssize_t i=0; i<(length); i++){
        PyTuple_SetItem(t, i, VideoMsg_GetItem_seq(self, i + start));
    }

    return t;
} 


static PyMappingMethods VideoMsg_map = {
    VideoMsg_len, // sq_length
    (binaryfunc)VideoMsg_GetItem, // sq_item
};


static PySequenceMethods VideoMsg_seq = {
    VideoMsg_len, // sq_length
    0, // sq_concat
    0, // sq_repeat
    (ssizeargfunc)VideoMsg_GetItem_seq, // sq_item
    0, // sq_ass_item
    0, // sq_contains
    0, // sq_inplace_concat
    0, // sq_inplace_repeat
};


static PyTypeObject VideoMsg_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.VideoMsg",             /* tp_name */
    sizeof(VideoMsg), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)VideoMsg_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    &VideoMsg_seq,            /* tp_as_sequence */
    &VideoMsg_map,            /* tp_as_mapping */
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
    VideoMsg_methods,             /* tp_methods */
    VideoMsg_members,             /* tp_members */
    VideoMsg_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)VideoMsg_init,      /* tp_init */
    0,                         /* tp_alloc */
    VideoMsg_new,                 /* tp_new */
};


PyObject *New_VideoMsg(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &VideoMsg_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_1553_class(PyObject *module){
	if (PyType_Ready(&VideoMsg_Type) < 0)
        return;

    Py_INCREF(&VideoMsg_Type);
    PyModule_AddObject(module, "VideoMsg", (PyObject *)&VideoMsg_Type);
}

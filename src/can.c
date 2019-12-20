
#include <Python.h>
#include "structmember.h"

#include "c10.h"
#include "can.h"


static void CANMsg_dealloc(CANMsg *self){
    Py_DECREF(self->packet);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *CANMsg_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    CANMsg *self = (CANMsg *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int CANMsg_init(CANMsg *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->packet = (Packet *)tmp;
    self->msg = *(CAN_Message *)self->packet->cur_msg;
    self->cur_byte = 0;
    Py_INCREF(self->packet);

    return 0;
}


static Py_ssize_t CANMsg_len(PyObject *self){
    return (Py_ssize_t)((CANMsg *)self)->msg.IPH->Length;
}


static PyObject *CANMsg_next(CANMsg *self){
    if (self->cur_byte >= self->msg.IPH->Length)
        return NULL;

    uint8_t b = *(self->msg.Data + self->cur_byte);
    PyObject *value = Py_BuildValue("i", b);
    self->cur_byte += 1;
    return value;
}


static PyObject *CANMsg_bytes(CANMsg *self){
    int buf_size = sizeof(CAN_IPH) + self->msg.IPH->Length;
    void *buffer = malloc(buf_size);
    memcpy(buffer, self->msg.IPH, buf_size);

    PyObject *result = Py_BuildValue("y#", (char *)buffer, buf_size);
    free(buffer);

    return result;
}

/* static PyObject *CANMsg_get_netid(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("i", self->msg.IPH->NetID); */
/* } */

/* int CANMsg_set_netid(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->NetID = value; */
/*     return 0; */
/* } */

/* static PyObject *CANMsg_get_length(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("i", self->msg.IPH->Length); */
/* } */

/* int CANMsg_set_length(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->Length = value; */
/*     return 0; */
/* } */

/* static PyObject *CANMsg_get_le(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("i", self->msg.IPH->LengthError); */
/* } */

/* int CANMsg_set_le(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->LengthError = value; */
/*     return 0; */
/* } */

/* static PyObject *CANMsg_get_rtc(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("l", self->msg.IPH->IPTS); */
/* } */

/* static PyObject *CANMsg_get_speed(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("l", self->msg.IPH->Speed); */
/* } */

/* int CANMsg_set_speed(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->Speed = value; */
/*     return 0; */
/* } */

/* static PyObject *CANMsg_get_fce(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("l", self->msg.IPH->FrameCRCError); */
/* } */

/* int CANMsg_set_fce(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->FrameCRCError = value; */
/*     return 0; */
/* } */

/* static PyObject *CANMsg_get_content(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("l", self->msg.IPH->Content); */
/* } */

/* int CANMsg_set_content(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->Content = value; */
/*     return 0; */
/* } */

/* static PyObject *CANMsg_get_fe(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("l", self->msg.IPH->FrameError); */
/* } */

/* int CANMsg_set_fe(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->FrameError = value; */
/*     return 0; */
/* } */

/* static PyObject *CANMsg_get_dce(CANMsg *self, void *closure){ */
/*     return Py_BuildValue("l", self->msg.IPH->DataCRCError); */
/* } */

/* int CANMsg_set_dce(CANMsg *self, PyObject *args, void *closure){ */
/*     int value; */
/*     if (!PyArg_Parse(args, "i", &value)) */
/*         return -1; */

/*     self->msg.IPH->DataCRCError = value; */
/*     return 0; */
/* } */

static PyMemberDef CANMsg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(CANMsg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef CANMsg_methods[] = {
    {"__bytes__", (PyCFunction)CANMsg_bytes, METH_NOARGS, "Return the message as raw bytes"},
    {NULL}  /* Sentinel */
};


static PyGetSetDef CANMsg_getset[] = {
    // TODO: some way to set RTC
    /* {"rtc", (getter)CANMsg_get_rtc, NULL, "RTC from IPTS"}, */
    /* {"length", (getter)CANMsg_get_length, (setter)CANMsg_set_length, "Length"}, */
    /* {"le", (getter)CANMsg_get_le, (setter)CANMsg_set_le, "Length error flag"}, */
    /* {"dce", (getter)CANMsg_get_dce, (setter)CANMsg_set_dce, "Data CRC error flag"}, */
    /* {"netid", (getter)CANMsg_get_netid, (setter)CANMsg_set_netid, "Network ID"}, */
    /* {"speed", (getter)CANMsg_get_speed, (setter)CANMsg_set_speed, "Bitrate for current Network ID"}, */
    /* {"content", (getter)CANMsg_get_content, (setter)CANMsg_set_content, "Content indicator"}, */
    /* {"fe", (getter)CANMsg_get_fe, (setter)CANMsg_set_fe, "Frame Error flag"}, */
    /* {"fce", (getter)CANMsg_get_fce, (setter)CANMsg_set_fce, "Frame CRC Error flag"}, */
    {NULL}
};


static PyObject *CANMsg_GetItem_seq(CANMsg *self, Py_ssize_t i){
    uint8_t byte = *(self->msg.Data + (uint8_t)i);
    return Py_BuildValue("i", byte);
}


static PyObject *CANMsg_GetItem(CANMsg *self, PyObject *key){
    if (!PySlice_Check(key)){
        int offset;
        PyArg_Parse(key, "i", &offset);
        return CANMsg_GetItem_seq(self, (Py_ssize_t)offset);
    }

    Py_ssize_t length, start, stop, step;
    PySlice_GetIndicesEx(key, CANMsg_len((PyObject *)self), &start, &stop, &step, &length);

    PyObject *t = PyTuple_New(length);
    for (Py_ssize_t i=0; i<(length); i++){
        PyTuple_SetItem(t, i, CANMsg_GetItem_seq(self, i + start));
    }

    return t;
} 


static PyMappingMethods CANMsg_map = {
    CANMsg_len, // sq_length
    (binaryfunc)CANMsg_GetItem, // sq_item
};


static PySequenceMethods CANMsg_seq = {
    CANMsg_len, // sq_length
    0, // sq_concat
    0, // sq_repeat
    (ssizeargfunc)CANMsg_GetItem_seq, // sq_item
    0, // sq_ass_item
    0, // sq_contains
    0, // sq_inplace_concat
    0, // sq_inplace_repeat
};


static PyTypeObject CANMsg_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.CANMsg",             /* tp_name */
    sizeof(CANMsg), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)CANMsg_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    &CANMsg_seq,            /* tp_as_sequence */
    &CANMsg_map,            /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "CAN message object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    PyObject_SelfIter,                         /* tp_iter */
    (iternextfunc)CANMsg_next,                         /* tp_iternext */
    CANMsg_methods,             /* tp_methods */
    CANMsg_members,             /* tp_members */
    CANMsg_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)CANMsg_init,      /* tp_init */
    0,                         /* tp_alloc */
    CANMsg_new,                 /* tp_new */
};


PyObject *New_CANMsg(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &CANMsg_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_can_class(PyObject *module){
	if (PyType_Ready(&CANMsg_Type) < 0)
        return;

    Py_INCREF(&CANMsg_Type);
    PyModule_AddObject(module, "CANMsg", (PyObject *)&CANMsg_Type);
}

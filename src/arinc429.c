
#include <Python.h>
#include "structmember.h"

#include "c10.h"
#include "arinc429.h"


static void Arinc429Msg_dealloc(Arinc429Msg *self){
    Py_DECREF(self->packet);
    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyObject *Arinc429Msg_new(PyTypeObject *type, PyObject *args, PyObject *kwargs){
    Arinc429Msg *self = (Arinc429Msg *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}


static int Arinc429Msg_init(Arinc429Msg *self, PyObject *args, PyObject *kwargs){
    I106Status status;

    PyObject *tmp;
    if (!PyArg_ParseTuple(args, "O", &tmp)){
        printf("No arguments!\n");
        return -1;
    }

    self->packet = (Packet *)tmp;
    self->msg = *(Arinc429F0_Message *)self->packet->cur_msg;
    self->cur_byte = 0;
    Py_INCREF(self->packet);

    return 0;
}


static PyObject *Arinc429Msg_bytes(Arinc429Msg *self){
    int buf_size = sizeof(Arinc429F0_IPH) + sizeof(Arinc429F0_Data);
    void *buffer = malloc(buf_size);
    memcpy(buffer, self->msg.IPH, buf_size);

    PyObject *result = Py_BuildValue("y#", (char *)buffer, buf_size);
    free(buffer);

    return result;
}

static PyObject *Arinc429Msg_get_gap(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->GapTime);
}

int Arinc429Msg_set_gap(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.IPH->GapTime = value;
    return 0;
}

static PyObject *Arinc429Msg_get_speed(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->BusSpeed);
}

int Arinc429Msg_set_speed(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.IPH->BusSpeed = value;
    return 0;
}

static PyObject *Arinc429Msg_get_pe(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->ParityError);
}

int Arinc429Msg_set_pe(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.IPH->ParityError = value;
    return 0;
}

static PyObject *Arinc429Msg_get_fe(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->FormatError);
}

int Arinc429Msg_set_fe(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.IPH->FormatError = value;
    return 0;
}

static PyObject *Arinc429Msg_get_bus(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.IPH->BusNumber);
}

int Arinc429Msg_set_bus(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.IPH->BusNumber = value;
    return 0;
}

static PyObject *Arinc429Msg_get_label(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.Data->Label);
}

int Arinc429Msg_set_label(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.Data->Label = value;
    return 0;
}

static PyObject *Arinc429Msg_get_sdi(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.Data->SDI);
}

int Arinc429Msg_set_sdi(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.Data->SDI = value;
    return 0;
}

static PyObject *Arinc429Msg_get_data(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.Data->Data);
}

int Arinc429Msg_set_data(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.Data->Data = value;
    return 0;
}

static PyObject *Arinc429Msg_get_ssm(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.Data->SSM);
}

int Arinc429Msg_set_ssm(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.Data->SSM = value;
    return 0;
}

static PyObject *Arinc429Msg_get_parity(Arinc429Msg *self, void *closure){
    return Py_BuildValue("i", self->msg.Data->Parity);
}

int Arinc429Msg_set_parity(Arinc429Msg *self, PyObject *args, void *closure){
    int value;
    if (!PyArg_Parse(args, "i", &value))
        return -1;

    self->msg.Data->Parity = value;
    return 0;
}

static PyMemberDef Arinc429Msg_members[] = {
    {"packet", T_OBJECT_EX, offsetof(Arinc429Msg, packet), 0, "Parent C10 object"},
    {NULL}
};


static PyMethodDef Arinc429Msg_methods[] = {
    {"__bytes__", (PyCFunction)Arinc429Msg_bytes, METH_NOARGS, "Return the message as raw bytes"},
    {NULL}  /* Sentinel */
};


static PyGetSetDef Arinc429Msg_getset[] = {
    {"gap", (getter)Arinc429Msg_get_gap, (setter)Arinc429Msg_set_gap, "Gap time"},
    {"speed", (getter)Arinc429Msg_get_speed, (setter)Arinc429Msg_set_speed, "Bus speed"},
    {"pe", (getter)Arinc429Msg_get_pe, (setter)Arinc429Msg_set_pe, "Parity error flag"},
    {"fe", (getter)Arinc429Msg_get_fe, (setter)Arinc429Msg_set_fe, "Format error flag"},
    {"bus", (getter)Arinc429Msg_get_bus, (setter)Arinc429Msg_set_bus, "Bus number"},
    {"label", (getter)Arinc429Msg_get_label, (setter)Arinc429Msg_set_label, "Arinc label"},
    {"sdi", (getter)Arinc429Msg_get_sdi, (setter)Arinc429Msg_set_sdi, "Source/Destination Identifier"},
    {"data", (getter)Arinc429Msg_get_data, (setter)Arinc429Msg_set_data, "Message data"},
    {"ssm", (getter)Arinc429Msg_get_ssm, (setter)Arinc429Msg_set_ssm, "Sign/Status matrix"},
    {"parity", (getter)Arinc429Msg_get_parity, (setter)Arinc429Msg_set_parity, "Parity bit"},
    {NULL}
};


static PyTypeObject Arinc429Msg_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "i106.Arinc429Msg",             /* tp_name */
    sizeof(Arinc429Msg), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Arinc429Msg_dealloc,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,            /* tp_as_sequence */
    0,            /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
    "Arinc429 message object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    PyObject_SelfIter,                         /* tp_iter */
    0,                         /* tp_iternext */
    Arinc429Msg_methods,             /* tp_methods */
    Arinc429Msg_members,             /* tp_members */
    Arinc429Msg_getset,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Arinc429Msg_init,      /* tp_init */
    0,                         /* tp_alloc */
    Arinc429Msg_new,                 /* tp_new */
};


PyObject *New_Arinc429F0Msg(PyObject *parent){
    PyObject *packet_args = Py_BuildValue("(O)", parent);
    PyObject *p = PyObject_CallObject((PyObject *) &Arinc429Msg_Type, packet_args);
    Py_DECREF(packet_args);
    return p;
}


void add_arinc_class(PyObject *module){
	if (PyType_Ready(&Arinc429Msg_Type) < 0)
        return;

    Py_INCREF(&Arinc429Msg_Type);
    PyModule_AddObject(module, "Arinc429Msg", (PyObject *)&Arinc429Msg_Type);
}

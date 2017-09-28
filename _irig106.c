
#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libirig106/src/irig106ch10.h"


// Functions

static PyObject *i106_open(PyObject *self, PyObject *args){
    char *filename;
    int mode;
    if (!PyArg_ParseTuple(args, "si", &filename, &mode))
        return NULL;

    int handle;
    I106Status status = I106C10Open(&handle, filename, (I106C10Mode)mode);

    return Py_BuildValue("ii", (int)status, handle);
}


static PyObject *hello(PyObject *self, PyObject *args){
    printf("Hello from c!\n");

    Py_RETURN_NONE;
}


// Module initialization

static PyMethodDef i106_methods[] = {
    {"i106_open", (PyCFunction)i106_open, METH_VARARGS, NULL},
    /* {"hello", (PyCFunction)hello, METH_NOARGS, "Hello world function"}, */
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC init_i106(void){
    PyObject *m = Py_InitModule3("_i106", i106_methods, "This is a placeholder.");
    if (m == NULL)
        return;
}

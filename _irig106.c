
#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static PyObject *hello(PyObject *self){
    printf("Hello from c!\n");

    Py_RETURN_NONE;
}

static char i106_docstring[] = "This is a placeholder.";
static PyMethodDef i106_methods[] = {
    {"hello", (PyCFunction)hello, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

void i106_init_module(PyObject *module){}

PyMODINIT_FUNC init_i106(void){
    PyObject *m = Py_InitModule3("_i106", i106_methods, i106_docstring);
    if (m == NULL)
        return;
}


#include <Python.h>
#include <stdio.h>

PyObject * i106_open(PyObject *self, PyObject *args){
    printf("Open");

    Py_RETURN_NONE;
}

PyObject * i106_hello(){
    printf("Hello");

    Py_RETURN_NONE;
}

static PyMethodDef funcs[] = {
    {"hello", (PyCFunction)i106_hello, METH_NOARGS, "Hello world test"},
    {"i106_open", (PyCFunction)i106_open, METH_VARARGS, "Open a chapter 10 file"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_i106(void){
    Py_InitModule("_i106", funcs);
}

int main(int argc, char *argv[]){
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    init_i106();
}

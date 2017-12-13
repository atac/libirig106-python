
#include <Python.h>
#include <stdio.h>


PyObject * _i106_open(PyObject *self, PyObject *args){
    printf("Open\n");

    Py_RETURN_NONE;
}

PyObject * _i106_hello(PyObject *self, PyObject *args){
    printf("Hello\n");

    
    Py_RETURN_NONE;
}

static PyMethodDef funcs[] = {
    {"hello", _i106_hello, METH_VARARGS, "Hello world test"},
    {"i106_open", _i106_open, METH_VARARGS, "Open a chapter 10 file"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef _i106 = {
    PyModuleDef_HEAD_INIT, "_i106", "docstring", -1, funcs
};

PyMODINIT_FUNC PyInit__i106(void){
    return PyModule_Create(&_i106);
}

int main(int argc, char *argv[]){
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    /* Add a built-in module, before Py_Initialize */
    PyImport_AppendInittab("_i106", PyInit__i106);

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Optionally import the module; alternatively,
     *        import can be deferred until the embedded
     *        script
     *               imports it. */
    PyImport_ImportModule("spam");

    PyMem_RawFree(program);
    return 0;
}

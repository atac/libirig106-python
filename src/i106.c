
#include <Python.h>
#include <stdio.h>

#include "libirig106/src/irig106ch10.h"
#include "c10.h"
#include "packet.h"
#include "1553.h"
#include "video.h"


static PyMethodDef funcs[] = {
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef _i106 = {
    PyModuleDef_HEAD_INIT, "_i106", "docstring", -1, funcs
};

PyMODINIT_FUNC PyInit_i106(void){
    PyObject *m = PyModule_Create(&_i106);
    add_c10_class(m);
    add_packet_class(m);
    add_1553_class(m);
    add_video_class(m);
    return m;
}

int main(int argc, char *argv[]){
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    PyImport_AppendInittab("_i106", PyInit_i106);

    Py_SetProgramName(program);

    Py_Initialize();

    PyImport_ImportModule("_i106");

    PyMem_RawFree(program);
    return 0;
}

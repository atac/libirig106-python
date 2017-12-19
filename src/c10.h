
typedef struct {
    PyObject_HEAD
    char *filename;
    int handle;
} C10;

void add_c10_class(PyObject *module);

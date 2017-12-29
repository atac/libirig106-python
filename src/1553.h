
#include "packet.h"


typedef struct MS1553Msg {
    PyObject_HEAD
    Packet *packet;
    MS1553F1_Message msg;
} MS1553Msg;

void add_1553_class(PyObject *module);

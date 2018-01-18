
#include "packet.h"


typedef struct MS1553Msg {
    PyObject_HEAD
    Packet *packet;
    MS1553F1_Message msg;
    int cur_word;
} MS1553Msg;

PyObject *New_MS1553Msg(PyObject *parent);
void add_1553_class(PyObject *module);

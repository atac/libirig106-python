
#include "packet.h"
#include "libirig106/src/i106_decode_1553f1.h"


typedef struct MS1553Msg {
    PyObject_HEAD
    Packet *packet;
    MS1553F1_Message msg;
    int cur_word;
} MS1553Msg;

PyObject *New_MS1553Msg(PyObject *parent);
void add_1553_class(PyObject *module);

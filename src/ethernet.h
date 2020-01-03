
#include "packet.h"
#include "libirig106/src/i106_decode_ethernet.h"


typedef struct EthernetF0_Msg {
    PyObject_HEAD
    Packet *packet;
    EthernetF0_Message msg;
    int cur_byte;
} EthernetMsg;

PyObject *New_EthernetF0Msg(PyObject *parent);
void add_ethernet_class(PyObject *module);

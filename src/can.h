
#include "packet.h"
#include "libirig106/src/i106_decode_can.h"


typedef struct CANF0_Msg {
    PyObject_HEAD
    Packet *packet;
    CAN_Message msg;
    int cur_byte;
} CANMsg;

PyObject *New_CANMsg(PyObject *parent);
void add_can_class(PyObject *module);


#include "packet.h"
#include "libirig106/src/i106_decode_arinc429.h"


typedef struct Arinc429F0_Msg {
    PyObject_HEAD
    Packet *packet;
    Arinc429F0_Message msg;
    int cur_byte;
} Arinc429Msg;

PyObject *New_Arinc429F0Msg(PyObject *parent);
void add_arinc_class(PyObject *module);

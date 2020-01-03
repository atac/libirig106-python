
#include "packet.h"
#include "libirig106/src/i106_decode_discrete.h"


typedef struct DiscreteF1_Msg {
    PyObject_HEAD
    Packet *packet;
    DiscreteF1_Message msg;
    int cur_index; // current discrete (used for iteration) 
} DiscreteMsg;

PyObject *New_DiscreteF1Msg(PyObject *parent);
void add_discrete_class(PyObject *module);

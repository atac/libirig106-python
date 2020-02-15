
#include "packet.h"
#include "libirig106/src/i106_decode_video.h"


typedef struct VideoMsg {
    PyObject_HEAD
    Packet *packet;
    VideoF0_Message msg;
} VideoMsg;

PyObject *New_VideoF0Message(PyObject *parent);
void add_videof0_class(PyObject *module);


#include "packet.h"
#include "libirig106/src/i106_decode_video.h"


typedef struct VideoF0Msg {
    PyObject_HEAD
    Packet *packet;
    VideoF0_Message msg;
} VideoF0Msg;

PyObject *New_VideoF0Message(PyObject *parent);
void add_videof0_class(PyObject *module);

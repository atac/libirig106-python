
#include "packet.h"
#include "libirig106/src/i106_decode_video.h"


typedef struct VideoMsg {
    PyObject_HEAD
    Packet *packet;
    VideoF0_Message msg;
} VideoMsg;

PyObject *New_VideoMessage(PyObject *parent);
void add_video_class(PyObject *module);

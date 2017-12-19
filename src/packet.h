
typedef struct Packet {
    PyObject_HEAD
    C10 *parent;

} Packet;

static PyTypeObject Packet_Type;

void add_packet_class(PyObject *module);



typedef struct Packet {
    PyObject_HEAD
    C10 *parent;
    off_t offset;

    // Copied from I106C10Header
    uint16_t  SyncPattern;
    uint16_t  ChannelID;
    uint32_t  PacketLength;
    uint32_t  DataLength;
    uint8_t   HeaderVersion;
    uint8_t   SequenceNumber;
    uint8_t   PacketFlags;
    uint8_t   DataType;
    uint8_t   RTC[6];
    uint16_t  Checksum;

    // Secondary header (optional)
    uint32_t  Time[2];
    uint16_t  Reserved;
    uint16_t  SecondaryChecksum;
} Packet;

static PyObject *Packet_test(PyObject *self);
static PyTypeObject Packet_Type;

PyObject *New_Packet(PyObject *c10);
void add_packet_class(PyObject *module);

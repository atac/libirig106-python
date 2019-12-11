
#ifndef packet_header
#define packet_header

#include "libirig106/src/i106_decode_time.h"
#include "libirig106/src/i106_decode_1553f1.h"

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

    // Packet body
    void *body;

    // Time
    I106Time *I106Time;

    // 1553
    MS1553F1_Message  *MS1553_MSG;
} Packet;

static PyObject *Packet_test(PyObject *self);
static PyTypeObject Packet_Type;

PyObject *New_Packet(PyObject *c10);
void add_packet_class(PyObject *module);

#endif

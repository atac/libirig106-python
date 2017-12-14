
'''
irig106 - This python package provides an interface to the IRIG 106 c library.

Message data structures are based on the ctypes module.  The main implication
of this is that to use message data, first the data buffer needs to be cast to
the appropriate data structure.

Example 1

    # Import the irig106 package
    import irig106

    # Create IRIG IO object
    io = irig106.Packet.IO()

    # Open data file for reading
    io.open('data.ch10', io.FileMode.READ)

    # Read IRIG headers
    for packet_header in io.packet_headers():
        print "Ch ID %3i  %s" % (
            io.Header.ChID,
            irig106.Packet.DataType.TypeName(PktHdr.DataType))

    irig106.close()


Basic packet I/O is handled in the "Packet" module. Return status values are
defined in the "Status" module. Both of these modules are loaded automatically
when the Py106 package is loaded.  Other modules must be loaded by user code if
they are to be used.  For example, 1553 decoding is supported by including the
line

    import irig106.decode_1553

irig106.packet - Basic packet reading and writing

irig106.status - Return status values for all modules

irig106.decode_1553 - Extract 1553 messages from a 1553 packet
'''

try:
    import packet
    import status
    import irig_time

except ImportError:
    print "Py106 init error - Error importing modules"

from i106 import C10, Packet

SAMPLE_FILE = 'src/libirig106/tests/copy.c10'


def test_c10():
    c = C10(SAMPLE_FILE)
    assert c.handle > -1
    assert c.filename == SAMPLE_FILE
    del c


def test_packet():
    c = C10(SAMPLE_FILE)
    p = Packet(c)
    assert p.parent == c

    for packet in c:
        assert packet.sync_pattern == 0xeb25

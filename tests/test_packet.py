from i106 import C10, Packet

from common import SAMPLE_FILE


def test_packet():
    c = C10(SAMPLE_FILE)
    p = Packet(c)
    assert p.parent == c

    for packet in c:
        assert packet.sync_pattern == 0xeb25

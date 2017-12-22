from i106 import C10, Packet
import pytest

from common import SAMPLE_FILE


class TestPacket:

    @pytest.fixture
    def c(self):
        return C10(SAMPLE_FILE)

    @pytest.fixture
    def p(self, c):
        return Packet(c)

    def test_packet(self, p, c):
        assert p.sync_pattern == 0xeb25

    def test_bytes(self, p):
        b = p.bytes()
        assert len(b) == p.packet_length, repr(b)

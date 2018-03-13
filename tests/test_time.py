
from datetime import timedelta

from i106 import C10, Packet
import pytest

from common import SAMPLE_FILE


class TestTime:

    @pytest.fixture
    def c(self):
        return C10(SAMPLE_FILE)

    @pytest.fixture
    def p(self, c):
        for packet in c:
            if packet.data_type == 0x11:
                return packet

    def test_time(self, c):
        t = None
        for packet in c:
            if packet.data_type == 0x11:
                if t:
                    assert packet.time - t == timedelta(seconds=1)
                t = packet.time

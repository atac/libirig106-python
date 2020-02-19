
from i106 import C10
import pytest


class TestVideoF0:

    @pytest.fixture
    def p(self):
        for packet in C10('tests/video.c10'):
            if packet.data_type == 0x40:
                return packet
        raise Exception('No video packets found!')

    @pytest.fixture
    def msg(self, p):
        for msg in p:
            return msg

    # Packet level tests

    def test_iteration(self, p):
        assert len(list(p)) == len(p)

    def test_len(self, p):
        assert len(p) == 83

    @pytest.mark.parametrize('attr,expected', [
        ('type', 0),
        ('klv', 0),
        ('srs', 0),
        ('iph', 0),
        ('et', 0),
    ])
    def test_packet_attrs(self, p, attr, expected):
        assert getattr(p, attr) == expected
        # setattr(p, attr, 1)
        # assert getattr(msg, attr) == 1

    # Message level tests

    def test_bytes(self, msg):
        assert bytes(msg)


from i106 import C10
import pytest


class TestArinc:

    @pytest.fixture
    def p(self):
        for packet in C10('tests/sample.c10'):
            if packet.data_type == 0x38:
                return packet
        raise Exception('No Arinc 429 packets found!')

    @pytest.fixture
    def msg(self, p):
        for msg in p:
            return msg

    # Packet level tests

    def test_iteration(self, p):
        assert len(list(p)) == len(p)

    def test_len(self, p):
        assert len(p) == 96

    # Message level tests

    def test_bytes(self, msg):
        before = bytes(msg)
        msg.speed = 2
        assert before != bytes(msg)

    @pytest.mark.parametrize('attr,expected', [
        ('gap', 0),
        ('speed', 1),
        ('pe', 0),
        ('fe', 0),
        ('bus', 0),
        ('label', 131),
        ('sdi', 2),
        ('data', 511624),
        ('ssm', 3),
        ('parity', 1),
    ])
    def test_attrs(self, msg, attr, expected):
        assert getattr(msg, attr) == expected
        setattr(msg, attr, 1)
        assert getattr(msg, attr) == 1

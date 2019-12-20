
from i106 import C10
import pytest


class Test1553:

    @pytest.fixture
    def p(self):
        for packet in C10('tests/1553.c10'):
            if packet.data_type == 25:
                return packet

    @pytest.fixture
    def msg(self, p):
        for msg in p:
            return msg

    # Packet level
    def test_iteration(self, p):
        for i, msg in enumerate(p):
            pass
        assert i == 84

    def test_len(self, p):
        assert len(p) == 85

    def test_ttb(self, p):
        assert 1 == p.ttb

    # Message level
    def test_wordcount(self, msg):
        assert len(msg) == 26

    def test_word_iteration(self, msg):
        for word in msg:
            assert word == 8
            return

    def test_getitem(self, msg):
        assert msg[2] == 65535

    def test_getslice(self, msg):
        assert msg[:3] == (8, 8903, 65535)

    def test_bytes(self, msg):
        first = bytes(msg)
        if msg.bus:
            msg.bus = 0
        else:
            msg.bus = 1
        assert first != bytes(msg)

    @pytest.mark.parametrize('attr,expected', [
        ('we', 0),
        ('se', 0),
        ('le', 0),
        ('timeout', 0),
        ('fe', 0),
        ('rt2rt', 0),
        ('me', 0),
        ('bus', 0),
        ('gap_time', 65),
        ('length', 56),
    ])
    def test_attrs(self, msg, attr, expected):
        assert getattr(msg, attr) == expected
        setattr(msg, attr, 1)
        assert getattr(msg, attr) == 1

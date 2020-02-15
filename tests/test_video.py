
from i106 import C10
import pytest


class TestVideoF0:

    @pytest.fixture
    def p(self):
        for packet in C10('tests/video.c10'):
            if packet.data_type == 0x40:
                return packet
        raise Exception('No ethernet packets found!')

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

    # # Message level tests

    # def test_msg_len(self, msg):
    #     assert len(msg) == 67 and msg.length == 67
    #     msg.length = 1
    #     assert len(msg) == 1 and msg.length == 1

    # def test_msg_iteration(self, msg):
    #     assert len(list(msg)) == len(msg)

    # def test_getitem(self, msg):
    #     assert msg[0], msg[3] == (3, 0)

    # def test_getslice(self, msg):
    #     assert msg[:3] == (3, 0, 0)

    # def test_bytes(self, msg):
    #     before = bytes(msg)
    #     msg.netid = 1
    #     assert before != bytes(msg)

    # @pytest.mark.parametrize('attr,expected', [
    #     ('fce', 0),
    #     ('fe', 0),
    #     ('le', 0),
    #     ('netid', 0),
    #     ('content', 0),
    #     ('dce', 0),
    # ])
    # def test_attrs(self, msg, attr, expected):
    #     assert getattr(msg, attr) == expected
    #     setattr(msg, attr, 1)
    #     assert getattr(msg, attr) == 1

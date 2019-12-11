
from i106 import C10
import pytest


class TestEthernet:

    @pytest.fixture
    def p(self):
        for packet in C10('tests/ethernet.c10'):
            if packet.data_type == 0x68:
                return packet
        raise Exception('No ethernet packets found!')

    @pytest.fixture
    def msg(self, p):
        for msg in p:
            return msg

    def test_iteration(self, p):
        assert len(list(p)) == len(p)

    # def test_len(self, p):
    #     assert len(p) == 85

    # def test_ttb(self, p):
    #     assert 1 == p.ttb

    # def test_wordcount(self, msg):
    #     assert len(msg) == 26

    # def test_word_iteration(self, msg):
    #     for word in msg:
    #         assert word == 8
    #         return

    # def test_getitem(self, msg):
    #     assert msg[2] == 65535

    # def test_getslice(self, msg):
    #     assert msg[:3] == (8, 8903, 65535)

    # def test_bytes(self, msg):
    #     first = bytes(msg)
    #     if msg.bus:
    #         msg.bus = 0
    #     else:
    #         msg.bus = 1
    #     assert first != bytes(msg)

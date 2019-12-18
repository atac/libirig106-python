
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

    # Packet level tests

    def test_iteration(self, p):
        assert len(list(p)) == len(p)

    def test_len(self, p):
        assert len(p) == 1

    def test_format(self, p):
        assert 0 == p.format

    # Message level tests

    def test_msg_len(self, msg):
        assert len(msg) == 84

    def test_msg_iteration(self, msg):
        assert len(list(msg)) == len(msg)

    def test_getitem(self, msg):
        assert msg[0], msg[3] == (3, 0)

    def test_getslice(self, msg):
        assert msg[:3] == (3, 0, 0)

    def test_netid(self, msg):
        assert msg.network_id == 0
        msg.network_id = 1
        assert msg.network_id == 1

    def test_bytes(self, msg):
        first = bytes(msg)
        msg.network_id = 1
        assert first != bytes(msg)

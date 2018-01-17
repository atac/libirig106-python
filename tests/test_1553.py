
from i106 import C10
import pytest


class Test1553:

    @pytest.fixture
    def p(self):
        for packet in C10('tests/1553.c10'):
            if packet.data_type == 25:
                return packet

    def test_iteration(self, p):
        for i, msg in enumerate(p):
            pass
        assert i == 83

    def test_len(self, p):
        assert len(p) == 85

    def test_ttb(self, p):
        assert 1 == p.ttb

    def test_wordcount(self, p):
        for msg in p:
            assert len(msg) == 26
            break

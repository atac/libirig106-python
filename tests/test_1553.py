
from i106 import C10
import pytest


class Test1553:

    @pytest.fixture
    def p(self):
        for packet in C10('1553.c10'):
            if packet.data_type == 25:
                return packet

    def test_iteration(self, p):
        # print ('%s messages' % len(p))
        for i, msg in enumerate(p):
            print (i)

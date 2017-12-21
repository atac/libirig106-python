from i106 import C10

from common import SAMPLE_FILE


def test_c10():
    c = C10(SAMPLE_FILE)
    assert c.handle > -1
    assert c.filename == SAMPLE_FILE
    del c

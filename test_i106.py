#!/usr/bin/env python

"""Temporary simplistic testing script from the python end."""

from i106 import C10, Packet


def test_c10():
    c = C10('src/libirig106/tests/copy.c10')
    print (c.handle, c.filename)
    print(dir(c))
    del c


def test_packet():
    c = C10('src/libirig106/tests/copy.c10')
    p = Packet(c)
    print (dir(p))
    print (p.parent)

    for i, packet in enumerate(c):
        for attr in [a for a in dir(packet) if not a.startswith('__')]:
            print('%s: %s' % (attr, getattr(packet, attr)))
        print()

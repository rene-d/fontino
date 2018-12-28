#! /usr/bin/env python3

import sys
import argparse

"""
Convert a Bitmap Distribution Format file to a C byte array.

Ref: https://www.x.org/docs/BDF/bdf.pdf

I assume these parameters:
PIXEL_SIZE 8
FONT_ASCENT 7
FONT_DESCENT 1
UNDERLINE_POSITION -1
"""


def reverse_bits(num):
    result = 0
    while num:
        result = (result << 1) + (num & 1)
        num >>= 1
    return result


def display(BITMAP, BBX, STARTCHAR, ENCODING, show=True):
    if show:
        print("{:6} {:15} BBX={:10} BITMAP={}".format(
            ENCODING, STARTCHAR, BBX, BITMAP))

    # x and y displacement (BBox, BBoy) of the lower left corner from the origin of the character.
    w, h, ox, oy = map(int, BBX.split())

    assert h == len(BITMAP)
    assert w <= 8 and h <= 8
    assert 0 < w + ox <= 8
    assert -1 < h + oy <= 7

    # repositionne en X
    BITMAP = [b >> ox for b in BITMAP]

    # repositionne en Y
    while oy > -1:
        oy -= 1
        h += 1
        BITMAP.append(0)
    while h < 8:
        h += 1
        BITMAP.insert(0, 0)

    assert h == 8 and oy == -1

    if show:
        on = "\033[38;5;196m●\033[0m"
        off = "\033[1;30m●\033[0m"
        uborder = "\033[0;37m┏━━━━━━━━┓\033[0m"
        vborder = "\033[0;37m┃\033[0m"
        lborder = "\033[0;37m┗━━━━━━━━┛\033[0m"

        print(" 01234567")
        print(uborder)
        y = 0
        for b in BITMAP:
            s = vborder + "".join([(off, on)[b & (1 << i) != 0]
                                   for i in range(7, -1, -1)]) + vborder + str(7 - y)
            y += 1
            print(s)
        print(lborder)

    c_def = "    {" + ", ".join(f"0x{b:02x}" for b in BITMAP) + \
        f"}},  // {ENCODING:04x} ({STARTCHAR})\n"
    return c_def


def plage(s):
    """
    retourne un itérateur correspondant à la plage
        1-3,5,9-10      1,2,3,5,9,10
    """
    p = set()
    for i in s.split(","):
        j = i.split("-")
        if len(j) == 1:
            p.add(int(j[0], 0))
        elif len(j) == 2:
            p.update(range(int(j[0], 0), int(j[1], 0) + 1))
        else:
            raise "mauvaise plage: " + str
    return p


def plages(filtre):
    p = set()
    for e in filtre:
        p.update(plage(e))
    return p


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--bdf", type=str, default="ib8x8u.bdf")
    parser.add_argument("-c", "--c-struct", action="store_true")
    parser.add_argument("-o", "--output", type=str)
    parser.add_argument("filtre", nargs='*', type=str)

    args = parser.parse_args()

    STARTCHAR = None
    ENCODING = None
    BITMAP = None
    BBX = None

    c_defs = []

    for i in open(args.bdf):
        i = i.strip()
        if i.startswith("STARTCHAR "):
            STARTCHAR = i[10:]
        elif i.startswith("ENCODING "):
            ENCODING = int(i[9:])
        elif i.startswith("BBX"):
            BBX = i[4:]
        elif i == "BITMAP":
            BITMAP = []
        elif i == "ENDCHAR":
            if len(args.filtre) == 0 or ENCODING in plages(args.filtre):
                c_def = display(BITMAP, BBX, STARTCHAR, ENCODING,
                                show=not args.c_struct and not args.output)
                c_defs.append(c_def)
            BITMAP = BBX = ENCODING = STARTCHAR = None
        elif BITMAP is not None and len(i) == 2:
            BITMAP.append(int(i, 16))

    if args.c_struct:
        sys.stdout.writelines(c_defs)

    if args.output:
        with open(args.output, "wt") as f:
            f.writelines(c_defs)


if __name__ == '__main__':
    main()

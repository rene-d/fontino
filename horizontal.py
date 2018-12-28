#! /usr/bin/env python3

import sys
import os

NB_LIGNES_CAR = 10
NB_COLONNES = 8

if len(sys.argv) == 2:
    NB_COLONNES = int(sys.argv[1])

if "COLUMNS" in os.environ:
    columns = int(os.getenv("COLUMNS"))
    if columns < NB_COLONNES * 10 + 1:
        NB_COLONNES = (columns - 1) // 10

a = sys.stdin.read().split()

for i in range(0, len(a), NB_LIGNES_CAR * NB_COLONNES):
    for j in range(NB_LIGNES_CAR):
        s = ""
        for z in range(NB_COLONNES):
            o = i + j + z * NB_LIGNES_CAR
            if o < len(a):
                s += a[o]
        print(s)

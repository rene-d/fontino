# Fonts for Arduino

## Purpose

This collection of code snippets and fonts are intended for use with a 8x8 LED matrix controlled with an Arduino.

It requires the [LedControl](https://playground.arduino.cc/Main/LedControl) library to communicate with the module.

Fonts are those of the PCs of the 80s and are grabbed from different sources (see Credits).

There are so many libraries and programs to display strings or sprites on LED matrices. This one covers my needs.


## Licenses

License for source code is the [Unlicense](http://unlicense.org) (aka. free software).

bdf font files are licensed under the [Creative Commons Attributions-ShareAlike International 4.0 license](https://creativecommons.org/licenses/by-sa/4.0/). And probably IBM, but the following authors do not mention it.


## Credits

1. [github.com/dhepper/font8x8](https://github.com/dhepper/font8x8.git)<br>
The inspiration source for `render.c`.

2. [github.com/farsil/ibmfonts](https://github.com/farsil/ibmfonts.git)<br>
A collection of monospaced, bitmap fonts in BDF format that origin from IBM
text mode and system fonts. They include extended latin, greek, cyrillic and
hebrew scripts and several extra characters, for a total of 782 glyphs each.
The fonts are in BDF 2.1 format and fully compatible with applications that
make use of either Xft or XLFD.

3. VileR from [The Ultimate Oldschool PC Font Pack](http://int10h.org/oldschool-pc-fonts/)
for his pixel-perfect reproductions of these old system fonts. I used his
PxPlus fonts as a base.

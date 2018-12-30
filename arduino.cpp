// test pour Arduino
// rene-d 2018
//

#include "arduino.hpp"

#define PROGMEM
#include "font8x8_icl8x8u.ino"
#include "font8x8_ic8x8u.ino"
#include "font8x8_ib8x8u.ino"
#include "font8x8_misc.ino"

#include "print.ino"

int main(int argc, char *argv[])
{
    LedControl lc;

    lc.simu();

    int a = argc == 1 ? 0 : atoi(argv[1]);

    switch (a)
    {
    case 0:
        lc.selectColor(1);
        printChar(lc, '0' + ROTATION_0);
        displayString(lc, "PR", 0, true, ROTATION_0);
        printChar(lc, '0' + ROTATION_90);
        displayString(lc, "PR", 0, true, ROTATION_90);
        printChar(lc, '0' + ROTATION_180);
        displayString(lc, "PR", 0, true, ROTATION_180);
        printChar(lc, '0' + ROTATION_270);
        displayString(lc, "PR", 0, true, ROTATION_270);
        break;
    case 1:
        displayString_P(lc, "\200\201 No\313l !\023", 1000, false, ROTATION_0);
        displayString_P(lc, " ROTATE 0 ", 1000, false, ROTATION_0);
        displayString_P(lc, " ROTATE 1 ", 1000, false, ROTATION_90);
        displayString_P(lc, " ROTATE 2 ", 1000, false, ROTATION_180);
        displayString_P(lc, " ROTATE 3 ", 1000, false, ROTATION_270);
        break;
    }
}

#include "LedControl.h"

const int DIN = 7;   // DIN pin of MAX7219 module
const int CLK = 6;   // CLK pin of MAX7219 module
const int CS = 5;    // CS pin of MAX7219 module
const int maxInUse = 1;

LedControl lc = LedControl(DIN, CLK, CS, maxInUse);

const char texte[] PROGMEM = "\200\201 JOYEUX NOEL! \201\200 ";

void setup()
{
    lc.shutdown(0, false);      /* The MAX72XX is in power-saving mode on startup, we have to do a wakeup call    */
    lc.setIntensity(0, 0);      /* Set the brightness */
    lc.clearDisplay(0);         /* and clear the display */
}

void loop()
{
    displayString_P(lc, texte, 1500, true, 0);
}

#define FONT8X8 font8x8_ic8x8u

const byte ROTATION_0 = 0;   // wires at the left
const byte ROTATION_90 = 1;  // wires at the top
const byte ROTATION_180 = 2; // wires at the right
const byte ROTATION_270 = 3; // wires at the bottom

const byte *getImage(byte ch)
{
    if (ch < 128)
    {
        return FONT8X8[ch];
    }
    if (ch >= 128 && ch < 160)
    {
        // miscellaneous
        ch -= 128;
        if (ch < sizeof(font8x8_misc) / 8)
        {
            return font8x8_misc[ch];
        }
        return FONT8X8[0]; // valeur par défaut
    }
    return FONT8X8[ch - 160 + 128];
}

inline byte reverseBits(byte b)
{
    byte inv = 0;
    for (int j = 0; j < 8; ++j)
    {
        inv <<= 1;
        if (b & 0x1)
            inv |= 1;
        b >>= 1;
    }
    return inv;
}

void rotateChar90(const byte *image, byte newb[8])
{
    for (int i = 0; i < 8; ++i)
    {
        newb[i] = 0;
        for (int j = 0; j < 8; ++j)
        {
            byte b = pgm_read_byte_near(image + j);
            newb[i] |= (b & (1 << i)) ? 1 << (7 - j) : 0;
        }
    }
}

void rotateChar270(const byte *image, byte newb[8])
{
    for (int i = 0; i < 8; ++i)
    {
        newb[i] = 0;
        for (int j = 0; j < 8; ++j)
        {
            byte b = pgm_read_byte_near(image + j);
            newb[i] |= (b & (1 << (7 - i))) ? 1 << j : 0;
        }
    }
}

void printChar(LedControl &lc, byte ch)
{
    const byte *image = getImage(ch);
    for (int i = 0; i < 8; i++)
    {
        byte b = pgm_read_byte_near(image + i);
        lc.setRow(0, i, b);
    }
}

void printChar(LedControl &lc, byte ch, byte rotate)
{
    const byte *image = getImage(ch);
    if (rotate == ROTATION_0)
    {
        for (int i = 0; i < 8; ++i)
        {
            byte b = pgm_read_byte_near(image + i);
            lc.setRow(0, i, b);
        }
    }
    else if (rotate == ROTATION_90)
    {
        byte newb[8];
        rotateChar90(image, newb);
        for (int i = 0; i < 8; ++i)
        {
            lc.setRow(0, i, newb[i]);
        }
    }
    else if (rotate == ROTATION_180)
    {
        for (int i = 0; i < 8; ++i)
        {
            byte b = pgm_read_byte_near(image + 7 - i);
            lc.setRow(0, i, reverseBits(b));
        }
    }
    else if (rotate == ROTATION_270)
    {
        byte newb[8];
        rotateChar270(image, newb);
        for (int i = 0; i < 8; ++i)
        {
            lc.setRow(0, i, newb[i]);
        }
    }
}

class StrBase
{
  protected:
    const char *str_;
    size_t len_;

  public:
    StrBase() = default;

  protected:
    size_t length() const
    {
        return len_;
    }
    virtual byte at(size_t) const = 0;

  private:
    void displayString0(LedControl &lc, unsigned long wait) const;
    void displayString90(LedControl &lc, unsigned long wait) const;
    void displayString180(LedControl &lc, unsigned long wait) const;
    void displayString270(LedControl &lc, unsigned long wait) const;

  public:
    void displayString(LedControl &lc, unsigned long wait, bool scroll, byte rotate = ROTATION_0) const
    {
        if (scroll)
        {
            if (rotate == ROTATION_0)
                displayString0(lc, wait);
            else if (rotate == ROTATION_90)
                displayString90(lc, wait);
            else if (rotate == ROTATION_180)
                displayString180(lc, wait);
            else if (rotate == ROTATION_270)
                displayString270(lc, wait);
        }
        else
        {
            for (size_t i = 0; i < length(); ++i)
            {
                printChar(lc, at(i), rotate);
                delay(wait);
            }
        }
    }
    void printString(LedControl &lc, unsigned long wait, byte rotate = ROTATION_0) const;
};

// string stored in data segment
class StrD : public StrBase
{
  public:
    explicit StrD(const char *str)
    {
        str_ = str;
        len_ = strlen(str);
    }

    virtual byte at(size_t i) const override
    {
        if (i < len_)
            return str_[i];
        return 0;
    }
};

// string with PROGMEM attribute (stored in code segment)
class StrP : public StrBase
{
  public:
    explicit StrP(const char *str)
    {
        str_ = str;
        len_ = strlen_P(str);
    }

    virtual byte at(size_t i) const override
    {
        if (i < len_)
            return pgm_read_byte_near(str_ + i);
        return 0;
    }
};

void StrBase::displayString0(LedControl &lc, unsigned long wait) const
{
    wait /= 8;
    unsigned o;
    for (o = 0; o < length() - 1; ++o)
    {
        const byte *image1 = getImage(at(o));
        const byte *image2 = getImage(at(o + 1));

        for (int offset = 0; offset < 8; ++offset)
        {
            for (int i = 0; i < 8; ++i)
            {
                byte b;

                if (offset == 0)
                {
                    b = pgm_read_byte_near(image1 + i);
                }
                else
                {
                    byte b1 = pgm_read_byte_near(image1 + i);
                    byte b2 = pgm_read_byte_near(image2 + i);
                    b = (b1 << offset) | (b2 >> (8 - offset));
                }

                lc.setRow(0, i, b);
            }
            delay(wait);
        }
    }

    // affiche le dernier caractère entièrement
    printChar(lc, at(o));
    delay(wait);
}

void StrBase::displayString90(LedControl &lc, unsigned long wait) const
{
    wait /= 8;
    unsigned o;
    for (o = 0; o < length() - 1; ++o)
    {
        const byte *image1 = getImage(at(o));
        const byte *image2 = getImage(at(o + 1));

        byte newb1[8];
        byte newb2[8];
        rotateChar90(image1, newb1);
        rotateChar90(image2, newb2);

        for (int offset = 0; offset < 8; ++offset)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (offset <= i)
                    lc.setRow(0, i, newb1[i - offset]);
                else
                    lc.setRow(0, i, newb2[i + 8 - offset]);
            }
            delay(wait);
        }
    }

    // affiche le dernier caractère entièrement
    printChar(lc, at(o), ROTATION_90);
    delay(wait);
}

void StrBase::displayString180(LedControl &lc, unsigned long wait) const
{
    wait /= 8;
    unsigned o;
    for (o = 0; o < length() - 1; ++o)
    {
        const byte *image1 = getImage(at(o));
        const byte *image2 = getImage(at(o + 1));

        for (int offset = 0; offset < 8; ++offset)
        {
            //for (int i = 7; i >= 0; --i)
            for (int i = 0; i < 8; ++i)
            {
                byte b;

                if (offset == 0)
                {
                    b = pgm_read_byte_near(image1 + 7 - i);
                }
                else
                {
                    byte b1 = pgm_read_byte_near(image1 + 7 - i);
                    byte b2 = pgm_read_byte_near(image2 + 7 - i);
                    b = (b1 << offset) | (b2 >> (8 - offset));
                }

                lc.setRow(0, i, reverseBits(b));
            }
            delay(wait);
        }
    }

    // affiche le dernier caractère entièrement
    printChar(lc, at(o), ROTATION_180);
    delay(wait);
}

void StrBase::displayString270(LedControl &lc, unsigned long wait) const
{
    wait /= 8;
    unsigned o;
    for (o = 0; o < length() - 1; ++o)
    {
        const byte *image1 = getImage(at(o));
        const byte *image2 = getImage(at(o + 1));

        byte newb1[8];
        byte newb2[8];
        rotateChar270(image1, newb1);
        rotateChar270(image2, newb2);

        for (int offset = 0; offset < 8; ++offset)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (offset - (7 - i) <= 0)
                    lc.setRow(0, i, newb1[7 + offset - (7 - i)]);
                else
                    lc.setRow(0, i, newb2[offset - (7 - i) - 1]);
            }
            delay(wait);
        }
    }

    // affiche le dernier caractère entièrement
    printChar(lc, at(o), ROTATION_270);
    delay(wait);
}

void StrBase::printString(LedControl &lc, unsigned long wait, byte rotate) const
{
    for (size_t i = 0; i < length(); ++i)
    {
        printChar(lc, at(i), rotate);
        delay(wait);
    }
}

// wrappers

// PROGMEM strings
void displayString_P(LedControl &lc, const char *str, unsigned long wait, bool scroll, byte rotate)
{
    StrP(str).displayString(lc, wait, scroll, rotate);
}

// DATA strings
void displayString(LedControl &lc, const char *str, unsigned long wait, bool scroll, byte rotate)
{
    StrD(str).displayString(lc, wait, scroll, rotate);
}

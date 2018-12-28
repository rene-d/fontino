#! /bin/bash
# rene-d 2018

convert()
{
    local name=$1

    cat <<EOF > font8x8_${name}.ino
//
// $(grep '^FACE_NAME' ${name}.bdf)
//
// 0-31,127 : Code page 437, cf. https://en.wikipedia.org/wiki/Code_page_437
// 32-126   : ASCII
// 160-255  : ISO-8859-1, cf. https://en.wikipedia.org/wiki/ISO/IEC_8859-1

const uint8_t font8x8_${name}[224][8] PROGMEM = {
    // ---------------------------  0-127 ---------------------------
$(./bdf.py -f ${name}.bdf 0-127 -c)
    // ---------------------------  160-255 ---------------------------
$(./bdf.py -f ${name}.bdf 160-255 -c)
};
EOF
}


convert_full()
{
    local name=$1

    cat <<EOF > font8x8_${name}_full.ino
//
// $(grep '^FACE_NAME' ${name}.bdf)
//

const uint8_t font8x8_${name}_full[][8] PROGMEM = {
$(./bdf.py -f ${name}.bdf -c)
};
EOF
}

convert ib8x8u
convert ic8x8u
convert icl8x8u

convert_full ib8x8u

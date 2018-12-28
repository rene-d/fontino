#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PROGMEM
#include "font8x8_icl8x8u.ino"
#include "font8x8_ic8x8u.ino"
#include "font8x8_ib8x8u.ino"
#include "font8x8_misc.ino"

#include "font8x8_ib8x8u_full.ino"

typedef uint8_t font_t[8];

void usage(const char *exec)
{
    printf("Usage: %s <char_code>\n", exec);
    printf("       <char_code> Decimal character code between 0 and 127\n");
}

void render(const font_t *bitmap)
{
    int x, y;
    int set;

    for (x = 0; x < 8; x++)
    {
        for (y = 0; y < 8; y++)
        {
            set = ((const uint8_t *)bitmap)[x] & (1 << (7 - y));
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

void render_all(const font_t *font, int count, int cols)
{
    for (int ord = 0; ord < count; ord += cols)
    {
        int max = ord + cols;
        if (max > count)
            max = count;

        char line[8][10 * cols];

        memset(line, 0, sizeof(line));

        if (ord == 0)
        {
            for (int i = ord; i < max; ++i)
            {
                printf("+--------");
            }
            printf("+\n");
        }
        printf("|");
        for (int i = ord; i < max; ++i)
        {
            //printf("  %3d   |", i);
            //printf("  0x%02X  |", i);
            if (i < 256)
                printf("0x%02X %03o|", i, i);
            else
                printf(" 0x%04X |", i);
        }
        printf("\n");
        for (int i = ord; i < max; ++i)
        {
            printf("+--------");
        }
        printf("+\n");

        for (int i = ord; i < max; ++i)
        {
            for (int y = 0; y < 8; y++)
            {
                const uint8_t *bitmap = ((const uint8_t *)(font + i));

                for (int x = 0; x < 8; ++x)
                {
                    int set = bitmap[y] & (1 << (7 - x));
                    line[y][x + (i - ord) * 9] = set ? 'X' : ' ';
                }
                line[y][8 + (i - ord) * 9] = '|';
            }
        }
        for (int y = 0; y < 8; y++)
        {
            printf("|%s\n", line[y]);
	    /*
            const char *c;
            printf("|");
            for (c = line[y]; *c; ++c)
                printf("%s", *c == 'X' ? "█" : " ");
            printf("\n");
	    */
        }
        for (int i = ord; i < max; ++i)
        {
            printf("+--------");
        }
        printf("+\n");
    }
}

#define ELSE_IF_FONT(name)                     \
    else if (strcmp(optarg, #name) == 0)       \
    {                                          \
        font = (const font_t *)font8x8_##name; \
        count = sizeof(font8x8_##name) / 8;    \
    }

#define RENDER_FONT(name)         \
    printf("\nFONT %s\n", #name); \
    render_all(font8x8_##name, sizeof(font8x8_##name) / 8, cols);

int main(int argc, char **argv)
{
    int ord;
    int c;
    const font_t *font = (const font_t *)font8x8_ic8x8u;
    int count = sizeof(font8x8_ic8x8u) / 8;
    bool all = false;
    int cols = 16;

    while ((c = getopt(argc, argv, "f:hac:")) != -1)
    {
        switch (c)
        {
        case 'h':
            usage(argv[0]);
            exit(0);
            break;
        case 'c':
            cols = atoi(optarg);
            if (cols < 1)
                cols = 1;
            else if (cols > 128)
                cols = 128;
            break;
        case 'a':
            all = true;
            break;
        case 'f':

            if (strcmp(optarg, "misc") == 0)
            {
                font = (const font_t *)font8x8_misc;
                count = sizeof(font8x8_misc) / 8;
            }
            ELSE_IF_FONT(ic8x8u)
            ELSE_IF_FONT(icl8x8u)
            ELSE_IF_FONT(ib8x8u)
            ELSE_IF_FONT(ib8x8u_full)
            else
            {
                fprintf(stderr, "unknown font %s\n", optarg);
                exit(1);
            }
            break;
        case '?':
            if (isprint(optopt))
                fprintf(stderr, "unknown/bad option -%c\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            break;
        default:
            abort();
        }
    }

    if (all)
    {
        RENDER_FONT(ic8x8u)
        RENDER_FONT(icl8x8u)
        RENDER_FONT(ib8x8u)
        RENDER_FONT(ib8x8u_full)

        printf("\nFONT %s\n", "misc");
        render_all(font8x8_misc, sizeof(font8x8_misc) / 8, cols);

        return 0;
    }

    if (optind == argc)
    {
        render_all(font, count, cols);
    }
    else
    {
        for (int index = optind; index < argc; index++)
        {
            ord = atoi(argv[index]);
            if (ord < count)
            {
                printf("%d\n", ord);
                render(font + ord);
            }
        }
    }

    return 0;
}

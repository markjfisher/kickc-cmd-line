#include <printf.h>

void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];       // stores the ASCII data
    unsigned char *pc = addr;     // cast to make the code cleaner.

	printf ("%s: (%04p)\n", desc, addr);

    for (i = 0; i < len; i++) {
        if (((word)i % 8) == 0) {
            if (i != 0) printf (" %s\n", buff);
            printf ("%04x", (word)i);
        }
        printf (" %02x", pc[i]);
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[(word)i % 8] = '.';
        else
            buff[(word)i % 8] = pc[i];
        buff[((word)i % 8) + 1] = '\0';
    }

    while (((word)i % 8) != 0) {
        printf (" ");
        i++;
    }

    printf (" %s\n", buff);
}

#pragma target(project)

#include <6502.h>
#include <printf.h>
#include "atari-mem.h"
// #include "atari-util.h"
// #include "conio-plus.h"
#include "dos.h"
#include "util.h"

int main(int argc, char** argv) {
	// read the DOS command line into our own values
	printVersion(3);
	
	// get the args
	parseArgs();
	argc = nargc;
	argv = (char **)nargv;

	printf("argc: %d\n", argc);
	for(int i=0; i < argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}

	return 0;
}

void printVersion(int version) {
	// unsigned char nBuf[10];
	printf("version: %d\n", version); // print(itoa(version, nBuf, 10)); print("\n");
}

void parseArgs() {
	char *from;
	nargc = 0;

	// buf is the area to store argv pointers in.
	*(cl_temp + CL_SIZE) = ATEOL; // initialise buffer

	// get location of start of buffer
	from = *DOSVEC + LBUF;
	hexDump("from", from, 64);
	
	// now convert it to an array of strings
	// copy DOS memory to our buffer
	int i = 0;
	bool hitEOL = false;
	while (i < CL_SIZE && !hitEOL) {
		char c = *(from + i);
		*(cl_temp + i) = c;
		if (c == ATEOL) hitEOL = true;
		i++;
	}
	*(cl_temp + i) = 0; // null terminate at the end
	hexDump("cl_temp", cl_temp, i + 1);

	i = 0;
	hitEOL = false;
	bool inArg = false;
	while (i++ < CL_SIZE && nargc < MAX_ARGS) {
		char c = *(from + i - 1);
		if (c == ATEOL) break;
		if (c == ' ') {
			if (inArg) {
				// terminate the string
				*(cl_temp + i - 1) = 0;
			}
			inArg = false;
			continue;
		};
		if (!inArg) {
			inArg = true;
			*(nargv + nargc * 2) = LOBYTE(cl_temp + i - 1);
			*(nargv + nargc * 2 + 1) = HIBYTE(cl_temp + i - 1);
			nargc++;
			printf("clx: %04p, nargc: %d\n", cl_temp + i - 1, nargc);
		}
	}
	// finally set last terminal 0
	*(cl_temp + i - 1) = 0;

	hexDump("nargv2 after", nargv, 32);
	printf("final i: %d, cl_temp: %04p\n", i, cl_temp);
	
}

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

__export int nargc = 0;
__export char nargv[MAX_ARGS * 2 + 1];
__export char cl_temp[CL_SIZE + 1];

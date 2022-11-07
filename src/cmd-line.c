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
	char *from, *end, *into;
	word *nargvp;
	nargc = 0;

	from = *DOSVEC + LBUF;
	into = cl_temp;
	end = from + CL_SIZE;
	*end = ATEOL;
	nargvp = (word *)nargv;

	hexDump("from", from, 64);
	
	// copy DOS memory to our buffer
	bool hitEOL = false;
	while (from < end && !hitEOL) {
		char c = *from++;
		*into++ = c;
		if (c == ATEOL) hitEOL = true;
	}
	*into = 0; // null terminate at the end
	hexDump("cl_temp", cl_temp, CL_SIZE);

	// redo strings putting 0 between args to turn into a list
	from = *DOSVEC + LBUF;
	into = cl_temp;
	bool inArg = false;
	while (from < end && nargc < MAX_ARGS) {
		char c = *from++;
		if (c == ATEOL) break;
		if (c == ' ') {
			if (inArg) *into++ = 0; // terminate string
			inArg = false;
			continue;
		};
		if (!inArg) {
			inArg = true;
			*nargvp++ = (word)into;
			nargc++;
			printf("clx: %04p, nargc: %d\n", into, nargc);
		}
		into++;
	}
	// finally set last terminal 0
	*into = 0;

	hexDump("nargv2 after", nargv, 32);
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

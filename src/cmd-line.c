#pragma target(project)

#include "atari-mem.h"
#include "dos.h"
#include "conio-plus.h"

int main(int argc, char** argv) {
	parseArgs();
	argc = nargc;
	argv = (char **)nargv;

	for(int i=0; i < argc; i++) {
		print("argv: "); print(argv[i]); print("\n");
	}

	return 0;
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
	
	// copy DOS memory to our buffer, skipping excess space and keeping track of each new arg's address
	bool inArg = false;
	while (from < end && nargc < MAX_ARGS) {
		char c = *from++;
		if (c == ATEOL) break;
		if (c == ' ') {
			if (inArg) *into++ = 0;
			inArg = false;
			continue;
		}
		if (!inArg) {
			inArg = true;
			*nargvp++ = (word)into;
			nargc++;
		}
		
		*into++ = c;
	}
	*into = 0; // null terminate at the end
}

__export int nargc = 0;
__export char nargv[MAX_ARGS * 2 + 1];
__export char cl_temp[CL_SIZE + 1];

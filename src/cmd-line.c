#pragma target(project)

#include <printf.h>
#include "atari-mem.h"
#include "dos.h"
// #include "conio-plus.h"

#pragma code_seg(Code)
#pragma data_seg(Data)

volatile char __dummy = 0;

int main(int argc, char** argv) {
	// purely to keep parseArgs from being optimized away. It's run during INIT of xex.
	if (__dummy == 0x96) parseArgs();
	argc = nargc;
	argv = (char **)nargv;

	printf("argc: %d\n", argc);
	for(int i=0; i < argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	return 0;
}

#pragma code_seg(CLICode)
#pragma data_seg(CLIData)
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

// nargc needs __ma to stop it getting zero'd by compiler after init block loaded
// not sure why it insists on doing that. it's code generated that does this setting to 0, not init loading clearing it
__export __ma int nargc;
__export char nargv[MAX_ARGS * 2 + 1];
__export char cl_temp[CL_SIZE + 1];

#pragma target(project)

#include <printf.h>
#include "cmdargs-atari.h"
#include "atari-mem.h"
#include "dos.h"
// #include "conio-plus.h"

#pragma code_seg(Code)
#pragma data_seg(Data)

#pragma args_parser(parseArgs, __argc, __argv)

int main(int argc, char** argv) {
	printf("cmdline args parsing example. v1\n");
	printf("   argc = (%04p) %d\n", &argc, argc);
	for(int i=0; i < argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	return 0;
}

#pragma code_seg(CLICode)
#pragma data_seg(CLIData)
void parseArgsOld() {
	char *from, *end, *into;
	word *nargvp;
	nargc = 0;

	int dos_type = NODOS;
	// detect the DOS version
	if (*DOS == 'S') {
		if (*(DOS+3) == 'B' && *(DOS+4) == 'W') {
			dos_type = BWDOS;
		} else {
			dos_type = SPARTADOS;
		}
	} else if(*DOS == 'M') {
		dos_type = MYDOS;
	} else if (*DOS == 'X') {
		dos_type = XDOS;
	} else if (*DOS == 'R') {
		dos_type = REALDOS;
	} else {
		// OS/A+ has JMP instructions at DOSVEC+COMTAB and DOSVEC+ZCRNAME and DOSVEC + 6
		char *dvp = *DOSVEC;
		if ( *(dvp + COMTAB) == 0x4c && *(dvp + ZCRNAME) == 0x4c && *(dvp + 6) == 0x4c) {
			dos_type = OSADOS;
		}
	}
	// A bit hacky, all the dos types are ordered so the ones with CMD processing are first
	// But if we're above the max, nothing to process so just return
	if (dos_type > MAX_DOS_WITH_CMDLINE ) return;
	
	// Find the memory buffer location of the command line
	if (dos_type == XDOS) {
		from = XLINE;
	} else {
		// Spartados style dos
		from = *DOSVEC + LBUF;
	}
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
__export __mem __ma int nargc = 0;
__export char nargv[MAX_ARGS * 2 + 1];
__export char cl_temp[CL_SIZE + 1];

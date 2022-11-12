#pragma target(project)

#include <printf.h>
#include "mainargs-atari.h"
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

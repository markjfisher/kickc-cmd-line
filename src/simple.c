#pragma target(project)

#include <6502.h>
#include "atari-util.h"
#include "conio-plus.h"

int main(int argc, char** argv) {
	print("Exiting...\n");
	waitFrames(10);
	return 0;
}

#include <atari-xl.h>
#include "atari-mem.h"

void waitFrames(signed word frames) {
	while(frames > 0) {
		waitFrame();
		frames--;
	}
}

inline void waitFrame() {
	asm {
			lda RTCLOK+2
		!:	cmp RTCLOK+2
			beq !-
	}
}

void rtclr() {
	*(RTCLOK + 0) = 0;
	*(RTCLOK + 1) = 0;
	*(RTCLOK + 2) = 0;
}

unsigned char kbhit() {
	if (*CH == 0xff) return 0; else return 1;
}

inline void clrkb() {
	*CH = 0xff;
}

void waitkey() {
	while(!kbhit()) ;
	clrkb();
}

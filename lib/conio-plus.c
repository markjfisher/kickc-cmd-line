/**
 * A minimal conio for E:
 * This cannot be combined with <printf.h> else you get conflicts
 */

#include <string.h>
#include "atari-mem.h"
#include "atari-cio.h"

void printl(const char* c, unsigned int l) {
	char *p = c;

	// fix \n
	while (*p != 0) {
		if (*p == '\n') *p = 0x9b;
		p++;
	}

	OS_iocb0.buffer=c;
	OS_iocb0.buflen=l;
	OS_iocb0.command=IOCB_PUTCHR;
	ciov();
}

void printc(char* c) {
	OS_iocb0.buffer=c;
	OS_iocb0.buflen=1;
	OS_iocb0.command=IOCB_PUTCHR;
	ciov();
}

void print(const char* c) {
	unsigned int l=strlen(c);
	printl(c,l);
}

void get_line(char* buf, unsigned int len) {
	OS_iocb0.buffer=buf;
	OS_iocb0.buflen=len;
	OS_iocb0.command=IOCB_GETREC;
	ciov();
}

char get_char() {
	char buf;
	OS_iocb0.buffer=&buf;
	OS_iocb0.buflen=1;
	OS_iocb0.command=IOCB_GETCHR;
	ciov();
	return buf;
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

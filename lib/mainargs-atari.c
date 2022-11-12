// routines adopted from cc65 getargs.s and dosdetect.s
// to minimise initialisation code rather than convert from C.

#pragma code_seg(CLICode)
#pragma data_seg(CLIData)

void parseArgs() {
	detect_dos();
	get_args();
}

void detect_dos() {
	kickasm(uses __dos_type) {{

.const SPARTADOS    = 0
.const REALDOS      = 1
.const BWDOS        = 2
.const OSADOS       = 3
.const XDOS         = 4
.const ATARIDOS     = 5
.const MYDOS        = 6
.const NODOS        = 255
.const MAX_DOS_WITH_CMDLINE = XDOS

.const COMTAB  = 0             // DOS entry jump vector
.const ZCRNAME = 3             // file name crunch routine jump vector

.const DOSVEC  = $0A
.const DOS     = $0700
		
		lda     DOS
		cmp     #'S'            // SpartaDOS
		beq     spdos
		cmp     #'M'            // MyDOS
		beq     mydos
		cmp     #'X'            // XDOS
		beq     xdos
		cmp     #'R'            // RealDOS
		beq     rdos

		lda     #$4C            // probably default (MJF: 6502 JMP = $4c)
		ldy     #COMTAB
		cmp     (DOSVEC),y
		bne     done
		ldy     #ZCRNAME
		cmp     (DOSVEC),y
		bne     done

		ldy     #6              // OS/A+ has a jmp here
		cmp     (DOSVEC),y
		beq     done			// MJF: is this correct? seem to be comparing to 
		lda     #OSADOS
		bne     _set

spdos:  lda     DOS+3           // 'B' in BW-DOS
		cmp     #'B'
		bne     spdos_real
		lda     DOS+4           // 'W' in BW-DOS
		cmp     #'W'
		bne     spdos_real

		lda     #BWDOS
		.byte   $2C             // BIT <abs>

spdos_real:
		lda     #SPARTADOS
		.byte   $2C             // BIT <abs>

mydos:  lda     #MYDOS
		.byte   $2C             // BIT <abs>

rdos:   lda     #REALDOS
		.byte   $2C             // BIT <abs>

xdos:   lda     #XDOS
_set:   sta     __dos_type
done:   
		// There will be an rts done for us
		// rts

	}};
	
}

void get_args() {
	char ptr1 = 0;
	kickasm(uses ptr1, uses __argc, uses __argv, uses __dos_type) {{

.const MAXARGS = 16            // max. amount of arguments in arg. table
.const CL_SIZE = 64            // command line buffer size
.const SPACE   = 32            // SPACE char.
.const ATEOL   = $9B           // EOL marker

.const DOSVEC  = $0A
.const XLINE   = $0880
.const LBUF    = 63            // command line input buffer

.const XDOS         = 4
.const MAX_DOS_WITH_CMDLINE = XDOS

initmainargs:
        lda     __dos_type      // which DOS?
        cmp     #MAX_DOS_WITH_CMDLINE + 1
        bcc     argdos
        beq     argdos
        rts

// Initialize ourcl buffer

argdos: ldy     #ATEOL
        sty     ourcl+CL_SIZE

// Move SpartaDOS/XDOS command line to our own buffer

        cmp     #XDOS
        bne     sparta

        lda     #<XLINE
        sta     ptr1
        lda     #>XLINE
        sta     ptr1+1
        bne     cpcl0

sparta: lda     DOSVEC
        clc
        adc     #<LBUF
        sta     ptr1
        lda     DOSVEC+1
        adc     #>LBUF
        sta     ptr1+1

cpcl0:  ldy     #0
cpcl:   lda     (ptr1),y
        sta     ourcl,y
        iny
        cmp     #ATEOL
        beq     movdon
        cpy     #CL_SIZE
        bne     cpcl

movdon: lda     #0
        sta     ourcl,y         // null terminate behind ATEOL

// Turn command line into argv table

        //ldy    #0
        tay
eatspc: lda     ourcl,y         // eat spaces
        cmp     #ATEOL
        beq     finargs
        cmp     #SPACE
        bne     rpar            // begin of argument found
        iny
        cpy     #CL_SIZE
        bne     eatspc
        beq     finargs         // only spaces is no argument

// Store argument vector

rpar:   lda     __argc          // low-byte
        asl
        tax                     // table index
        tya                     // ourcl index
        clc
        adc     #<ourcl
        sta     __argv,x
        lda     #>ourcl
        adc     #0
        sta     __argv+1,x
        ldx     __argc
        inx
        stx     __argc
        cpx     #MAXARGS
        beq     finargs

// Skip this arg.

skiparg:
        ldx     ourcl,y
        cpx     #ATEOL          // end of line?
        beq     eopar
        cpx     #SPACE
        beq     eopar
        iny
        cpy     #CL_SIZE
        bne     skiparg

// End of arg. -> place 0

eopar:
        lda     #0
        sta     ourcl,y
        iny                     // y behind arg.
        cpx     #ATEOL          // was it the last arg?
        bne     eatspc

// Finish args

finargs:
        lda     __argc
        asl
        tax
        lda     #0
        sta     __argv,x
        sta     __argv+1,x

	}};
}

__export __ma char __dos_type = 0xff;
__export __ma int __argc = 0;
__export char __argv[16 * 2 + 1];
__export char ourcl[64 + 1];

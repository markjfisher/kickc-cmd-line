/*
; Copied from https://github.com/cc65/cc65/blob/master/asminc/atari.inc

; Special Entry Command Equates
; DOS Commands
; Command line table, Index values for (DOSVEC),Y -- COMTAB
; Compatible with OS/A+, DOS XL and SpartaDOS
*/

#ifndef CONST_DOS_H
#define CONST_DOS_H

#define COMTAB  0       /* DOS entry jump vector                  */
#define ZCRNAME 3       /* file name crunch routine jump vector   */
#define BUFOFF  10      /* next parameter buffer offset           */
#define COMFNAM 33      /* destination buffer for crunch routine  */
#define LBUF    63      /* command line input buffer              */

#define MAX_ARGS 16     /* max number of command line args        */
#define CL_SIZE  64     /* buffer size to work with command line  */
#define ATEOL    0x9b   /* EOL char */

#endif

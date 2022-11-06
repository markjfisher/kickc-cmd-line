; Copied from https://github.com/cc65/cc65/blob/master/asminc/atari.inc

; Special Entry Command Equates
; DOS Commands
; Command line table, Index values for (DOSVEC),Y -- COMTAB
; Compatible with OS/A+, DOS XL and SpartaDOS

COMTAB  = 0             ;DOS entry jump vector
ZCRNAME = 3             ;file name crunch routine jump vector
BUFOFF  = 10            ;next parameter buffer offset
COMFNAM = 33            ;destination buffer for crunch routine
LBUF    = 63            ;command line input buffer

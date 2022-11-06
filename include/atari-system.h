// Add these to global definitions

// Turns off OS with custom PORTB($D301) value.
void systemOffB(char port_b);

// Turns off OS and BASIC. Gives maximum available RAM
void systemOff();

// Turns on OS back and resets machine
void systemResetB(char port_b);

// Calls with default arg
void systemReset();

// Enable and set custom handler for Vertical Blank Interrupt.
// To set vertical blank interrupt vector from your inline assembly code
void enableVBLI(VEC_TYPE vblptr) ;

// Disables custom routine of Vertical Blank Interrupt
void disableVBLI();

// Enables IRQ Interrupts
void enableIRQ();

// Disables IRQ Interrupts
void disableIRQ();

// Set IRQ Interrupt Vector
void setIRQ(VEC_TYPE irqptr);

// Enable and set custom handler for Display List Interrupt
void enableDLI(VEC_TYPE dliptr);

// Set custom handler for Display List Interrupt
void setDLI(VEC_TYPE dliptr);

// Disables Display List Interrupts
void disableDLI();

// Waits till drawing of current frame ends
void waitFrame();

// Waits for a specified number of frames.
// Each frame is 1/50 sec for PAL systems, or 1/60 sec for NTSC
void waitFrames(signed char frames);

// Sets font charset located at specified page of memory
void setCharset(char msb);

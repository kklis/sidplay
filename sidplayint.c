#include <string.h>

#define intctrl (*((volatile unsigned char*)0xDC0D))
#define screenctrl (*((volatile unsigned char*)0xD011))
#define rasterline (*((volatile unsigned char*)0xD012))
#define rasterintctrl (*((volatile unsigned char*)0xD01A))
#define rasterintlo (*((volatile unsigned char*)0x314))
#define rasterinthi (*((volatile unsigned char*)0x315))

// SID refresh function
void play() {
    asm("ASL $D019\n\t"  // acknowledge interrupt (reset bit 0)
        "JSR $1003\n\t"  // call SID refresh routine
        "JMP $EA31");    // jump to default interrupt handler routine
}

int main() {
    char *fname = "GINGER";
    unsigned char fnamelo = (unsigned int)fname & 0xFFFF;
    unsigned char fnamehi = (unsigned int)fname >> 8;
    unsigned char fnamelen = strlen(fname);
    void (* fun)(void) = &play;

    // Load SID file into memory
    asm("JSR $FFBD\n\t"  // SETNAM (A = fname lenght, XY = fname address)
        "LDA #$01\n\t"   // logical file number
        "LDX #$08\n\t"   // device number
        "LDY #$01\n\t"   // load to address found in file header
        "JSR $FFBA\n\t"  // SETLFS
        "LDA #$00\n\t"   // load to memory
        "JSR $FFD5\n\t"  // LOAD
        :: "a"(fnamelen), "x"(fnamelo), "y"(fnamehi));

    // Init SID player routine
    asm("LDA #$00\n\t"
        "TAX\n\t"
        "TAY\n\t"
        "JSR $1000");

    // Set up raster interrupt
    asm("SEI");         // switch off interrupts
    intctrl = 0x7F;     // disable CIA interrupts
    rasterintctrl = 1;  // enable raster interrupts
    rasterintlo = (unsigned int)fun & 0xFFFF; // low byte of raster interrupt routine
    rasterinthi = (unsigned int)fun >> 8;     // high byte of raster interrupt routine
    rasterline = 127;   // trigger interrupt at raster line 127
    screenctrl = screenctrl & 0x7F;  // ignore rasterlines > 255
    asm("CLI");         // switch on interrupts

    return 0;
}

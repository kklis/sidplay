#include <string.h>

void play() {
    asm ("ASL $D019\n\t"  // acknowledge interrupt (reset bit 0)
         "JSR $1003\n\t"  // call SID refresh routine
         "JMP $EA31");    // jump to default interrupt handler routine
}

int main() {
    char *fname = "GINGER";
    unsigned char fnamelo = (unsigned int)fname & 0xFFFF;
    unsigned char fnamehi = (unsigned int)fname >> 8;
    unsigned char fnamelen = strlen(fname);
    void (* fun)(void) = &play;
    unsigned char funlo = (unsigned int)fun & 0xFFFF;
    unsigned char funhi = (unsigned int)fun >> 8;

    // Load SID file into memory
    asm ("JSR $FFBD\n\t"  // SETNAM (A = fname lenght, XY = fname address)
         "LDA #$01\n\t"   // logical file number
         "LDX #$08\n\t"   // device number
         "LDY #$01\n\t"   // load to address found in file header
         "JSR $FFBA\n\t"  // SETLFS
         "LDA #$00\n\t"   // load to memory
         "JSR $FFD5\n\t"  // LOAD
         "LDA #$01\n\t"   // logical file number
         "JSR $FFC3"      // CLOSE
         :: "a"(fnamelen), "x"(fnamelo), "y"(fnamehi));
    // Init SID player routine
    asm ("LDA #$00\n\t"
         "TAX\n\t"
         "TAY\n\t"
         "JSR $1000");
    // Set up raster interrupt
    asm ("SEI\n\t"        // switch off interrupts
         "LDA #$01\n\t"   // enable raster interrupts
         "STA $D01A\n\t"
         "STX $0314\n\t"  // low byte of raster interrupt routine
         "STY $0315\n\t"  // high byte of raster interrupt routine
         "LDA #$7F\n\t"   // disable CIA interrupts
         "STA $DC0D\n\t"
         "AND $D011\n\t"  // clear bit 8 of raster comparator
         "STA $D011\n\t"
         "STA $D012\n\t"  // trigger interrupt at raster line 127
         "CLI"            // switch on interrupts
         :: "x"(funlo), "y"(funhi));
    return 0;
}

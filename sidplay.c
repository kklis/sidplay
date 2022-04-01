#include <string.h>

#define rasterline (*((volatile unsigned char*)0xD012))

int main() {
    char *fname = "GINGER";
    unsigned char fnamelo = (unsigned int)fname & 0xFFFF;
    unsigned char fnamehi = (unsigned int)fname >> 8;
    unsigned char fnamelen = strlen(fname);

    // Load SID file into memory
    asm ("JSR $FFBD\n\t"  // SETNAM (A = fname lenght, XY = fname address)
         "LDA #$01\n\t"   // logical file number
         "LDX #$08\n\t"   // device number
         "LDY #$01\n\t"   // load to address found in file header
         "JSR $FFBA\n\t"  // SETLFS
         "LDA #$00\n\t"   // load to memory
         "JSR $FFD5\n\t"  // LOAD
         :: "a"(fnamelen), "x"(fnamelo), "y"(fnamehi));
    // Init SID player routine
    asm ("LDA #$00\n\t"
         "TAX\n\t"
         "TAY\n\t"
         "JSR $1000");
    // Call SID refresh routine
    while (1) {
        if (rasterline == 255) {
            asm ("JSR $1003");
        }
    }
    return 0;
}

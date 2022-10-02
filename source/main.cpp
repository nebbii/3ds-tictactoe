#include <3ds.h>
#include <stdio.h>
#include <string>

std::string renderPlayfield(int field[3][3]) {
    std::string pf = "\n";

    for(int y=0; y<3; y++) {
        for(int x=0; x<3; x++) {
            switch(field[x][y]) {
                case 1:
                    pf = pf + "X";
                    break;
                case 2:
                    pf = pf + "O";
                    break;
                default:
                    pf = pf + "-";
            }
        }
        pf = pf + "\n";
    }

    return pf;
}

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    int pf[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    // Main loop
    while (aptMainLoop())
    {
        // Read inputs
        hidScanInput(); 
        touchPosition touch;
        hidTouchRead(&touch);

        printf("\x1b[1;1H%s", renderPlayfield(pf).c_str());

        printf("\x1b[5;1HX position: %i    ", touch.px);
        printf("\x1b[6;1HY position: %i    ", touch.py);

        // Check for buttons
        u32 kDown = hidKeysDown();

        if (kDown & KEY_DUP) pf[1][0] = 1;
        if (kDown & KEY_DLEFT) pf[0][1] = 1;
        if (kDown & KEY_DRIGHT) pf[2][1] = 1;
        if (kDown & KEY_DDOWN) pf[1][2] = 1;

        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();

        if (kDown & KEY_START) break;
    }

    gfxExit();
    return 0;
}

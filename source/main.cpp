#include <3ds.h>
#include <stdio.h>
#include <string>

std::string renderPlayfield(int field[3][3]) {
    std::string pf;

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

int *handleInput(u16 px, u16 py) {
    static int slot[] = {0, 0};
    if (px < 8) {
        slot[0] = 0;
    }
    else if (px < 16) {
        slot[0] = 1;
    }
    else if (px < 24) {
        slot[0] = 2;
    }

    if (py < 8) {
        slot[1] = 0;
    }
    else if (py < 16) {
        slot[1] = 1;
    }
    else if (py < 24) {
        slot[1] = 2;
    }

    return slot;
}

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    int pf[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    int *currentSlot = handleInput(0, 0);

    // Main loop
    while (aptMainLoop())
    {
        // Read inputs
        hidScanInput(); 
        touchPosition touch;
        hidTouchRead(&touch);
        
        // Check for buttons
        u32 kDown = hidKeysDown();

        printf("\x1b[1;1H%s", renderPlayfield(pf).c_str());

        printf("\x1b[5;1HX position: %i    ", touch.px);
        printf("\x1b[6;1HY position: %i    ", touch.py);

        int *cs = handleInput(touch.px, touch.py);

        if (kDown & KEY_TOUCH) pf[cs[0]][cs[1]] = 1;

        printf("\x1b[7;1HDetected Slot: %i, %i    ", cs[0], cs[1]);

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

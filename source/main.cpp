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
    static int slot[] = {-1, -1};

    if (px < 8) {
        slot[0] = 0;
    }
    else if (px < 16) {
        slot[0] = 1;
    }
    else if (px < 24) {
        slot[0] = 2;
    }
    else {
        return {new int[2]{-1, -1}};
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
    else {
        return {new int[2]{-1, -1}};
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

    int turn = 1;

    // Main loop
    while (aptMainLoop())
    {
        // Read inputs
        hidScanInput(); 
        touchPosition touch;
        hidTouchRead(&touch);
        
        // Check for buttons
        u32 kDown = hidKeysDown();
        int *cs = handleInput(touch.px, touch.py);

        // Handle input and set checkboxes
        if ((kDown & KEY_TOUCH) && (cs[0] != -1 && cs[1] != -1)) { 
            if (pf[cs[0]][cs[1]] == 0) {
                pf[cs[0]][cs[1]] = turn;

                // flip turn
                turn = (turn == 1 ? 2 : 1);
            }
        }

        // Print playfield
        printf("\x1b[1;1H%s", renderPlayfield(pf).c_str());

        // Print debug lines
        printf("\x1b[5;1HX position: %i    ", touch.px);
        printf("\x1b[6;1HY position: %i    ", touch.py);
        printf("\x1b[7;1HDetected Slot: %i, %i    ", cs[0], cs[1]);
        printf("\x1b[8;1HTurn: %i", turn);

        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();

        if (kDown & KEY_START) break;
    }

    gfxExit();
    return 0;
}

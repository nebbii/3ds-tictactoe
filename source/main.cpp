#include <3ds.h>
#include <stdio.h>
#include <string>

std::string renderPlayfield(int** field) {
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

int* handleTappedCell(u16 px, u16 py) {
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

int checkWinCondition(int** pf) {
    for(int i=0; i<3; i++) {
        if((pf[i][0]==1 && pf[i][1]==1 && pf[i][2]==1) ||
           (pf[0][i]==1 && pf[1][i]==1 && pf[2][i]==1) ||
           (pf[0][0]==1 && pf[1][1]==1 && pf[2][2]==1) ||
           (pf[0][2]==1 && pf[1][1]==1 && pf[2][0]==1))
        {
            return 10;
        }

        if((pf[i][0]==2 && pf[i][1]==2 && pf[i][2]==2) ||
           (pf[0][i]==2 && pf[1][i]==2 && pf[2][i]==2) ||
           (pf[0][0]==2 && pf[1][1]==2 && pf[2][2]==2) ||
           (pf[0][2]==2 && pf[1][1]==2 && pf[2][0]==2))
        {
            return 20;
        }
    }

    return 0;
}

int** clearPlayfield() {
    int** arr;
    arr = new int*[3];

    for (int h=0; h<3; h++) {
        arr[h] = new int[3];

        for(int w=0; w<3; w++) {
            arr[h][w] = 0;
        }
    }

    return arr;
}


int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    int** pf = clearPlayfield();

    int turn = 1;
    int state = 0;

    int score[] = {0, 0};

    // Main loop
    while (aptMainLoop())
    {
        // Read inputs
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        touchPosition touch;
        hidTouchRead(&touch);
        
        switch(state) {
            case 0:
                {
                    // Check for tapped cells
                    int *cs = handleTappedCell(touch.px, touch.py);

                    // On tap within playfield
                    if ((kDown & KEY_TOUCH) && (cs[0] != -1 && cs[1] != -1)) { 
                        if (pf[cs[0]][cs[1]] == 0) {
                            pf[cs[0]][cs[1]] = turn;

                            // flip turn
                            turn = (turn == 1 ? 2 : 1);

                            state = checkWinCondition(pf);
                        }
                    }

                    // blank out win screen
                    printf("\x1b[9;1H                  ");
                    break;
                }
            case 10:
                score[0]++;
                state = state/10;
                break;
            case 20:
                score[1]++;
                state = state/10;
                break;
            default:
                printf("\x1b[9;1HPlayer %i has won!", state);

                if (kDown & KEY_TOUCH) {
                    pf = clearPlayfield();
                    turn = state = 0;
                }
        }
        // Print playfield
        printf("\x1b[1;1H%s", renderPlayfield(pf).c_str());
        // Print scoreboard
        printf("\x1b[5;1HPlayer 1 score: %i", score[0]);
        printf("\x1b[6;1HPlayer 2 score: %i", score[1]);

        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();

        if (kDown & KEY_START) break;
    }

    gfxExit();
    return 0;
}

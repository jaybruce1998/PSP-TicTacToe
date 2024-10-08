#include <SDL.h>

#define SIZE 3 // Size of the Tic Tac Toe grid
#define CELL_SIZE 80 // Size of each cell
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 272

// Game state
char board[SIZE][SIZE]; // Tic Tac Toe board
char cur, turn, winner; // Current player ('X' or 'O')

// Cursor position
int cursorRow = 0; // Row position of the cursor
int cursorCol = 0; // Column position of the cursor

// Function to initialize the board
void initializeBoard() {
    cur=1;
    turn='X';
    winner=0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            board[i][j] = ' '; // Fill the board with spaces
        }
    }
}

void drawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        // Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void drawW(SDL_Renderer* renderer, int x, int y) {
    // Set color for the W (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Define the points for the W shape
    int width = 20; // Width of the W
    int height = 30; // Height of the W
    int offset = 5; // Offset for the peaks of the W

    // Draw the lines to form the W
    SDL_RenderDrawLine(renderer, x, y, x + width / 4, y + height);                // Left line down
    SDL_RenderDrawLine(renderer, x + width / 4, y + height, x + width / 2, y);    // First peak
    SDL_RenderDrawLine(renderer, x + width / 2, y, x + (3 * width / 4), y + height); // Second peak
    SDL_RenderDrawLine(renderer, x + (3 * width / 4), y + height, x + width, y);    // Right line down
}

void drawN(SDL_Renderer* renderer, int x, int y) {
    // Set color for the N (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Define the points for the N shape
    int height = 30; // Height of the N
    int width = 20;  // Width of the N

    // Draw the lines to form the N
    SDL_RenderDrawLine(renderer, x, y, x, y + height);            // Left vertical line
    SDL_RenderDrawLine(renderer, x, y, x + width, y + height);   // Diagonal line
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + height); // Right vertical line
}

void drawT(SDL_Renderer* renderer, int x, int y) {
    // Set color for the T (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Define the points for the T shape
    int width = 20;  // Width of the T
    int height = 30; // Height of the T

    // Draw the horizontal line (top of the T)
    SDL_RenderDrawLine(renderer, x, y, x + width, y);               // Top horizontal line

    // Draw the vertical line (stem of the T)
    SDL_RenderDrawLine(renderer, x + width / 2, y, x + width / 2, y + height); // Vertical line
}

void drawI(SDL_Renderer* renderer, int x, int y) {
    drawT(renderer, x, y);
    SDL_RenderDrawLine(renderer, x, y+30, x + 20, y+30);
}

void drawE(SDL_Renderer* renderer, int x, int y) {
    // Define the dimensions for the letter E
    int width = 20;  // Width of the E
    int height = 30; // Height of the E

    // Draw the vertical line (stem of the E)
    SDL_RenderDrawLine(renderer, x, y, x, y + height);           // Vertical line

    // Draw the three horizontal lines (top, middle, bottom of the E)
    SDL_RenderDrawLine(renderer, x, y, x + width, y);           // Top horizontal line
    SDL_RenderDrawLine(renderer, x, y + height / 2, x + width, y + height / 2); // Middle horizontal line
    SDL_RenderDrawLine(renderer, x, y + height, x + width, y + height); // Bottom horizontal line
}

void drawX(SDL_Renderer* renderer, int x, int y) {
    // Set color for the X (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Define the dimensions for the letter X
    int width = 20;   // Width of the X
    int height = 30;  // Height of the X

    // Draw the two diagonal lines that form the X
    SDL_RenderDrawLine(renderer, x, y, x + width, y + height);           // Line from top-left to bottom-right
    SDL_RenderDrawLine(renderer, x + width, y, x, y + height);           // Line from top-right to bottom-left
}

void drawBoard(SDL_Renderer *renderer) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    // Draw the grid
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < SIZE; i++) {
        // Vertical lines
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SIZE * CELL_SIZE);
        // Horizontal lines
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SIZE * CELL_SIZE, i * CELL_SIZE);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 'X') {
                // Draw X
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLine(renderer, j * CELL_SIZE + 10, i * CELL_SIZE + 10,
                                   j * CELL_SIZE + CELL_SIZE - 10, i * CELL_SIZE + CELL_SIZE - 10);
                SDL_RenderDrawLine(renderer, j * CELL_SIZE + CELL_SIZE - 10, i * CELL_SIZE + 10,
                                   j * CELL_SIZE + 10, i * CELL_SIZE + CELL_SIZE - 10);
            } else if (board[i][j] == 'O') {
                // Draw O
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                drawCircle(renderer, j * CELL_SIZE + CELL_SIZE / 2, i * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 2 - 10);
            }
        }
    }

    // Draw the cursor
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Set color to green
    SDL_Rect cursorRect = { cursorCol * CELL_SIZE + 10, cursorRow * CELL_SIZE + 10, CELL_SIZE - 20, CELL_SIZE - 20 };
    SDL_RenderDrawRect(renderer, &cursorRect); // Draw non-filled rectangle
    if(winner)
        if(winner=='T') {
            drawT(renderer, 250, 100);
            drawI(renderer, 275, 100);
            drawE(renderer, 300, 100);
        }
        else {
            if(winner=='X')
                drawX(renderer, 250, 100);
            else
                drawCircle(renderer, 270, 115, 15);
            drawW(renderer, 300, 100);
            drawCircle(renderer, 340, 115, 15);
            drawN(renderer, 360, 100);
        }
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

    SDL_Window *window = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    initializeBoard();

    int running = 1;
    SDL_Event event;

    while (running) {
        // Process input
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // End the loop if the program is being closed
                    running = 0;
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    // Connect a controller when it is connected
                    SDL_GameControllerOpen(event.cdevice.which);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START && winner) {
                        initializeBoard();
                    }
                    if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A && !winner && board[cursorRow][cursorCol] == ' ') {
                        board[cursorRow][cursorCol]=turn;
                        if((board[0][cursorCol]==turn&&board[1][cursorCol]==turn&&board[2][cursorCol]==turn)||(board[cursorRow][0]==turn&&board[cursorRow][1]==turn&&board[cursorRow][2]==turn)
                            ||(board[1][1]==turn&&((board[0][0]==turn&&board[2][2]==turn)||(board[0][2]==turn&&board[2][0]==turn))))
                            winner=turn;
                        else if(cur++==9)
                            winner='T';
                        else
                            turn=turn=='X'?'O':'X';
                    }
                    if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                        cursorRow=cursorRow==0?2:cursorRow-1;
                    }
                    else if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                        cursorRow=cursorRow==2?0:cursorRow+1;
                    }
                    if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
                        cursorCol=cursorCol==0?2:cursorCol-1;
                    }
                    else if(event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
                        cursorCol=cursorCol==2?0:cursorCol+1;
                    }
                    break;
            }
        }

        drawBoard(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

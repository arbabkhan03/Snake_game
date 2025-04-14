// Included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h> // Wont work on github

 
// Defining constants
#define row 6
#define col 24
#define START_SIZE 10

// Speed settings
#define DEFAULT_SPEED 200 // milliseconds
#define MAX_SPEED 100 // milliseconds

// Key binds
#define UP 'w' // Key to start the upwards animation
#define LEFT 'a' // Key to start the leftwards animation
#define DOWN 's' // Key to start the downwards animation
#define RIGHT 'd' // Key to start the rightwards animation
#define PAUSE ' ' // Key to pause and unpause the animation
#define QUIT 'q' // Key to quit the program
 
// Global variables
int game_map[row][col]; // The array is defined as a global variable to permit the functions to eddit it directly
int (*snake_body)[2]; // The array that stores the snakes body as a series of cordinates
int body_len = START_SIZE - 1; // Length of the snake at the start and will store that attribute for later use minus the head
int food_pos[2]; // Bufferss that hold the food position
bool game_run = true; // Makes the game run and is the end condition for the main loop
int cmd[2] = {PAUSE, RIGHT}; // Records user input (Only changes from proper input)
int pauseSymbol[16][2]; // The position of the pause symbol components that is based on the map dimetions
int current_speed = DEFAULT_SPEED; // this should be outside of your functions if global
bool speed_toggle = false; // false = normal, true = fast

// Embedded function decleration
void menu(); // Function that prints and handles the menu logic
void initGame(); // Initializes the game
    void initArray(); // Function that populates the array with it's placeholder chars
    void statusSetup(); // Calculates the pause symbols position and save it
void initGameLogic(); // Initializes the games background logic
    void move_snake(int* new_head_pos); // Code for body movement (including head)
        int* move_snake_head(int direction); // Code for head movement
    bool checkCollision(); // Checks if the snake has colided with itself
    bool checkWinCon(); // Checks if you have won the game
    void foodLogic(); // The logic that places the food while ensuring it does not get placed in anything else and what happens when eaten
        bool placeFood(); // Code that places the food on the map
void initDisplay(); // Initializes the displaying of the game
    void AddGameObjects(); // Adds snake body to array for print (food will be inlucded later)
    void printArray(); // Function that prints the array out in the console
    void reset_map(); // Removes all previous display elements for the snake and food (Uniplemented food may not be needed since it dosent move)

int* random_pos(); // Function that returns a random position within the array
void append_element(int* new_element, int append_pos); // Function that appends the value entered to the body array in a [row, col] format
void input(); // Records user input (Uniplemented as of yet)
void statusPrint(); // Displays the pause icon
void resetGame(); // Resets all the important variables to their default values

// Main loop
int main() {

main_menu: // Restart the the code from the begining if 'm' is pressed

    menu(); // Function that prints and handles the menu logic

restart_game: // Restart the the game with the same settings if 'r' is pressed

    initGame(); // Initializes the game

    while (game_run == true) { // Main logic loop for the game
        input();
        if (cmd[0] != PAUSE && game_run) {
            initGameLogic();
            if (game_run) {
                initDisplay();
                Sleep(current_speed);
            }
        }
    }
    // After the loop ends, print the game over message
    printf("\nPress 'q' key to exit");
    printf("\nPress 'm' to go to main menu");
    printf("\nPress 'r' to restart game");
    switch(getch()) {
        case 'm':
            resetGame();
            goto main_menu; // Goes to start of main loop
            break;
        case 'r':
            resetGame();
            goto restart_game; // Goes to after the menu function in main
            break;
        case 'q':
            return 0; // Closes the window
    }
}

// Embedded function definitions
void menu() { // Function that prints and handles the menu logic
    char choice;

    while (choice != 1) {
        system("cls"); // Clears the console screen (Windows-specific)
        printf("=====================================\n");
        printf("         WELCOME TO SNAKE GAME       \n");
        printf("=====================================\n");
        printf("1. Start Game\n");
        printf("2. Instructions\n");
        printf("3. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice (1/2/3): ");

        choice = getch();

        switch (choice) {
            case '1':
                system("cls");
                return; // Start game (just returns to main)
            case '2':
                system("cls");
                printf("============= INSTRUCTIONS =============\n");
                printf("Use WASD keys to control the snake:\n");
                printf("  W - Up\n");
                printf("  A - Left\n");
                printf("  S - Down\n");
                printf("  D - Right\n\n");
                printf("Eat '*' to grow. Don't hit yourself!\n");
                printf("Press 'q' to quit the game.\n");
                printf("Press 'e' to return to menu...\n");
                printf("Press spacebar for pause\n");
                printf("Press 'f' adjust your speed\n\n");
                switch(speed_toggle) { // Prints correct option
                    case true:
                        printf("\rCurrent Speed setting: [Max Speed]    ");
                        break;
                    case false:
                        printf("\rCurrent Speed setting: [Default Speed]");
                        break;
                }

                int key; // Records input

                while (key != 'e') { // While not exiting
                    if (_kbhit) {
                        key = getch();
                        if (key == 'f') { // Key is f to cause toggle
                            if (!speed_toggle) {
                                printf("\rCurrent Speed setting: [Max Speed]    ");
                                speed_toggle = true;
                                current_speed = MAX_SPEED;
                            } else {
                                printf("\rCurrent Speed setting: [Default Speed]");
                                speed_toggle = false;
                                current_speed = DEFAULT_SPEED;
                            }
                        }
                    }
                }

                key = 0; // Resets the key otherwise it dosent work when you try to go back into menu 2
                break;
            case '3':
                printf("\nExiting game. Goodbye!");
                Sleep(1000);
                exit(0);
            default:
                printf("\nInvalid choice. Try again.");
                Sleep(250);
                break;
        }
    }
}

void initGame() { // Initializes the game
    srand((unsigned int)time(NULL) + rand()); // Random number generator initilisation (Seed init)
    initArray();
    statusSetup();
    append_element(random_pos(), 0); // Sets the head position
    placeFood();
    initDisplay();
}
 
void initArray() { // Function that populates the array with it's placeholder chars
    for (int i = 0; i < row; i++) { // Loops through each row
        for (int j = 0; j < col; j++) { // Loops through each column
            game_map[i][j] = ' ';
        }
    }
}

void statusSetup() { // Calculates the pause symbols position and save it
    int center[2] = {(row / 2) - 1, (col / 2) - 1};
    int count = 0;
    for (int i = -1; i < 3; i++) {
        for (int j = -2; j < 3; j++) {
            if (j != 0) {
                pauseSymbol[count][0] = center[0] + i;
                pauseSymbol[count][1] = center[1] + j;
                count++;
            }
        }
    }
}

void initGameLogic() {
    move_snake(move_snake_head(cmd[0])); // Performs snake movement
    if (checkCollision() || checkWinCon()) { // Checks if a end contition is met
        game_run = false; // Ends the game
    }
    foodLogic(); // Logic code related to the food
}

void move_snake(int* new_head_pos) { // Code for body movement (including head) Body will follow the head

    // e.g. [0, 2], [0, 1], [0, 0] => [0, 3], [0, 2], [0, 1] (With a vertical movement head is the first space)
 
    for (int i = body_len - 1; i > -1; i--) { // Loops through the bidy array
        switch(i) { // To differentiate the head from the rest of the body
            case 0:
                snake_body[0][0] = new_head_pos[0]; // Sets the new head row position to body segment 0
                snake_body[0][1] = new_head_pos[1]; // Sets the new head col position to body segment 0
                break;
            default:
                snake_body[i][0] = snake_body[i - 1][0]; // Sets the row value of the later segment to the previous position
                snake_body[i][1] = snake_body[i - 1][1]; // Sets the col value of the later segment to the previous position
                // May require a error hadeling to the AddGameObject function relating to empty positions if there are more than 1 body segments to fill
                // e.g. if we have the snake start with more than 1 segment
                break;
        }
    }
}

int* move_snake_head(int direction) {
    // Allocate memory to store the new head position as [row, col]
    int* new_head_pos = (int*)malloc(2 * sizeof(int));
 
    if (new_head_pos == NULL) {
        printf("Memory allocation failed!\n");
        exit(1); // Exit the program if memory allocation fails
    }
    // Update the position based on the direction input
    switch(direction) {
        case UP:
            // Move one row up, wrap around if it goes out of bounds
            new_head_pos[0] = ((snake_body[0][0] - 1) + row) % row;
            new_head_pos[1] = snake_body[0][1];
            break;
        case DOWN:
            // Move one row down, wrap around if it goes out of bounds
            new_head_pos[0] = (snake_body[0][0] + 1) % row;
            new_head_pos[1] = snake_body[0][1];
            break;
        case LEFT:
            // Move one column left, wrap around if it goes out of bounds
            new_head_pos[0] = snake_body[0][0];
            new_head_pos[1] = ((snake_body[0][1] - 1) + col) % col;
            break;
        case RIGHT:
            // Move one column right, wrap around if it goes out of bounds
            new_head_pos[0] = snake_body[0][0];
            new_head_pos[1] = (snake_body[0][1] + 1) % col;
            break;
    }
    return new_head_pos;
}

bool checkCollision() { // Checks if the snake has colided with itself
    for (int i = 1; i < body_len; i++) {
        if (snake_body[0][0] == snake_body[i][0] && snake_body[0][1] == snake_body[i][1]) {

            printf("\nGame Over! Final Score: %d\n", body_len - 1); // Adjust the final score or message as needed
            printf("===== GAME OVER! You collided with yourself! =====\n");
            printf("Snake length: %d\n", body_len);

            return true; // Head has collided with body
        }
    }
    return false;
}

bool checkWinCon() { // Checks if you have won the game
    if (body_len == row * col) { // Snake covers the whole map

        printf("\nYou Win! Final Score: %d\n", body_len - 1); // Adjust the final score or message as needed
        printf("===== YOU WIN! You became the biggest snake! =====\n");
        printf("Snake length: %d\n", body_len);

        return true; // You won the game
    }
    return false; // You have not won yes
}

void foodLogic() { // The logic that places the food while ensuring it does not get placed in anything else and what happens when eaten
    // Check if snake head is at the same position as food
    if (snake_body[0][0] == food_pos[0] && snake_body[0][1] == food_pos[1]) {
        // Grow the snake (duplicate tail segment)
        int* new_segment = (int*)malloc(2 * sizeof(int));
        new_segment[0] = snake_body[body_len - 1][0];
        new_segment[1] = snake_body[body_len - 1][1];
        append_element(new_segment, body_len); // Add to end of snake
        free(new_segment);
        
        placeFood(); // Places a new food item

    }
}

bool placeFood() { // Code that places the food on the map
    bool valid = false;
    while (!valid) {
        int* rand_food = random_pos();
        valid = true;

        for (int i = 0; i < body_len; i++) {
            if (snake_body[i][0] == rand_food[0] && snake_body[i][1] == rand_food[1]) {
                valid = false;
                break;
            }
        }

        if (valid) {
            food_pos[0] = rand_food[0];
            food_pos[1] = rand_food[1];
        }

        free(rand_food);
    }
    return valid;
}

void initDisplay() {
    AddGameObjects();
    system("cls");
    printArray();
    reset_map();
}

void AddGameObjects() { // Adds snake body and food to array for print

game_map[food_pos[0]][food_pos[1]] = '*'; // The food
    for (int i = body_len - 1; i > -1; i--) {
        switch (i) {
            case 0: // The head must be distict
                game_map[snake_body[i][0]][snake_body[i][1]] = '@';
                break;
            default: // The body of the snake
                game_map[snake_body[i][0]][snake_body[i][1]] = '#';
                break;
        }
    }
}

void printArray() { // Function that prints the array out in the console
    for (int i = 0; i < row + 2; i++) { // Loops through the rows

        char lineBuffer[col + 1]; // It is col + 3 to allow for the borders

        if (i == 0 || i == row + 1) { // Only head and foot
            for (int j = 0; j < col; j++) { // Loops through the columns
                lineBuffer[j] = '|'; // Adds data to buffer
            }
            printf("/%s/\n", lineBuffer); // Prints head and foot line
        } else {
            for (int j = 0; j < col; j++) { // Loops through the columns
                lineBuffer[j] = game_map[i - 1][j]; // Takes data from the game map
            }
            printf("|%s|\n", lineBuffer); // Prints body line
        }
    }
}

void reset_map() { // Removes all previous display elements for the snake and food
    for (int i = 0; i < body_len; i++)
    {
        game_map[snake_body[i][0]][snake_body[i][1]] = ' ';
    }
    game_map[food_pos[0]][food_pos[1]] = ' ';
}

int* random_pos() { // Function that returns a random position within the array
    int* pos = (int*)malloc(2 * sizeof(int)); // Allocates the memory used to store the [row, col] as 2 ints
    if (pos == NULL) {
        printf("Memory allocation failed!\n");
        exit(1); // Exit the program if allocation fails
    }
    pos[0] = rand() % (row); // Sets the vertical position
    pos[1] = rand() % (col); // Sets the horizontal position
    return pos; // Formatted as [row, col]
}

void append_element(int* append_element, int append_pos) { // Function that appends the value entered to the body array in a [row, col] format
    snake_body = realloc(snake_body, ++body_len * sizeof(*snake_body)); // Reallocates the memory to store the position of each body segment
    if (snake_body == NULL) {
        printf("Memory reallocation failed!\n");
        exit(1); // Exit the program if allocation fails
    }
 
    snake_body[append_pos][0] = append_element[0]; // Sets the value of the first element
    snake_body[append_pos][1] = append_element[1]; // Sets the value of the second element
}
 
void input() { // Records user input (Uniplemented as of yet)
    if (_kbhit()) { // User input loop
        int pressedKey = getch(); // Fetches the pressed keys' value
 
        if (pressedKey == UP || pressedKey == LEFT || pressedKey == DOWN || pressedKey == RIGHT || pressedKey == PAUSE || pressedKey == QUIT) { // Checks if the entered key is assigned to a command
            switch(pressedKey) {
                case QUIT:
                    printf("\nYou quit?! Final Score: %d\n", body_len - 1); // Adjust the final score or message as needed
                    printf("===== YOU QUIT! Why?! you were so close! =====\n");
                    printf("Snake length: %d\n", body_len);
                    game_run = false;
                    return;
                case PAUSE:
                    if (cmd[0] != PAUSE) { // Checks if the animation status is not paused
                        cmd[1] = cmd[0]; // The old command is replaced with the current command * Note prevCmd[1] can not be ' ' *
                        cmd[0] = pressedKey; // Stores the new commands value into the buffer
                        statusPrint(); // Displays the pause icon
                    } else { // If it is paused already
                        cmd[0] = cmd[1]; // Stores the new commands value into the buffer
                    }
                    break;
                default:
                    if (body_len > 1) { // If the body is not just the head
                        switch(pressedKey) { // Ensures the snake dosent go back over itself
                            case UP:
                                if (cmd[0] == DOWN) {
                                    return;
                                }
                                break;
                            case LEFT:
                                if (cmd[0] == RIGHT) {
                                    return;
                                }
                                break;
                            case DOWN:
                                if (cmd[0] == UP) {
                                    return;
                                }
                                break;
                            case RIGHT:
                                if (cmd[0] == LEFT) {
                                    return;
                                }
                                break;
                        }
                    }
                    cmd[1] = cmd[0]; // The old command is replaced with the current command * Note prevCmd[1] can not be ' '
                    cmd[0] = pressedKey; // Stores the new commands value into the buffer
                    break;
            }
        }
    }
}

void statusPrint() { // Displays the pause icon
    AddGameObjects();
    for (int i = 0; i < 16; i++) {
        game_map[pauseSymbol[i][0]][pauseSymbol[i][1]] = '+';
    }
    system("cls");
    printArray();
    for (int i = 0; i < 16; i++) {
        game_map[pauseSymbol[i][0]][pauseSymbol[i][1]] = ' ';
    }
    reset_map();
}

void resetGame() { // Resets all the important variables to their default values
    body_len = START_SIZE - 1;
    cmd[0] = PAUSE;
    cmd[1] = RIGHT;
    game_run = true;
}

// 88888  888  8   8  888  88888
// 8   8 8   8 8   8 8     8    
// 88888 88888 8   8  888  8888 
// 8     8   8 8   8     8 8    
// 8     8   8  888   888  88888
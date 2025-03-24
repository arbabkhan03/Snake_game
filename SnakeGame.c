// Included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h> // Wont work on github

// Defining constants
#define row 6
#define col 24
#define START_SIZE 1

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
int food_pos[2]; // Buffer that hold the food position
bool game_run = true; // Makes the game run and is the end condition for the main loop
int cmd; // Records user input (Only changes from proper input)
// Pause is unimplemented and may never be depends if you want it

// Embedded function decleration
void initGame(); // Initializes the game
    void initArray(); // Function that populates the array with it's placeholder chars
    int* random_pos(); // Function that returns a random position within the array
void initGameLogic(); // Initializes the games background logic
    int move_snake_head(int direction); // Code for head movement
    void move_snake(int* new_head_pos); // Code for body movement (including head)
    void foodLogic(); // The logic that places the food while ensuring it does not get placed in anything else and what happens when eaten
void AddGameObjects(); // Adds snake body to array for print (food will be inlucded later)
void printArray(); // Function that prints the array out in the console
void reset_map(); // Removes all previous display elements for the snake and food (Uniplemented food may not be needed since it dosent move)
void append_element(int* new_element, int append_pos); // Function that appends the value entered to the body array in a [row, col] format
void input(); // Records user input (Uniplemented as of yet)

// Main loop
int main() {
    srand((unsigned int)time(NULL) + rand()); // Random number generator initilisation (Seed init)
    initArray();

    snake_body = calloc(START_SIZE, sizeof(*snake_body));

    //int* rand = random_pos();
    //game_map[rand[0]][rand[1]] = '#';
    
    //printf("%i, %i\n", rand[0], rand[1]);
    //free(rand);
    //printf("%i in %d int memspaces\n", body_len, body_len * sizeof(snake_body)/sizeof(int));

    initGame();
    AddGameObjects();
    printArray();

    while (game_run == true && cmd != 'q') { // Main logic loop for the game
        if (_kbhit()) { // User input loop
            input();
        }
    }
}

// Embedded function definitions
void initGame() { // Initializes the game
    initArray();
    append_element(random_pos(), 0); // Sets the head position
}

void initArray() { // Function that populates the array with it's placeholder chars
    for (int i = 0; i < row; i++) { // Loops through each row
        for (int j = 0; j < col; j++) { // Loops through each column
            game_map[i][j] = ' ';
        }
    }
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

void initGameLogic() {
    move_snake(move_snake_head(cmd));
    foodLogic();
}

int move_snake_head(int direction) { // Code for head movement
    int* head_pos = snake_body[0];
    switch(direction) {
        case UP:

            break;
        case LEFT:

            break;
        case DOWN:

            break;
        case RIGHT:

            break;
    }
    return 0;
}

void move_snake(int* new_head_pos) { // Code for body movement (including head)

}

void foodLogic() { // The logic that places the food while ensuring it does not get placed in anything else and what happens when eaten

}

void AddGameObjects() { // Adds snake body to array for print (food will be inlucded later)
    for (int i = 0; i < body_len; i++) {
        //printf("%i: [%d, %d]\n", i, snake_body[i][0], snake_body[i][1]);
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
    for (int i = 0; i < col + 2; i++) {
        printf("/");
    }
    for (int i = 0; i < row; i++) { // Loops through each row
        printf("\n|");
        for (int j = 0; j < col; j++) { // Loops through each column
            printf("%c", game_map[i][j]); // Prints the value of the desingated row and column
            // printf("[%i, %i] ", integer_array[i][j], i, j); // Test print for debuging purposes with the i, j value in a cartesian format
        }
        printf("|"); // New line after a row is completed to print the array with its correct dimentions
    }
    printf("\n");
    for (int i = 0; i < col + 2; i++) {
        printf("/");
    }
    printf("\n");
}

void reset_map() { // Removes all previous display elements for the snake and food
    for (int i = 0; i < body_len; i++) {
        game_map[snake_body[i][0]][snake_body[i][1]] = ' ';
    }
}

void append_element(int* append_element, int append_pos) { // Function that appends the value entered to the body array in a [row, col] format
    snake_body = (int*)realloc(snake_body, ++body_len * sizeof(snake_body)); // Reallocates the memory to store the position of each body segment
    if (snake_body == NULL) {
        printf("Memory reallocation failed!\n");
        exit(1); // Exit the program if allocation fails
    }

    snake_body[append_pos][0] = append_element[0]; // Sets the value of the first element
    snake_body[append_pos][1] = append_element[1]; // Sets the value of the second element
}

void input() { // Records user input (Uniplemented as of yet)

    int pressedKey = getch(); // Fetches the pressed keys' value

    if (pressedKey == UP || pressedKey == LEFT || pressedKey == DOWN || pressedKey == RIGHT || pressedKey == PAUSE || pressedKey == QUIT) { // Checks if the entered key is assigned to a command
        cmd = pressedKey;
        printf("%c", cmd);
    }
}
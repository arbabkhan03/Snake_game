/*
Authors: Jeremie Jones, Arbab Khan, SiChao Qi
Date: 2025/04/14
Note: Long 35 min video sorry about that. The code was desinged to run the
      least amount of functions at any given time only triggering them when
      they are needed. There is a document that you can use to read through
      the transcripts of the video using Ctrl + F and there are many
      descritive comments available.
*/

// Included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // Wont work on github
#include <time.h>
#include <stdbool.h>
#include <conio.h> // Wont work on github

// Defining constants
#define row 6 // Vertical dimention of the map
#define col 24 // Horizontal dimention of the map
#define START_SIZE 1 // Intial size of the snake

// Speed settings (Smaller = Faster)
#define DEFAULT_SPEED 200 // Milliseconds
#define MAX_SPEED 100 // Milliseconds

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
int body_len = START_SIZE - 1; // Length of the snake at the start and will store that value for later use
int food_pos[2]; // Bufferss that hold the food position
bool game_run = true; // Makes the game run and is the end condition for the main loop
int cmd[2] = {PAUSE, RIGHT}; // Records user input (Only changes from proper input)
int pauseSymbol[16][2]; // Stores the coordinates of the pause symbol components calculated based on map dimetions
int current_speed = DEFAULT_SPEED; // Sets the delay between code repetitions smaller value = faster
bool speed_toggle = false; // false = DEFAULT_SPEED || true = MAX_SPEED

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
    void AddGameObjects(); // Adds snake body and food to array for print
    void printArray(); // Function that prints the array out in the console
    void reset_map(); // Removes all previous display elements for the snake and food (Uniplemented food may not be needed since it dosent move)

int* random_pos(); // Function that returns a random position within the array
void append_element(int* new_element, int append_pos); // Function that appends the value entered to the body array in a [row, col] format
void input(); // Records user input
void statusPrint(); // Displays the pause icon
void resetGame(); // Resets all the important variables to their default values

// Main loop
int main() {

main_menu: // Restart the the code from the begining if 'm' is pressed (during game end)

    menu(); // Function that prints and handles the menu logic

restart_game: // Restart the the game with the same settings if 'r' is pressed (during game end)

    initGame(); // Initializes the game

    while (game_run == true) { // Main logic loop for the game
        input(); // User input
        if (cmd[0] != PAUSE && game_run) { // Only to run if game_run is true and the game is not paused
            initGameLogic(); // Games logic
            if (game_run) { // If the game ends durring initGameLogic() it will imediatly end the game
                initDisplay(); // All the display functions and logic
                Sleep(current_speed); // Delay for code repetition smaller value = faster
            }
        }
    }

    // After the loop ends, print the game over message
    printf("\nPress 'q' key to exit");
    printf("\nPress 'm' to go to main menu");
    printf("\nPress 'r' to restart game");

    Sleep(250);

repeat:

    switch(getch()) { // Reads user input for respective commands
        case 'm':
            resetGame(); // Resets all the important variables to their default values
            goto main_menu; // Goes to start of main loop
            break;
        case 'r':
            resetGame(); // Resets all the important variables to their default values
            goto restart_game; // Goes to after the menu function in main
            break;
        case 'q': // Quits the game
            printf("\n\nExiting game. Goodbye!");
            Sleep(1000);
            return 0; // Closes the window
        default: // To ensure the code dosent do anything with an invalid input
            goto repeat;
    }
}

/*Arbab code*/ /*improvement and optimization by jeremie*/
// Embedded function definitions
void menu() 
{ // Function that prints and handles the menu logic
    char choice; //variable used to store user's input pf choice

    // Loop until user chooses to start the game ('1')
    while (choice != 1) 
    {
        system("cls"); // Clears the console screen (Windows-specific)
        printf("=====================================\n");
        printf("         WELCOME TO SNAKE GAME       \n");
        printf("=====================================\n");
        printf("1. Start Game\n"); //start the game
        printf("2. Settings & Instructions\n"); //option to view the game instructions
        printf("3. Exit\n"); //exit
        printf("=====================================\n");
        printf("Enter your choice (1/2/3): "); //Asking user input

        //Get user input without requiring Enter
        choice = getch();

        // Handle user's menu selection
        switch (choice) {
            case '1': //user chooses '1' begin game
                system("cls"); //clear screen for windows
                return; // Start game (just returns to main)

            case '2': //If user chooses '2' show instructs
            //statements to show user instructions and settings
                system("cls"); //clear screen in windows
                printf("============= INSTRUCTIONS =============\n");
                printf("\nPress 'e' to return to main menu.\n"); //key to return to the main menu
                printf("Use WASD keys to control the snake:\n"); //instructions for snake movement
                printf("  W - Up\n");
                printf("  A - Left\n");
                printf("  S - Down\n");
                printf("  D - Right\n");
                printf("Press [spacebar] to pause & unpause the game.\n");
                printf("Press 'q' to quit the game.\n\n");
                printf("To win eat '*' to grow till you cover the map. Don't hit yourself!\n"); //game objective
                printf("Press 'f' adjust your speed setting\n\n"); //button for toggling speeds

                switch(speed_toggle) // this is the speed setting which will be chosen based on toggle
                { // Prints current setting
                    case true: //is speed is toggled it will choose CONSTANT [MAX_SPEED]
                        printf("\rCurrent Speed setting: [Max Speed]    "); //Display current Speed 
                        break;
                    case false: //Speed toggled again it will revert to DEFAULT_SPEED
                        printf("\rCurrent Speed setting: [Default Speed]"); //Display current Speed
                        break;
                }

                 // Variable to record input during instructions screen
                int key; // Records input

                //User should press 'e' to return to the main menu
                while (key != 'e') // While not exiting
                 { 
                    if (_kbhit) 
                    {
                        key = getch(); //get pressed key without requiring enter
                        //if 'f' is pressed toggle speed setting
                        if (key == 'f') // Key is f to cause toggle
                        { 
                            // Prints current setting and changes the value of current speed
                            if (!speed_toggle) 
                            {
                                printf("\rCurrent Speed setting: [Max Speed]    "); //speed setting message 
                                speed_toggle = true; //Set speed to MAX
                                current_speed = MAX_SPEED; //Update the current peed value to MAX
                            }
                             else 
                            {
                                printf("\rCurrent Speed setting: [Default Speed]"); //Update current speed setting message
                                speed_toggle = false; //Set speed to Default Speed
                                current_speed = DEFAULT_SPEED; //Update's back to default speed
                            }
                        }
                    }
                }

                key = 0; // Resets the key otherwise it dosent work when you try to go back into menu 2
                break;

            case '3': //If user chooses to exit game
                printf("\n\nExiting game. Goodbye!");
                Sleep(1000); //1s pause before exits
                exit(0); //Exit the program

            default: //If the user enters an invalid menu choice
                printf("\n\nInvalid choice. Try again."); //print error message
                Sleep(250); //short delay before reprinting the menu
                break;
        }
    }
}

void initGame() { // Initializes the game
    srand((unsigned int)time(NULL) + rand()); // Random number generator initilisation (Seed init)
    initArray(); // Populates the array
    statusSetup(); // Preps the pause symbol based on map size
    append_element(random_pos(), 0); // Sets the head position
    placeFood(); // Places food on the map
    initDisplay(); // Dispalays the map
}
 
void initArray() { // Function that populates the array with it's placeholder chars
    for (int i = 0; i < row; i++) { // Loops through each row
        for (int j = 0; j < col; j++) { // Loops through each column
            game_map[i][j] = ' ';
        }
    }
}

void statusSetup() { // Calculates the pause symbols position and save it
    int center[2] = {(row / 2) - 1, (col / 2) - 1}; // Locates the center of the map
    int count = 0; // Counts the number of repetitions
    // Places the pause symbols components in there respective coordinates from the center procedurally
    for (int i = -1; i < 3; i++) {
        for (int j = -2; j < 3; j++) {
            if (j != 0) {
                // Saves the coordinates into the array
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
    } else {
        foodLogic(); // Logic code related to the food
    }
}

void move_snake(int* new_head_pos) { // Code for body movement (including head) Body will follow the head

    // e.g. [0, 2], [0, 1], [0, 0] => [0, 3], [0, 2], [0, 1] (With a vertical movement head is the first space aka. 0)
 
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
                // Currently it initiates all the undefined segments to position [0, 0]
                break;
        }
    }
}

/*Arbab and optimized and updated by Jeremie*/
int* move_snake_head(int direction) {
    // Allocate memory to store the new head position as [row, col]
    // A 2 memory element array
    int* new_head_pos = (int*)malloc(2 * sizeof(int));
 
    //To check if memeory allocation failed
    if (new_head_pos == NULL) 
    {
        printf("Memory allocation failed!\n");
        exit(1); // Exit the program if memory allocation fails
    }

    // Update the position based on the direction input
    switch(direction) 
    {
        case UP:
            // Move one row up, wrap around if it goes out of bounds
            new_head_pos[0] = ((snake_body[0][0] - 1) + row) % row;
            new_head_pos[1] = snake_body[0][1]; //column 
            break;
        case DOWN:
            // Move one row down, wrap around if it goes out of bounds
            new_head_pos[0] = (snake_body[0][0] + 1) % row;
            new_head_pos[1] = snake_body[0][1]; //column 
            break;
        case LEFT:
            // Move one column left, wrap around if it goes out of bounds
            new_head_pos[0] = snake_body[0][0]; //Row doesn't change
            new_head_pos[1] = ((snake_body[0][1] - 1) + col) % col;
            break;
        case RIGHT:
            // Move one column right, wrap around if it goes out of bounds
            new_head_pos[0] = snake_body[0][0]; //Row doesn't change 
            new_head_pos[1] = (snake_body[0][1] + 1) % col;
            break;
    }
     // Return the new position of the snake's head as a dynamically allocated array
    return new_head_pos;
}

/*Arbab code*/
bool checkCollision() 
{ // Checks if the snake has colided with itself
    
    //loop will start from 1 cause 0 is head
    for (int i = 1; i < body_len; i++) 
    {   //let's compare head position with other body part
        if (snake_body[0][0] == snake_body[i][0] && snake_body[0][1] == snake_body[i][1]) 
        {
            //if the hea's row and column colide with any part of the body
            printf("\nGame Over! Final Score: %d\n", body_len - 1); // Adjust the final score for message as needed
            printf("===== GAME OVER! You collided with yourself! =====\n");
            printf("Snake length: %d\n", body_len);

            return true; // Head has collided with body
        }
    }
    return false; // No collision
}

bool checkWinCon() 
{ // Checks if you have won the game
    if (body_len == row * col) { // Snake covers the whole map

        for (int i = 0; i < row; i++) { // Loops through each row
            for (int j = 0; j < col; j++) { // Loops through each column
                game_map[i][j] = '#'; // Covers the map with the body
            }
        }
        game_map[snake_body[0][0]][snake_body[0][1]] = '@'; // Adds the head
        system("cls");
        printArray(); // Prints the array

        printf("\nYou Win! Final Score: %d\n", body_len - 1); // Adjust the final score for message as needed
        printf("===== YOU WIN! You became the biggest snake! =====\n");
        printf("Snake length: %d\n", body_len);

        return true; // You won the game
    }
    return false; // You have not won yet
}

/*Arbab code*/
void foodLogic() 
{ 
    // Check if snake head is at the same position as food
    if (snake_body[0][0] == food_pos[0] && snake_body[0][1] == food_pos[1]) 
    {
        // Grow the snake (duplicate tail segment)

        // Dynamically allocate memory for a new segment (tail segment), which will become the new body part
        int* new_segment = (int*)malloc(2 * sizeof(int));

        // Set the new segment's position to the current tail (last segment) position
        // The new segment will be added at the end of the snake's body
        new_segment[0] = snake_body[body_len - 1][0];  // Row of the last segment
        new_segment[1] = snake_body[body_len - 1][1];  // Column of the last segment

        //adds a new body or segment using append element function
        append_element(new_segment, body_len); // Add to end of snake
        //free memory used by new segment
        free(new_segment);
        
        //place a new food item on the grid
        placeFood(); // Places a new food item
    }
}

/*Arbab code*/
bool placeFood() 
{ // Code that places the food on the map

    bool valid = false; //we are seeing if food positin is valid

    while (!valid) //keep trying until a valid position is made
    {
        //make random food appear in row,col as an array
        int* rand_food = random_pos();
        valid = true; //now food position is valid for the time being "initial"

        //seeing if food appears on snake body
        for (int i = 0; i < body_len; i++) 
        { // Loops through the body segments to ensure the food is not placed on the snake
            if (snake_body[i][0] == rand_food[0] && snake_body[i][1] == rand_food[1]) 
            {
                valid = false; // Food is on the snake, it is invalid
                break; //escape the loop
            }
        }
        //if a valid position is found
        if (valid) 
        { // Sets the food position to the approved random one
            food_pos[0] = rand_food[0]; //valid row postion
            food_pos[1] = rand_food[1]; //valid col position
        }
        // Free the dynamically allocated memory for rand_food as it's no longer needed
        free(rand_food);
    }
    //valid food is found return as true
    return valid; // Allowing a state change if required
}

void initDisplay() {
    AddGameObjects(); // Adds snake body and food to array for print
    system("cls"); // Clears the screen
    printArray(); // Function that prints the array out in the console
    reset_map(); // Removes all previous display elements for the snake and food (*Uniplemented food may not be needed since it dosent move)
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
    // Prints one whole row at a time to avoid "flicker" caused by delays in the computers processes
    char* lineBuffer = (char*)calloc((col + 1), sizeof(char)); // Buffer for the line (col + 1 for the terminator)
    char headFootLine[col + 1]; // Buffer for the header and footer lines (col + 1 for the terminator)

    for (int j = 0; j < col; j++) { // Loops through the columns
        lineBuffer[j] = '='; // Adds data to buffer
    }
    lineBuffer[col + 1] = '\0'; // Adds the terminator to the end

    strcpy(headFootLine, lineBuffer); // Copies the head line to its buffer
    printf("/%s\\\n", headFootLine); // Prints the head line

    for (int i = 0; i < row; i++) { // Loops through the rows
        for (int j = 0; j < col; j++) { // Loops through the columns
            lineBuffer[j] = game_map[i][j]; // Takes data from the game map array
        }
        lineBuffer[col + 1] = '\0'; // Adds the terminator to the end
        printf("|%s|\n", lineBuffer); // Prints body line
    }
    printf("\\%s/\n", headFootLine); // Prints the foot line
}

void reset_map() { // Removes all previous display elements for the snake and food
    for (int i = 0; i < body_len; i++) // Loops through the body of the snake to clear each segment
    {
        game_map[snake_body[i][0]][snake_body[i][1]] = ' ';
    }
    game_map[food_pos[0]][food_pos[1]] = ' '; // The food item is cleared here
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

void append_element(int* append_element, int append_pos) 
{ // Function that appends the value entered to the body array in a [row, col] format

    snake_body = realloc(snake_body, ++body_len * sizeof(*snake_body)); // Reallocates the memory to store the position of each body segment

    if (snake_body == NULL) {
        printf("Memory reallocation failed!\n");
        exit(1); // Exit the program if allocation fails
    }
    snake_body[append_pos][0] = append_element[0]; // Sets the value of the first element
    snake_body[append_pos][1] = append_element[1]; // Sets the value of the second element

    return;
}
 
void input() 
{ // Records user input (Uniplemented as of yet)
    if (_kbhit()) { // User input loop
        int pressedKey = getch(); // Fetches the pressed keys' value
 
        if (pressedKey == UP || pressedKey == LEFT || pressedKey == DOWN || pressedKey == RIGHT || pressedKey == PAUSE || pressedKey == QUIT) { // Checks if the entered key is assigned to a command
            switch(pressedKey) {
                case QUIT:
                    printf("\nYou quit?! Final Score: %d\n", body_len - 1); // Adjust the final score for message as needed
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
    if (row >= 4 && col >= 5) { // Only prints the symbol if the map is large enough
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
}

void resetGame() 
{ // Resets all the important variables to their default values
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
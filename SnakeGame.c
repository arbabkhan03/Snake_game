// Included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
//#include <conio.h>

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

// Embedded function decleration
void initArray();
void printArray();
int* random_pos();
void append_element(int* new_element, int append_pos);

// Main loop
int main() {
    srand((unsigned int)time(NULL)); // Random number generator initilisation (Seed init)
    initArray();

    //snake_body = (int*)malloc(2 * (body_len + 1) * sizeof(int));
    snake_body = calloc(START_SIZE, sizeof(*snake_body));

    int* rand = random_pos();
    game_map[rand[0]][rand[1]] = '#';
    //printArray();
    //printf("%i, %i\n", rand[0], rand[1]);
    free(rand);
    append_element(random_pos(), 0);
    append_element(random_pos(), 1);
    append_element(random_pos(), 2);
    append_element(random_pos(), 3);
    append_element(random_pos(), 4);
    append_element(random_pos(), 5);
    append_element(random_pos(), 6);
    append_element(random_pos(), 7);
    append_element(random_pos(), 8);
    append_element(random_pos(), 9);
    append_element(random_pos(), 10);
    append_element(random_pos(), 11);
    append_element(random_pos(), 12);
    append_element(random_pos(), 13);
    for (int i = 0; i < body_len; i++) {
        printf("%i: [%d, %d]\n", i, snake_body[i][0], snake_body[i][1]);
    }
    printf("%i in %d int memspaces\n", body_len, body_len * sizeof(snake_body)/sizeof(int));
}

// Embedded function definitions
void initArray() { // Function that populates the array with the pattern of 1's and 0's

    for (int i = 0; i < row; i++) { // Loops through each row
        for (int j = 0; j < col; j++) { // Loops through each column
            game_map[i][j] = ' ';
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

int* random_pos() { // Function that returns a random position within the array
    int* pos = (int*)malloc(2 * sizeof(int)); // Allocates the memory used to store the [row, col] as 2 ints
    if (pos == NULL) {
        printf("Memory allocation failed!\n");
        exit(1); // Exit the program if allocation fails
    }
    pos[0] = rand() % (row); // Sets the vertical position
    pos[1] = rand() % (col); // Sets the horizontal position
    return pos; // Formated as [row, col]
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
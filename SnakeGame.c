// Included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include <unistd.h>
#include <time.h>

// Defining constants
#define row 6
#define col 24

// Key binds
#define UP 'w' // Key to start the upwards animation
#define LEFT 'a' // Key to start the leftwards animation
#define DOWN 's' // Key to start the downwards animation
#define RIGHT 'd' // Key to start the rightwards animation
#define PAUSE ' ' // Key to pause and unpause the animation
#define QUIT 'q' // Key to quit the program

// Global variables
int game_map[row][col]; // The array is defined as a global variable to permit the functions to eddit it directly

// Embedded function decleration
void initArray();
void printArray();
int* random_pos();

// Main loop
int main() {
    srand((unsigned int)time(NULL)); // Random number generator initilisation (Seed init)
    initArray();
    int* rand = random_pos();
    game_map[rand[0]][rand[1]] = '#';
    printArray();
    printf("%i, %i\n", rand[0], rand[1]);
    free(rand);
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
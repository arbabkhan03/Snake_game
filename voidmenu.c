
// //code menu 
// void menu() {
//     system("cls"); // clear screen on Windows
//     printf("=====================================\n");
//     printf("           SNAKE GAME MENU           \n");
//     printf("=====================================\n");
//     printf("Controls:\n");
//     printf("   W - Move Up\n");
//     printf("   A - Move Left\n");
//     printf("   S - Move Down\n");
//     printf("   D - Move Right\n");
//     printf("   Q - Quit Game\n");
//     printf("\nPress any key to start...\n");
//     getch(); // waits for user to press a key before continuing
// }

/*
Two different menu functions decide which one to use. 
*/

// void menu() {
//     char choice;

//     while (1) {
//         system("cls"); // Clears the console screen (Windows-specific)
//         printf("=====================================\n");
//         printf("         WELCOME TO SNAKE GAME       \n");
//         printf("=====================================\n");
//         printf("1. Start Game\n");
//         printf("2. Instructions\n");
//         printf("3. Exit\n");
//         printf("=====================================\n");
//         printf("Enter your choice (1/2/3): ");

//         choice = getch();

//         switch (choice) {
//             case '1':
//                 return; // Start game (just returns to main)
//             case '2':
//                 system("cls");
//                 printf("============= INSTRUCTIONS =============\n");
//                 printf("Use WASD keys to control the snake:\n");
//                 printf("  W - Up\n");
//                 printf("  A - Left\n");
//                 printf("  S - Down\n");
//                 printf("  D - Right\n\n");
//                 printf("Eat '*' to grow. Don't hit yourself!\n");
//                 printf("Press 'Q' to quit the game.\n");
//                 printf("Press any key to return to menu...");
//                 getch();
//                 break;
//             case '3':
//                 printf("Exiting game. Goodbye!\n");
//                 exit(0);
//             default:
//                 printf("\nInvalid choice. Try again.\n");
//                 Sleep(1000);
//                 break;
//         }
//     }
// }


// #include "includes/minishell.h"

// struct termios orig_termios;

// void disable_raw_mode(void) {
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
// }

// void enable_raw_mode(void) {
//     struct termios raw;
    
//     tcgetattr(STDIN_FILENO, &orig_termios);
//     atexit(disable_raw_mode);
    
//     raw = orig_termios;
//     raw.c_lflag &= ~(ECHO | ICANON | ISIG);
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
// }

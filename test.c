#include <stdio.h>

enum Day {
    SUNDAY,     // 0
    MONDAY,     // 1
    TUESDAY,    // 2
    WEDNESDAY,  // 3
    THURSDAY,   // 4
    FRIDAY,     // 5
    SATURDAY    // 6
};

int main() {
    enum Day today;

    today = WEDNESDAY;
	enum Day flag = THURSDAY;
    if (flag == THURSDAY) {
        printf("It's is flag, my dude!\n");
    }

    return 0;
}

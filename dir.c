#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main() {
    struct dirent *entry;
    DIR *dir = opendir("."); 
    printf("Contents of directory:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir); 
    return EXIT_SUCCESS;
}


#include <stdio.h>
#include <string.h>

// Function to check if a string matches a given pattern
int isMatch(const char *str, const char *pat) {
    int sLen = strlen(str);
    int pLen = strlen(pat);
    int s = 0, p = 0;
    int star = -1, match = -1;

    while (s < sLen) {
        // Match character or '?'
        if (p < pLen && (pat[p] == '?' || pat[p] == str[s])) {
            s++;
            p++;
        }
        // Match '*'
        else if (p < pLen && pat[p] == '*') {
            star = p;
            match = s;
            p++;
        }
        // Backtrack if previous '*' exists
        else if (star != -1) {
            p = star + 1;
            match++;
            s = match;
        }
        // No match
        else {
            return 0;
        }
    }

    // Check for remaining characters in pattern
    while (p < pLen && pat[p] == '*') {
        p++;
    }

    return p == pLen;
}

int main() {
    const char *pat = "b*a*a?";
    const char *str = "baaabab";

    if (isMatch(str, pat)) {
        printf("1\n"); // Match
    } else {
        printf("0\n"); // No match
    }

    return 0;
}

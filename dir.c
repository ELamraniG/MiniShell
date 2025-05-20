#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main() {
    struct dirent *entry;
    DIR *dir = opendir("."); 
    printf("Contents of directory:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("-> %s\n", entry->d_name);
    }
    closedir(dir); 
    return EXIT_SUCCESS;
}

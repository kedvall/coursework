#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main(void)
{
    struct dirent *de;  // Pointer for directory entry
    const char * envar = "USERPROFILE";

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(getenv("USERPROFILE"));

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL)
            printf("%s\n", de->d_name);

    closedir(dr);
    return 0;
}

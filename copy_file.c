#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf(2, "Usage: copy files\n");
        exit();
    }
    else if (argc != 3) {
        printf(2, "Not correct input!\n");
        exit();
    }

    if (copy_file(argv[1], argv[2]) < 0) {
        printf(2, "Failed to copy file\n");
        exit();
    }

    exit();
}

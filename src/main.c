#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#define SIZE 4096

int main(int argc, char * argv[]) {
    char lib_path[SIZE] = "bin/sandbox.so";
    char sandbox_dir[SIZE] = "./";
    char ch;

    while ((ch = getopt(argc, argv, "p:d:")) != -1) {
        switch (ch) {
            case 'p':
                strncpy(lib_path, optarg, sizeof(lib_path));
                break;

            case 'd':
                strncpy(sandbox_dir, optarg, sizeof(sandbox_dir));
                break;

            default:
                fprintf(stderr, "usage: bin/main [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n");
                fprintf(stderr, "        -p: set the path to sandbox.so, default = lib/sandbox.so\n");
                fprintf(stderr, "        -d: the base directory that is allowed to access, default = .\n");
                fprintf(stderr, "        --: separate the arguments for sandbox and for the executed command\n");

                exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc == optind) {
        fprintf(stderr, "no command given.\n");

        exit(EXIT_FAILURE);
    } else {
        char cmd[SIZE] = "\0";

        snprintf(cmd, sizeof(cmd), "LD_PRELOAD=%s SANDBOX_DIR=%s", lib_path, sandbox_dir);

        while (optind < argc) {
            strncat(cmd, " ", strlen(" "));
            strncat(cmd, argv[optind], strlen(argv[optind]));

            optind++;
        }

        system(cmd);
    }

    return EXIT_SUCCESS;
}

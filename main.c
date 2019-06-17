#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


void printHelp(char **argv){
    printf(
            "Usage: %s [OPTION]...\n"
            "\nblah blah blah"
            );
}

void handle_command(int argc, char **argv, int *root, int *hide, int *unhide){
    if (argc < 2) {
        fprintf(stderr, "Error: No arguments provided.\n\n");
        // Does this work???
        printHelp(argv);
        exit(1);
    }
    // What is this????
    opterr = 0;

    // Wooow, what's an option
    static struct option long_options[] ={
            {"root-shell", no_argument, 0, 'a'},
            {"hide", no_argument, 0, 'f'},
            {"unhide", no_argument, 0, 'g'},
            {0, 0, 0, 0}
    };

    *root = 0;
    *hide = 0;
    *unhide = 0;

    int opt;
    while ((opt = getopt_long(argc, argv, ":", long_options, NULL)) != -1){

        switch (opt){
            case 'a':
                *root = 1;
                printf("Root");
                break;
            case 'f':
                printf("hide");
                *hide = 1;
                break;
            case 'g':
                printf("unhide");
                *unhide = 1;
                break;
            case '?':
                fprintf(stderr, "Error: Unrecognised option %s\n\n", argv[optind - 1]);
                printHelp(argv);
                exit(1);
            case ':':
                fprintf(stderr, "Error: Unrecognised option %s\n\n", argv[optind - 1]);
                printHelp(argv);
                exit(1);
        }
    }
//    size_t buf_size = 0;
//    printf("\nInitial: %zu", buf_size);
//    buf_size += sizeof(CFG_PASS)

}

void pointer_example(){
    int i = 25;
    int *ptr = &i;

    printf("The value of i(*ptr):%d", *ptr);
    printf("\nThe address of i(*ptr):%p", ptr);
    printf("\nThe value of i: %d", i);
    printf("\nThe address of i(&i):%p", &i);
    printf("\nThe address of ptr(&ptr):%p", &ptr);
}

int main(int argc, char **argv) {
    int root;
    int hide;
    int unhide;

//    printf("\nThe address of root: %p", &root);
//    printf("\nThe address of hide: %p", &hide);
//    printf("\nThe address of unhide: %p", &unhide);

    handle_command(argc, argv, &root, &hide, &unhide);
    return 0;
}
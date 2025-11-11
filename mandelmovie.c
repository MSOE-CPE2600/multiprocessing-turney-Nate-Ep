/**
* @file mandelmovie.c
* @author Nathan Eppler <epplern@msoe.edu>
* @date 9 November 2025
* @brief Creates a short video zooming into a mandelbrot image set
* 
* Course: CPE2600
* Section: 111
* Due: 19 November 2025
* 
* To Compile: gcc -Wextra -o mandelmovie mandelmovie.c
* To Run: ./mandelmovie <num processors>
*/

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

void gen_images(int num_proc);

int main (int argc, char *argv[]) {

    int opt = getopt(argc, argv, ":p:"); //set custom behavior for invalid num args with preceeding ':'

    if (opt != -1) { //if all command line options have been parsed
        switch(opt) {
            case 'p':
                printf("Option p selected\nArgument: %s\n", optarg);
                char *endptr;
                int num_proc = strtod(optarg, &endptr);

                if (endptr == optarg || *endptr != '\0') {
                    printf("Error: invalid number of processors '%s'\n", optarg);
                    exit(EXIT_FAILURE);
                }

                gen_images(num_proc);
                break;
            case ':':
                printf("Error: No argument input\n");
                break;
            case '?':
                printf("Error: Invalid command\n");
                break;
            default:
                printf("Error: no command found\n");
                break;
        }
    } else {
        printf("Error: must use flag -p for proper execution\n");
    }
}

/**
* @brief Generates 50 images using the specified number of processors
* @param num_proc The number of processors to use to generate the 50 images (1-50)
*/
void gen_images(int num_proc) {
    printf("Generating images\n");
    char *args[] = {"./mandel", "-x", "-0.787", "-y", "-0.287", NULL}; //path of file to run, null terminated array of char args
    errno = -1;
    execvp(args[0], args);
    perror("Error running execvp");
    exit(EXIT_FAILURE);
}
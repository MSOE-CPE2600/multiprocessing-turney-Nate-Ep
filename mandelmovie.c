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

#include <unistd.h>
#include <stdio.h>

int main (int argc, char *argv[]) {

    int opt = getopt(argc, argv, ":p:"); //set custom behavior for invalid num args with preceeding ':'

    if (opt != -1) { //if all command line options have been parsed
        switch(opt) {
            case 'p':
                printf("Option p selected\n");
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
    }
}
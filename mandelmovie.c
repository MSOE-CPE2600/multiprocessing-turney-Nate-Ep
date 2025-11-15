/**
* @file mandelmovie.c
* @author Nathan Eppler <epplern@msoe.edu>
* @date 9 November 2025
* @brief Creates a short video zooming into a mandelbrot image set
* 
* Assignment: Lab 11 - Multiprocessing
* Course: CPE2600
* Section: 111
* Due: 19 November 2025
* 
* To Compile: gcc -Wextra -o mandelmovie mandelmovie.c -lm
* To Run: ./mandelmovie -p <num processors>
* To Stitch Images: ffmpeg -framerate 15 -i mandel%d.jpg -c:v libx264 -pix_fmt yuv420p output.mp4
*/

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <math.h>

#define NUM_IMAGES 50
#define DEFAULT_SCALE 4

void gen_images(int num_proc, int num_threads);

int main (int argc, char *argv[]) {
    int num_proc = 1;
    int num_threads = 1;
    char *endptr;
    int opt; //set custom behavior for invalid num args with preceeding ':'

    while ((opt = getopt(argc, argv, ":p:t:")) != -1)  { //if all command line options have not been parsed
        switch(opt) {
            case 'p': //get number of processors
                num_proc = strtod(optarg, &endptr);

                if (endptr == optarg || *endptr != '\0') {
                    printf("Error: invalid number of processors '%s'\n", optarg);
                    exit(EXIT_FAILURE);
                } else if (num_proc > 50) {
                    printf("Number of processors must be in the range of 1 to 50\n");
                    exit(EXIT_FAILURE);
                }

                break;
            case 't': //get number of threads
                num_threads = strtod(optarg, &endptr);

                if (endptr == optarg || *endptr != '\0') {
                    printf("Error: invalid number of threads '%s'\n", optarg);
                    exit(EXIT_FAILURE);
                } else if (num_threads > 20) {
                    printf("Number of threads must be in the range of 1 to 20\n");
                    exit(EXIT_FAILURE);
                }
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

    gen_images(num_proc, num_threads); //generate images

    return 0;
}

/**
* @brief Generates 50 images using the specified number of processors
* @param num_proc The number of processors to use to generate the 50 images (1-50)
*/
void gen_images(int num_proc, int num_threads) {
    int active_proc = 0;
    printf("Generating images\n");
    char *args[12]; //path of file to run, null terminated array of char args
    char num_threads_char[10];
    char scale[50];
    char output[50];
    //define constant flags/commands
    args[0] = "./mandel"; //always stays ./mandel for executable to call
    args[1] = "-x"; //loc 2 is x val
    args[2] = "-0.787";
    args[3] = "-y"; //loc 4 is y val
    args[4] = "-0.287";
    args[5] = "-s"; //loc 6 is scale, default is 4
    args[7] = "-o"; //loc 8 is output val
    args[9] = "-t"; //loc 10 is number of threads
    sprintf(num_threads_char, "%d", num_threads); //pass in number of threads
    args[10] = num_threads_char;
    args[11] = NULL;

    for (int i = 0; i < NUM_IMAGES; ++i) { //generate 50 images
        if (active_proc >= num_proc) {
            wait(NULL); //do not keep forking, wait for child processes, then continue
            active_proc--;
        }

        int pid = fork();

        if (pid == 0) {
            sprintf(scale, "%f", (DEFAULT_SCALE * pow(0.9, i)));
            sprintf(output, "mandel%d.jpg", i);
            //set scale and output name
            args[6] = scale;
            args[8] = output;

            errno = -1;
            execvp(args[0], args); //exits the child process, so no need for exit cmd
            printf("I: %d", i);
            perror("Error running execvp");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            //parent process
            active_proc++; //increment num processors, then loop back to fork again
        }
    }

    while (active_proc > 0) { //wait for all remaining processes
        wait(NULL);
        active_proc--;
    }

}
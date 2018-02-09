/*
 * Joshua Bearden
 * CS4760 Assignment 1
 * 2/8/2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "makeargv.h"

#define MAX_BUF 200
#define MAX_ARG 3

//A program to create child processes running concurrently
//that run a given command (with arguments) from stdin

int main(int argc, char *argv[])
{
   pid_t pid = 0; //process ID so we know whether parent or child
   pid_t wait = 0; //holds the process ID for the last terminated process
   int i, pr_limit, pr_count, status;
   pr_count = 0;
   pr_limit = -1;
   char str[MAX_BUF]; //Holds the command to be executed and its args
   char * done;
   char delim[] = " ";
   int opt;

   //Process the command line arguments
   while ((opt = getopt(argc, argv, "n:h")) != -1)
   {

      switch (opt)
      {
         case 'n': //Used the switch -n, get the number of concurrent processes
            pr_limit = atoi(optarg);
            break;
         case 'h': //Used the switch -h, display help info and exit
            printf("To run this program, use ./runsim -n num < testing.data\n");
            printf("For example, ./runsim -n 2 < testing.data\n");
            exit(1);
         default: //Used anything else, display error and exit
            perror("Command line arguments are incorrect! Please run ./runsim -h");
            exit(1);
      }
   }
   
   if (pr_limit <= 0) //If the user input an invalid integer, error and quit
   {
      perror("Command line arguments are incorrect! Please run ./runsim -h");
      exit(1);
   }
   

   //Set the max number of concurrent processes
   //pr_limit = atoi(argv[1]);

   //read the first line of input from stdin
   done = fgets(str, MAX_BUF, stdin);

   //main loop
   while(done != NULL)
   {
      printf("Command is: %s\n", str);

      //check if we are running the max number of processes
      if(pr_count == pr_limit)
      {
         //wait for one to terminate
         wait = waitpid(-1, &status, WNOHANG);
         while(wait == 0)
         {
            wait = waitpid(-1, &status, WNOHANG);
         }
         printf("Process %d exited with status %d\n", wait, status);
         pr_count--;
      }
      
      //make a new child process
      pid = fork();

      //if this is the child, get the command and args to run and exec      
      if(pid == 0)
      {
         char **args;
         //call makeargv to make our array. Returns -1 if it fails.
         if (makeargv(str, delim, &args) == -1)
         {
            fprintf(stderr, "%s: ", argv[0]);
            perror("makeargv failed");
         }
         execvp(args[0], args); //exec our process, error if failure
         fprintf(stderr, "%s: ", argv[0]);
         perror("Exec failed.");
      } 
      else //if this is the parent, increment num of processes & check if a process terminated
      {
         pr_count++;
         wait = waitpid(-1, &status, WNOHANG);
         if (wait != 0)
         {
            pr_count--;
            printf("Process %d exited with status %d\n", wait, status);
         }
      }
      done = fgets(str, MAX_BUF, stdin);
   }

   //Once the input has been exhausted, wait for each process to finish
   while (pr_count > 0)
   {
      wait = waitpid(-1, &status, WNOHANG);
      if(wait != 0)
      {
         pr_count--;
         printf("Process %d exited with status %d\n", wait, status);
      }
   }

   return 0;
}
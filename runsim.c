/*
 * Joshua Bearden
 * CS4780 Assignment 1
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
   char str[MAX_BUF]; //Holds the command to be executed and its args
   char * done;

   //Ensure the program was executed correctly
   if(argc != 2)
   {
      perror("Wrong number of command line arguments!\n");
      return 1;
   }

   //Set the max number of concurrent processes
   pr_limit = atoi(argv[1]);

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
         char * args[MAX_ARG];
         makeargv(str, " ", &args)
         execv(args[0], args);
         perror("Exec failed.\n");
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
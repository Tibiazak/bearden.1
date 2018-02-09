/* 
 * testsim.c 
 * A program designed to be forked off
 * Just waits and prints out messages while it waits
 * And prints its process ID when it quits.
 *
 * Given by Shahrouz Yousefi to CS2750
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int n;
	int sec;

	if (argc <= 2)
	{
		fprintf(stderr, "Usage: %s sleep_time repeat_factor\n", argv[0]);
		exit(1);
	}

	sscanf(argv[1], "%d", &sec);
	sscanf(argv[2], "%d", &n);

	while (n-- > 0)
	{
		sleep(sec);
		printf("Process %d is waiting up! [%d]\n", getpid(), n+1);
	}
	printf("Process ID = %d \n", getpid());
	return sec;
}

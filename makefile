all: runsim testsim

runsim: runsim.o makeargv.o
	gcc runsim.o makeargv.o -o runsim
runsim.o:
	gcc -c runsim.c
makeargv.o:
	gcc -c makeargv.c

testsim: testsim.o
	gcc testsim.o -o testsim
testsim.o:
	gcc -c testsim.c
	
clean:
	-rm -rf *.o runsim testsim

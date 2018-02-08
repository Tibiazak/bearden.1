all: runsim testsim

runsim: runsim.o
	gcc runsim.o -o runsim
runsim.o:
	gcc -c runsim.c

testsim: testsim.o
	gcc testsim.o -o testsim
testsim.o:
	gcc -c testsim.c
	
clean:
	-rm -rf *.o runsim testsim

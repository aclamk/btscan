all: btscan.so

btscan.o: btscan.cpp Makefile
	g++ -c -Wall -Werror -fpic $< -o $@
btscan.so: btscan.o Makefile
	g++ -shared -o $@ $< -lpthread
	
all: cpu

cpu:
	mkdir -p bin
	gcc -g -Wall -I../include -I../../commons/so-commons-library/ -L../../commons/so-commons-library/ -lcommons -lpthread src/*.c -o bin/cpu

clean:
	rm -rf bin/cpu

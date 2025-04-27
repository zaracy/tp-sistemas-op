all: kernel

kernel:
	mkdir -p bin
	gcc -g -Wall -I../include -I../../commons/so-commons-library/ -L../../commons/so-commons-library/ -lcommons -lpthread src/*.c -o bin/kernel

clean:
	rm -rf bin/kernel
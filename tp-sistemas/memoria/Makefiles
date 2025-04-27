all: memoria

memoria:
	mkdir -p bin
	gcc -g -Wall -I../include -I../../commons/so-commons-library/ -L../../commons/so-commons-library/ -lcommons -lpthread src/*.c -o bin/memoria

clean:
	rm -rf bin/memoria

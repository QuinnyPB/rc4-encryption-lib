all: arcfour encrypt
encrypt: encrypt.o
	gcc arcfour.so encrypt.o -o encrypt -Wall -O2

encrypt.o: encrypt.c
	gcc -c -O2 -Wall encrypt.c 

# library,  *.so = extension library + additional flags for shared library
arcfour: arcfour.o
	gcc arcfour.o -o arcfour.so -O2 -Wall -fPIC -shared -D_GNU_SOURCE

# Command for compiling program
arcfour.o: arcfour.c
	gcc -c -O2 -Wall arcfour.c

clean: 
	del *.o *.so encrypt.exe


cube: cube.o
	gcc -o cube cube.o -lGL -lGLU -lglut
cube.o: cube.c cube.h
	gcc -g -c cube.c
clean:
	rm cube.o cube
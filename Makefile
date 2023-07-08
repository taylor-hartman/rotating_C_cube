cube: cube.o
	gcc -o cube cube.o -lGL -lGLU -lglut -lm -lgsl
cube.o: cube.c cube.h
	gcc -g -c cube.c
matrix: matrix.c
	gcc -g matrix.c -lgsl -o matrix
clean:
	rm cube.o cube
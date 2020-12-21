CC=gcc

all: maestro.out estudiante.out

maestro.out: maestro.c
	$(CC) $^ -o $@

estudiante.out: estudiante.c
	$(CC) $^ -o $@ -lm

clean:
	rm *.out *.fifo

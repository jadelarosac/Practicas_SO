CC=gcc

all: servidor.out clientes.out proxy.out

%.out: %.c
	$(CC) $^ -o $@


clean:
	rm -f *.out FIFO* fifo* bloq
	killall servidor.out

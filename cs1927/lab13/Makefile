CC=gcc
CFLAGS= -Wall -g -c 
LDFLAGS=

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

gc: GRAPH_adjlist.o graph_client.o
	$(CC) $(LDFLAGS) $+ -o $@

clean:
	rm *.o;rm gc
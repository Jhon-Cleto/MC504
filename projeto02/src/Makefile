CFLAGS = -c -pthread
LDFLAGS = -pthread

rollercoaster: main.o animation.o
	$(CC) $(LDFLAGS) -o $@ $^
	rm *.o

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

animation.o: animation.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o
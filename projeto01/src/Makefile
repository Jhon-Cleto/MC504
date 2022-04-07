CFLAGS = -pthread

matmul: main.o
	$(CC) -o $@ $< 
	rm -f *.o

matmul.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o matmul matmul_demo
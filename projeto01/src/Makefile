CFLAGS = -pthread -c
LDFLAGS = -pthread

matmul: matmul.o
	$(CC) $(LDFLAGS) -o $@ $< 
	rm -f *.o

matmul_demo: matmul_demo.o
	$(CC) $(LDFLAGS) -o $@ $< 
	rm -f *.o
	
matmul.o: main.c
	$(CC) $(CFLAGS) -o $@ $<

matmul_demo.o: main_demo.c
	$(CC) $(CFLAGS) -o $@ $<
	
clean:
	rm -rf *.o matmul matmul_demo
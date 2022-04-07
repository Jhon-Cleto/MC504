CFLAGS = -pthread

matmul: matmul.o
	$(CC) -o $@ $< 
	rm -f *.o

matmul_demo: matmul_demo.o
	$(CC) -o $@ $< 
	rm -f *.o
	
matmul.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

matmul_demo.o: main_demo.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o matmul matmul_demo
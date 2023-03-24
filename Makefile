CC=gcc
CFLAGS=-Wextra -g
TARGET=tema3

build:
	$(CC) $(CFLAGS) tema3.c tarb.c -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) *~

valgrind:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)


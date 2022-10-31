CC = gcc
CFLAGS = -Wall -O2 -g
LIBS =
LDFLAGS =
SRC = tests.c type/pile.c type/case.c fichiers.c
EXEC = tests
OBJ = $(SRC:.c=.o)


all: $(EXEC)


tests: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o type/*.o *~

mrproper: clean
	rm -rf $(EXEC)

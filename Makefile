CC = gcc
CFLAGS = -Wall -O2 -g
LIBS = -L./SDL2_ttf/.libs
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf
SRC = main.c type/pile.c type/case.c fichiers.c fonctions_SDL.c
EXEC = main
OBJ = $(SRC:.c=.o)


all: $(EXEC)


main: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o type/*.o *~

mrproper: clean
	rm -rf $(EXEC)

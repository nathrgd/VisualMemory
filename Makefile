CC = gcc
CFLAGS = -Wall -O2 -g
LIBS = -L./SDL2_ttf/.libs
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf
SRC = visualmemory.c type/case.c type/file.c processing.c jeu.c fichiers.c fonctions_SDL.c
EXEC = visualmemory
OBJ = $(SRC:.c=.o)


all: $(EXEC)


visualmemory: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ) *~

mrproper: clean
	rm -rf $(EXEC)

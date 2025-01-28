# Nom de l'exécutable
TARGET = bin/Pioneers.exe

# Fichiers source
SRC = $(wildcard src/*.c)

# Fichiers objets générés
OBJ = $(SRC:.c=.o)

# Commande pour récupérer les drapeaux SDL2 automatiquement
CFLAGS = -Wall `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf -lSDL2_image

# Compilateur
CC = gcc

# Règles
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f src/*.o $(TARGET)

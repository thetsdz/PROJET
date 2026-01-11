# -----------------------------------------------------------------------------
# MAKEFILE POUR RAYLIB (Refactorisé)
# -----------------------------------------------------------------------------

PROJECT_NAME = game
CC = gcc
CFLAGS = -Wall -std=c99 -Wno-missing-braces -I.

# LISTE DES FICHIERS SOURCE
SRCS = main.c level.c player.c projectile.c

# DÉTECTION OS
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
    EXT = .exe
    RM = del
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        EXT = 
        RM = rm -f
    endif
    ifeq ($(UNAME_S),Darwin)
        LDFLAGS = -lraylib -framework IOKit -framework Cocoa -framework OpenGL
        EXT = 
        RM = rm -f
    endif
endif

# RÈGLES
all:
	$(CC) $(SRCS) -o $(PROJECT_NAME)$(EXT) $(CFLAGS) $(LDFLAGS)

run: all
	./$(PROJECT_NAME)$(EXT)

clean:
	$(RM) $(PROJECT_NAME)$(EXT)
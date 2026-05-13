# Compilador, opções de compilação, bibliotecas e nome do executável
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = jogo

# Arquivos fonte do projeto e seus objetos correspondentes
SRC = main.c mapa.c player.c render.c
OBJ = $(SRC:.c=.o)

# Regra padrão: compila o executável final
all: $(TARGET)

# Cria o executável a partir dos arquivos objeto
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compila cada arquivo .c em .o usando as flags definidas
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove arquivos de compilação gerados
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
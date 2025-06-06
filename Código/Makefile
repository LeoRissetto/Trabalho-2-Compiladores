CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

SRCS = main.c lexico.c sintatico.c hash_table.c
OBJS = $(SRCS:.c=.o)
TARGET = compilador

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) 
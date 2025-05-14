CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

SRCS = main.c lexico.c
OBJS = $(SRCS:.c=.o)
TARGET = lexico

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
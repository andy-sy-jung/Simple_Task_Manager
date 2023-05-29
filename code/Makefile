CC      = gcc
TARGET  = TaskManager
C_FILES = $(filter-out $(TARGET).c, $(wildcard *.c))
OBJS    = $(patsubst %.c,%.o,$(C_FILES))
CFLAGS  = -g -Wall -Werror -pedantic-errors
LDFLAGS = -framework OpenGL -I/opt/homebrew/Cellar/glfw/3.3.8/include -L/opt/homebrew/Cellar/glfw/3.3.8/lib -lglfw

.PHONY: all clean
all: $(TARGET)
$(TARGET): $(OBJS) $(TARGET).c
	$(CC) $(CFLAGS) $(OBJS) $(TARGET).c -o $(TARGET) $(LDFLAGS)
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe


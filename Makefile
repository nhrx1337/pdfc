# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -lm `pkg-config --cflags gtk+-3.0 poppler-glib`

# Source files
SRCS = main.c controls.c pdf_view.c

# Header files
HEADERS = controls.h pdf_view.h

# Target executable
TARGET = pdfc

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS) `pkg-config --libs gtk+-3.0 poppler-glib`

# Clean target to remove compiled files
clean:
	rm -f $(TARGET)


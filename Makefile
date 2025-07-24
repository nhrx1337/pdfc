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

# Installation directory
INSTALL_DIR = /usr/local/bin

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS) `pkg-config --libs gtk+-3.0 poppler-glib`

# Install target to copy the executable to /usr/local/bin/
install: $(TARGET)
	install -m 755 $(TARGET) $(INSTALL_DIR)/

# Uninstall target to remove the executable from /usr/local/bin/
uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

# Clean target to remove compiled files
clean:
	rm -f $(TARGET)

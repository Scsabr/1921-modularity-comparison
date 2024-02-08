# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -std=c99

# Source files
SRCS = errors.c ioUtils.c structs.c ebfEcho.c

TSRCS = errors.c ioUtils.c structs.c unittestexample.c

# Object files
OBJS = $(SRCS:.c=.o)

TOBJS = $(TSRCS:.c=.o)

# Default rule
all: ebfEcho test

# Rule to build the executable
ebfEcho: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

test: $(TOBJS)
	$(CC) $(CFLAGS) -o $@ $(TOBJS)

# Rule to compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) $(OBJS) $(TOBJS) test ebfEcho 

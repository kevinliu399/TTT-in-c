CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lraylib -lm -ldl -lpthread

SRCS = main.c
OBJS = $(SRCS:.c=.o)
MAIN = tic_tac_toe

.PHONY: clean

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
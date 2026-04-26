CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wconversion -Werror -g
TARGET  = sensor_fsm
SRCS    = main.c sensor.c fsm.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

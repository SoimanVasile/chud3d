CC=gcc
CFLAGS=-Wall -Wextra -gdwarf
LFLAGS=-lglfw

SRCS:=$(wildcard src/*.c)
OBJS:=$(patsubst src/%.c, ./%.o, $(SRCS))

LIB_SRCS:=$(wildcard lib/**/src/*.c)
LIB_OBJS:=$(patsubst %.c, ./%.o, $(notdir $(LIB_SRCS)))



TARGET=terrea


all:$(TARGET)



$(TARGET):$(OBJS) $(LIB_OBJS)
	$(CC) $(LFLAGS)  $^ -o $(TARGET)


$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) $^ -c $<

$(LIB_OBJS): $(LIB_SRCS)
	$(CC) $(CFLAGS) $^ -c $<

clean:
	rm $(OBJS)
	rm $(LIB_OBJS)
	rm $(TARGET)

makedebug:
	@echo $(notdir $(LIB_OBJS))

.PHONY: clean, makedebug

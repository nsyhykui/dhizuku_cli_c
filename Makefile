CC ?= gcc
CFLAGS += -Wall -Wextra -O2 -g -Iinclude
LDFLAGS ?=
LDLIBS = -lssl -lcrypto

TARGET = dcli
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

.PHONY: all clean

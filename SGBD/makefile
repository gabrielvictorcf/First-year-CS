PROGNAME = banco

CFLAGS := -Wall -Werror

C_SRCS := $(wildcard ./source/*.c)

all:
	@gcc $(C_SRCS) -o $(PROGNAME) $(CFLAGS)

run:
	@./$(PROGNAME)
PROGNAME = filtro

ifdef IN
INPUT := < $(IN)
endif

CFLAGS := -Wall -Werror

C_SRCS := $(wildcard ./source/*.c)

all:
	@gcc $(CFLAGS) $(C_SRCS) -o $(PROGNAME)

run:
	@./$(PROGNAME)

valgrind: all
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROGNAME) $(INPUT)
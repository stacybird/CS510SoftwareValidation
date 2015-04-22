
.PHONY: clean cleanall

CC=gcc

SOURCES = main.c list.c parser.c pstack.c
OBJECTS = $(SOURCES:.c=.o)
PROGRAM = clic
LDFLAGS = -ldl -lm -lreadline -lhistory -ltermcap
CFLAGS += -Wall

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
clean:
	rm -f $(OBJECTS)
cleanall:
	rm -f $(OBJECTS) $(PROGRAM)

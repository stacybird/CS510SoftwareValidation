
.PHONY: clean cleanall

CC=gcc

SOURCES = main.c list.c parser.c pstack.c
OBJECTS = $(SOURCES:.c=.o)
TESTLOG = $(SOURCES:.c=.gcno)
PROGRAM = clic
LDFLAGS = -ldl -lm -lreadline -lhistory -ltermcap --coverage
CFLAGS += -Wall --coverage -fprofile-arcs -ftest-coverage

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
clean:
	rm -f $(OBJECTS)
cleanall:
	rm -f $(OBJECTS) $(TESTLOG) $(PROGRAM)

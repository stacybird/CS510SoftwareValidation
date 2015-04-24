
.PHONY: clean cleanall

CC=gcc

SOURCES = data_structure.c 
OBJECTS = $(SOURCES:.c=.o)
TESTLOG = $(SOURCES:.c=.gcno)
PROGRAM = data_structure
LDFLAGS = -ldl -lm -lreadline -lhistory -ltermcap --coverage
CFLAGS += -Wall --coverage -fprofile-arcs -ftest-coverage

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
clean:
	rm -f $(OBJECTS)
cleanall:
	rm -f $(OBJECTS) $(TESTLOG) $(PROGRAM)

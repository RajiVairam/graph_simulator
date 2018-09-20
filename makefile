GRAPH_LIBRARY := libgraph.so
CLI_LIBRARY := libcli.so
BIN := graph_simulator

CC := g++

CPPFLAGS += -fPIC
LDFLAGS += -L ./ -Wl,-rpath=./

ifeq ($(DEBUG), yes)
	CPPFLAGS += -g
endif

all : $(BIN)

$(BIN): $(GRAPH_LIBRARY) $(CLI_LIBRARY) main.o
	$(CC) $(LDFLAGS) -o $@ main.o -lgraph -lcli

$(GRAPH_LIBRARY): graph.o
	$(CC) -shared -o $@ $^

$(CLI_LIBRARY): cli.o
	$(CC) -shared -o $@ $^

%.cpp : %.h

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o
	rm -f $(BIN) $(CLI_LIBRARY) $(GRAPH_LIBRARY)

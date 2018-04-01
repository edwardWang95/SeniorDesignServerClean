APP = server

APP_OBJS = server.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	g++ server.o -o server

#compile server into object file
server.o: server.cpp
	g++ -c server.cpp

#remove all object files and output
clean:
	rm -f *.o output
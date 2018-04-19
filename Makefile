APP = server

APP_OBJS = server.o

all: build

build: $(APP)

$(APP): server client

server: server.cpp
	g++ -c server.cpp
	g++ server.o -o server

client: client.cpp
	g++ -c client.cpp
	g++ client.o -o client

#remove all object files and output
clean:
	rm *.o server client
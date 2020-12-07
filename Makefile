CC = g++
OPENCV =  `pkg-config --cflags --libs opencv`
PTHREAD = -pthread

CLIENT = client.cpp
SERVER = server.cpp
CLI = client
SER = server

all: server client

server: $(SERVER)
	$(CC) $(SERVER) -o $(SER)  $(OPENCV) $(PTHREAD) -std=c++11
client: $(CLIENT)
	$(CC) $(CLIENT) -o $(CLI)  $(OPENCV) $(PTHREAD) -std=c++11
opencv:
	$(CC) $(CVFILE) -o $(CV)  $(OPENCV)
.PHONY: clean

clean:
	rm $(CLI) $(SER) client_folder* -rf
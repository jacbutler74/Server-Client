CPPFLAGS = -lws2_32

server.exe: server.o
	g++ -o server.exe server.o $(CPPFLAGS)

server.o: server.cpp
	g++ -c server.cpp

run: server.exe
	./server.exe localhost

clean:
	del server.exe
	del server.o
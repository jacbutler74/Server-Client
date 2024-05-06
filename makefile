CPPFLAGS = -lws2_32

client.exe: client.o
	g++ -o client.exe client.o $(CPPFLAGS)

client.o: client.cpp
	g++ -c client.cpp

run: client.exe
	./client.exe localhost

clean:
	del client.exe
	del client.o
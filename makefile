CPPFLAGS = -lws2_32

client.exe: client.o
	g++ -o client.exe client.o $(CPPFLAGS)

client.o: client.cpp
	g++ -c client.cpp

runC: client.exe
	./client.exe localhost

cleanC:
	del client.exe
	del client.o
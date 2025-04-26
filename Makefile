# Run the 'make' command from your terminal only if you are running both the server and the client on the same computer.
# Run 'make client' if you want to compile the client only. Similarly, 'make server' if you ....
#Run 'make clean' to delete all the object files
# If client and server are running in different computers then copy the following commands and run in the terminals of the appropriate compuetrs.

all: MyChat DNS-Server

MyChat:
	@echo "Compiling the Client..."
	g++ -o MyChat MyChat.cpp DNS-Functions.cpp UDP-Functions.cpp

DNS-Server:
	@echo "Compiling the Server..."
	g++ -o DNS-Server DNS-Server.cpp DNS-Functions.cpp UDP-Functions.cpp

clean:
	@echo "Removing all object files..."
	rm MyChat DNS-Server

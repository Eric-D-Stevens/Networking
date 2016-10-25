Eric Stevens: CS 372 Program 1

INTRODUCTION
This file implements a simple TCP chat application using
the client-server modle. The client side of the application
is implemented in c and can be compiled on the oregonstate.edu
servers using gcc. The server side of the application is
built in python and requires the use of the python inturpreter
to run. This is also available on the oregonstate.edu servers.

RUNNING THE PROGRAM
There are two different programs in two different languages.
 1)	The client source code is "chatclient.c" and can be
	compiled using the normal gcc compilation command
	"gcc -o chatclient chatclient.c", and can then be run
	using "./chatclient". A compiled version will be 
	included in the zip file for your convience.
 2)	The server source code is "chatserve.py" and can be run
	through the python interpreter by typing the command
	"python chatserve.py".

ESTABLISHING CONNECTION
The client and the server applications should first be executed
in seperate terminals. Once both application have been run do
the following steps:
 1) 	Enter port number into the server side. This should result 
	in the program telling you that the server is listening to
	the port that you entered the number for.
 2)	Go to thie client terminal and enter the name of the server
	that the TCP server application is running on. 
 3)	Still in the client window, enter the same port number
	that you entered into the server application in step one.
 4)	Still in the client application, enter the handle name
	that the client will be refered to durring the chat.
	Doing this will establish the connection and both server
	and client terminals will simultaniously read "CONNECTION
	ESTABLISHED" if the process was succesful.
 5)	After entering the handle in the client terminal you will
	be prompted to enter your first message. Do so to begin
	communication. 

DURRING THE CHAT
The application is set up so that every message must alternate
between the client and the server. The first message will be 
issued by the client. The second must be issued by the server.
This patter must precist in order for the application to function
properly.

QUIT COMMAND AND EXIT
While in the chat, you can execute the quit command by typing the
expression "\quit" into either the client or the server message
lines, as if they were regular messages. When you enter this command
two things happen. On the client side, the quit command will close
the connection and exit the client application. On the server side,
the quit command will close the connection, detatch from the current
port, and reprompt you for a new port number to listen to. If you
want to exit the server application, go to the server application 
terminal and use SIGINT (Ctrl+C). 

RESOURCES I UTILIZED TO HELP ME BUILD THIS PROGRAM
https://books.google.com/books?id=bnNuBgAAQBAJ&pg=PA11&lpg=PA11&dq=socket.listen+python&source=bl&ots=-ZerjbM_Cq&sig=zjqm5OA3l2nvX6JEnB8HXNuhujs&hl=en&sa=X&sqi=2&ved=0ahUKEwiqiZOVy4LOAhVD5mMKHZNFDXQQ6AEIHDAA#v=onepage&q=socket.listen%20python&f=false
http://www.tutorialspoint.com/python/python_while_loop.htm
https://docs.python.org/2/library/socket.html
http://www.bogotobogo.com/python/python_network_programming_server_client.php
https://docs.python.org/release/2.6.5/library/socketserver.html#socketserver-tcpserver-example
http://socket.io/get-started/chat/
http://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/tcpclient.c

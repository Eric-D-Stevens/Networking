#
# Eric Stevens
# TCP Messaging Application
# Server Side
#


from socket import*
import os


handel = 'ChatServe >>> '
quit = '\quit'

while 1:
	os.system('clear')
	# first attempt at port number input
	portNum = raw_input('Please Enter A Port Number:')
	# if input is not number, loop
	while not portNum.isdigit():
		print 'not valid'
		portNum = raw_input('Please Enter A Port Number:')
	
	#SET UP SERVER
	serverPort = int(portNum) # assign port number
	serverSocket = socket(AF_INET, SOCK_STREAM) # set up TCP stream
	serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1) # assign socket
	serverSocket.bind(('',serverPort)) # bind socket to application
	serverSocket.listen(1) # begin listening to port
	print 'SERVER READY'
	print 'Listening on Port:'+ portNum
	connectionSocket, addr = serverSocket.accept()
	print '\n---------------CONNECTION ESTABLISHED---------------'	
	while 1:
		
		received = '\n'+ connectionSocket.recv(1024) # receive
		print received # output received 
				
		# mannage received quit commands
		if received.endswith("\\quit"):
			connectionSocket.close()
			serverSocket.close()
			break
		reply = raw_input('\nChatServe >>> ')
		
		toSend = handel + reply # format outupt with handle
		connectionSocket.send(toSend) # send message to client
		
		# mannage sent quit commands
		if reply == quit:
			connectionSocket.close()
			serverSocket.close()
			break

	


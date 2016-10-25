from socket import*
import os


handel = 'ChatServe >>> '
quit = '\quit'

while 1:
	os.system('clear')
	# first attempt at port number input
	portNum = raw_input('Pleas Enter A Port Number:')
	# if input is not number, loop
	while not portNum.isdigit():
		print 'not valid'
		portNum = raw_input('Pleas Enter A Port Number:')
	
	#SET UP SERVER
	serverPort = int(portNum)
	serverSocket = socket(AF_INET, SOCK_STREAM)
	serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
	serverSocket.bind(('',serverPort))
	serverSocket.listen(1)
	print 'SERVER READY'
	print 'Listening on Port:'+ portNum
	connectionSocket, addr = serverSocket.accept()
	print '\n---------------CONNECTION ESTABLISHED-------------------'	
	while 1:
		
		received = '\n'+ connectionSocket.recv(1024)
		print received #+ '\n'
				
		if received.endswith("\\quit"):
			connectionSocket.close()
			serverSocket.close()
			break
		reply = raw_input('\nChatServe >>> ')
		reply = reply #+ '\n'
		toSend = handel + reply
		connectionSocket.send(toSend)
		if reply == quit:
			connectionSocket.close()
			serverSocket.close()
			break

	


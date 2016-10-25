from socket import*

servername = 'localhost'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((servername, serverPort))
sentence = raw_input('chad>>>')

while 1:
	clientSocket.send(sentence)
	servReply = clientSocket.recv(1024)
	print 'from server:', servReply
	sentence = raw_input('chad>>>')
	
clientSocket.close()

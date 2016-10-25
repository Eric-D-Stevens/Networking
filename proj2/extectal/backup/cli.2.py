# TCP client example
import socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(("localhost", 5000))
while 1:
	data = client_socket.recv(512)
	if ( data == 'q' or data == 'Q'):
		client_socket.close()
		break;

	else:
		print "RECIEVED:" , data
		data = raw_input ( "SEND:" )

#-------------------------------------------------------------------#
		if data.startswith("-g"):
			print "got the dash g"
			client_socket.send(data)
			fo = open("something", "w")
			while data != '':
				data = client_socket.recv(1)
				print data
				fo.write(data)
			fo.close()	
#-------------------------------------------------------------------#

		elif (data == '-l'):
			print "made it in"
			client_socket.send(data)
			while data != '':
				data = client_socket.recv(512)
				print data
#-------------------------------------------------------------------#
		else:
			print "not in"
			client_socket.send(data)
			client_socket.close()
			break;


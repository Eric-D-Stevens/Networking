# TCP client example
import socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(("localhost", 5000))
while 1:
		print "REQUEST:" 
		data = raw_input ( "SEND:" )

#-------------------------------------------------------------------#
		if data.startswith("-g"):
			fname = data.split()[1]
			print "got the dash g"
			client_socket.send(data)
			
			data = client_socket.recv(512)
			print data
			
			data_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			data_socket.connect(("localhost", 5001))
			
			fo = open("textfile", "w")
			while data.endswith("eof") != 1:
				data = data_socket.recv(512)
				fo.write(data)
			fo.close()
#			data_socket.close()	
#-------------------------------------------------------------------#

		elif (data == '-l'):
			print "made it in"
			client_socket.send(data)
			while data.endswith("eof") != 1:
				data = client_socket.recv(512)
				print data
#-------------------------------------------------------------------#
		else:
			print "not in"
			client_socket.send(data)
			client_socket.close()
			break;


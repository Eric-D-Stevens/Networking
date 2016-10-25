# TCP client example
import socket
import sys
import os
import time
servername = sys.argv[1]
serverport = sys.argv[2]
cmdOpt = sys.argv[3]

if not(serverport.isdigit()) or (cmdOpt !="-l" and cmdOpt != "-g"): 
	print ("\n\nUsage Error:\n")
	print("View Directory: 'python cli.py <SERVER_PORT> -l <DATA_PORT>'")
	print("File Transfer:  'python cli.py <SERVER_PORT> -g <FILE_NAMDE> <DATA_PORT>'\n\n") 
	sys.exit()



print("\n\n")

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((servername, int(serverport)))
client_socket.send(socket.gethostname())
#time.sleep(10.0/1000.0)
while 1:
	

#-------------------------------------------------------------------#
		if cmdOpt == "-g":
			
			fname = sys.argv[4]
			if os.path.isfile(fname):
				overWrite = raw_input("FILE \"" + fname + "\" ALREADY EXISTS: OVER WRITE? (y/n):")
				if overWrite != 'yes' and overWrite != 'y' and overWrite != 'Y':
					client_socket.send("noOW")
					print("\n\nExiting...\n\n")
					sys.exit()
			

			dataport = sys.argv[5]
			client_socket.send(dataport)
			
			data = client_socket.recv(512)
				
			data = "-g " + fname 
			client_socket.send(data)
			
			data = client_socket.recv(512)
			if str(data) == "no$$ff\0":
				print(servername + ":" + serverport + " SAYS " + fname + "DOES NOT EXIST\n\n")
				sys.exit()	
			data_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			data_socket.connect((servername, int(dataport)))
		
			print("\nRECEIVING \"" + fname + "\" FROM " + servername + ":" + dataport)	
			fo = open(fname, "w")
			while data.endswith("eof") != 1:
				data = data_socket.recv(512)
				if data.endswith("eof"):
					data = data[:-3]
					fo.write(data)
					break
				fo.write(data)
			fo.close()
			print("FILE TRANSFER COMPLETE\n\n")
			sys.exit()

#-------------------------------------------------------------------#

		elif cmdOpt == "-l":
			time.sleep(10.0/1000.0)
			dataport = sys.argv[4]
			client_socket.send(dataport)
			print("RECEIVING DIRECTORY STRUCTURE FROM " + servername + ":" + dataport)

			data = client_socket.recv(512)
			data = "-l"
			client_socket.send(data)
			print "CONTENTS OF CURRENT DIRECTORY:"
			data = client_socket.recv(512)
			print(data)
			data_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			data_socket.connect((servername, int(dataport)))

			while data.endswith("eof") != 1:
				data = data_socket.recv(512)
				if data.endswith("eof"):
					data = data[:-3]
					print data
					break
				print data
			sys.exit()
#-------------------------------------------------------------------#
		else:
			print ("Usage Error:")
			print("View Directory: 'python cli.py <SERVER_PORT> -l <DATA_PORT>'")
			print("File Transfer:  'python cli.py <SERVER_PORT> -g <FILE_NAMDE> <DATA_PORT>'") 
			sys.exit()

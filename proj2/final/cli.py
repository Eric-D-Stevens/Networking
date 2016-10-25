import socket 
import sys
import os
import time

# pull arguments off command line
servername = sys.argv[1]	# name of server to be connected to
serverport = sys.argv[2]	# port on that server to connect to
cmdOpt = sys.argv[3]		# -l or -g

# if any of the inputs are unexpectd print usage message
if not(serverport.isdigit()) or (cmdOpt !="-l" and cmdOpt != "-g"): 
	print ("\n\nUsage Error:\n")
	print("View Directory: 'python cli.py <SERVER_PORT> -l <DATA_PORT>'")
	print("File Transfer:  'python cli.py <SERVER_PORT> -g <FILE_NAMDE> <DATA_PORT>'\n\n") 
	sys.exit()	#exit program

print("\n\n")

#initalize main server socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((servername, int(serverport)))
client_socket.send(socket.gethostname())
#time.sleep(10.0/1000.0)

# MAIN LOOP 
while 1:
#-------------------------------------------------------------------#
		# If command -g is sent
		if cmdOpt == "-g":
						
			fname = sys.argv[4]		# get file name from command line

			# if the file already exists
			if os.path.isfile(fname):			# prompt user to overwrite or not
				overWrite = raw_input("FILE \"" + fname + "\" ALREADY EXISTS: OVER WRITE? (y/n):")
				if overWrite != 'yes' and overWrite != 'y' and overWrite != 'Y':
					client_socket.send("noOW")	# send no overwrite command
					print("\n\nExiting...\n\n")	# exit the program
					sys.exit()
			
			# otherwise
			dataport = sys.argv[5]		# get data port form input
			client_socket.send(dataport)	# send data port number over main connection
			
			data = client_socket.recv(512)	# receive timing message
				
			data = "-g " + fname 		# send command to server over main connection
			client_socket.send(data)	
			
			data = client_socket.recv(512)	# wait for response

			if str(data) == "no$$ff\0":	# if there is a 'file doesnt exist msg'
				print(servername + ":" + serverport + " SAYS " + fname + "DOES NOT EXIST\n\n")
				sys.exit()		# exit the program

			# set up data connection
			data_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			data_socket.connect((servername, int(dataport)))
		
			# receive data and write to file
			print("\nRECEIVING \"" + fname + "\" FROM " + servername + ":" + dataport)	
			fo = open(fname, "w")			# open file with entered file name
			while data.endswith("eof") != 1:	# while eof signal not received
				data = data_socket.recv(512)	# write character to file
				if data.endswith("eof"):	# if eof recieved 
					data = data[:-3]	# remove eof
					fo.write(data)		# write data
					break			# break writing loop
				fo.write(data)			# otherwise write data
			fo.close()				# close file object
			print("FILE TRANSFER COMPLETE\n\n")	
			sys.exit()				# exit program

#-------------------------------------------------------------------#

		# if the -l command is sent
		elif cmdOpt == "-l":
			time.sleep(10.0/1000.0)		# sleep for timing
			dataport = sys.argv[4]		# get data port form command
			client_socket.send(dataport)	# send desired dataport to server
			print("RECEIVING DIRECTORY STRUCTURE FROM " + servername + ":" + dataport)

			data = client_socket.recv(512)	# recieve timing message
			data = "-l"			# send -l command
			client_socket.send(data)
			print("CONTENTS OF CURRENT DIRECTORY:")
			data = client_socket.recv(512)	# timing message
			print(data)
			
			# SET UP DATA CONNECTION
			data_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			data_socket.connect((servername, int(dataport)))
			
			# loop data transfer read
			while data.endswith("eof") != 1:	# while eof signal not received
				data = data_socket.recv(512)	# read character
				if data.endswith("eof"):	# if character is eof signal
					data = data[:-3]	# remove eof character
					print(data)		# print the rest
					break			# break out of reading loop
				print(data)			# otherwise print recieved
			sys.exit()				# exit program
#-------------------------------------------------------------------#
		else:	#if neither the -g or the -l command is received output usage error
			print ("Usage Error:")
			print("View Directory: 'python cli.py <SERVER_PORT> -l <DATA_PORT>'")
			print("File Transfer:  'python cli.py <SERVER_PORT> -g <FILE_NAMDE> <DATA_PORT>'") 
			sys.exit()

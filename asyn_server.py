#-*- coding: utf-8 -*-

# TCP server example
import socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", 5000))
server_socket.listen(5)

print ("TCPServer Waiting for client on port 5000")

while 1:
	client_socket, address = server_socket.accept()
	print ("연결 성공. 상대 아이피 :  ", address)
	while 1:
		data = input('SEND( TYPE q or Q to Quit) : ')
		client_socket.send(data.encode('cp949'))
		if(data == 'Q' or data == 'q'):
			client_socket.close()
			break;

		data = client_socket.recv(512).decode('cp949')
		if(data == 'q' or data == 'Q'):
			client_socket.close()
			break;
		else:
			print ("RECEIVED : " , data)

	break;
server_socket.close()
print("SOCKET closed... END")

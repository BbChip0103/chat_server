#-*- coding: utf-8 -*-

# TCP server example
import socket
import threading

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", 5000))
server_socket.listen(5)

print ("TCPServer Waiting for client on port 5000")

while 1:
	client_socket, address = server_socket.accept()
	print ("연결 성공. 상대 아이피 :  ", address)

	def send_handle :
		data = input('SEND( TYPE q or Q to Quit) : ')
		client_socket.send(data.encode('cp949'))
		if(data == 'Q' or data == 'q'):
			client_socket.close()
			break;

	def recv_handle :
		data = client_socket.recv(512).decode('cp949')
		if(data == 'q' or data == 'Q'):
			client_socket.close()
			break;
		else:
			print ("RECEIVED : " , data)

	while 1:
		threading._start_new_thread(send_handle,())
		threading._start_new_thread(recv_handle,())

	break;
server_socket.close()
print("SOCKET closed... END")

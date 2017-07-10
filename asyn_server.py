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

	def send_handle() :
		while True :
			data = input('보낼 메세지( q or Q 입력시 종료) : ')
			client_socket.send(data.encode('cp949'))
			if(data == 'Q' or data == 'q'):
				client_socket.close()
				break;

	def recv_handle() :
		while True :
			data = client_socket.recv(512).decode('cp949')
			if(data == 'q' or data == 'Q'):
				client_socket.close()
				break;
			else:
				print ("받은 메세지 : " , data)

	threading._start_new_thread(send_handle,())
	threading._start_new_thread(recv_handle,())

	while 1:
		pass

# this code execute next line never  
	break;
server_socket.close()
print("SOCKET closed... END")

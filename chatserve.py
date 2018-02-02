#!/usr/bin/python3
#############################
#Description: operates as a simple chat server
#Input: port to run on (int)
#############################

import socket as S
from threading import Thread 
import sys

class Chat_Server:


	def __init__(self, port, max_clients):
		self.ssoc = S.socket(S.AF_INET, S.SOCK_STREAM)
		self.ssoc.bind(("127.0.0.1", int(port)))
		self.ssoc.listen(max_clients)
		self.clients = []
		self.client_addresses = []
		self.BUFFER_SZ = 500
		print("Server started on port %s" % port)
		print("Host name: %s" %S.gethostname())

	def handle_New_Conn(self):
		while True:
			print("Accepting Connections!!!")
			client, client_address = self.ssoc.accept()
			print("{} has connected".format(client_address))
			self.clients.append(client)
			self.client_addresses.append(client_address)
			client.send("connected!".encode())
			nt = self.client_Thread
			Thread(target=nt, args=(client,)).start()

	def client_Thread(self, client):
		while True:
			msg = client.recv(self.BUFFER_SZ)
			if msg:
				print("broadcast called")
				self.broadcast(msg, client)
			else:
				client.close()
				try:
					self.clients.remove(client)
				except ValueError:
					pass

				break

	def broadcast(self, msg, client):
		print(msg)
		for sock in self.clients:
			sock.send(msg)


cs = Chat_Server(sys.argv[1], 5)
top_thread = Thread(target=cs.handle_New_Conn)
top_thread.start()
top_thread.join()



#!/usr/bin/python3
#############################
#Description: operates as a simple chat server
#Input: port to run on (int)
#############################

import socket as S
from threading import Thread 
import sys

class Chat_Server:

	BUFFER_SZ = 500

	def __init__(self, port, max_clients):
		self.ssoc = S.socket(S.AF_Inet, S.SOCK_STREAM)
		self.ssoc.bind((S.gethostname(), port))
		self.ssoc.listen(max_clients)
		self.clients = []
		self.client_addresses = []
		print("Server started on port %s" % port)

	def handle_New_Conn(self):
		while True:
			client, client_address = ssoc.accept()
		 	print("%s:%s has connected" % client_address)
			self.clients.append(client)
			self.client_addresses.append(client_address)
			client.send("connected!")
			Thread(target=client_Thread, args=(client,)).start()

	def client_Thread(self, client):
		while True:
			msg = client.recv(BUFFER_SZ)
			if msg:
				self.broadcast(msg, client)
			else:
				client.close()
				try:
					self.clients.remove(client)
				except ValueError:
					pass

				break

	def broadcast(self, msg, client):
		for sock in self.clients:
			sock.send(msg)



top_thread = Thread(target=Chat_Server, args=(sys.argv[1], 5)).start()
top_thread.join()



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
		#this part sets up socket addr and port
		self.ssoc = S.socket(S.AF_INET, S.SOCK_STREAM)
		self.ssoc.bind(("127.0.0.1", int(port)))
		self.ssoc.listen(max_clients)
		self.clients = []
		self.client_addresses = []
		self.BUFFER_SZ = 500
		print("Server started on port %s" % port)
		print("Host name: %s" %S.gethostname())

	def handle_New_Conn(self):
		while True:#spawns a new thread for every connection after handling
			   #the initial setup
			print("Accepting Connections!!!")
			client, client_address = self.ssoc.accept()#handle client connecting
			print("{} has connected".format(client_address))
			self.clients.append(client)#list of all clients for repeating msg
			self.client_addresses.append(client_address) #collect ips 
			client.send("connected!".encode())#all transmissions have 
							  #to be in bytes
			nt = self.client_Thread #get function alias
			Thread(target=nt, args=(client,)).start()

	def client_Thread(self, client):
		while True:
			msg = client.recv(self.BUFFER_SZ)#if you recieve 0 bytes
							 #then client dc'ed
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
			sock.send(msg)#  inc message is byte encoded no need to encode


cs = Chat_Server(sys.argv[1], 5)
top_thread = Thread(target=cs.handle_New_Conn)
top_thread.start()
top_thread.join()



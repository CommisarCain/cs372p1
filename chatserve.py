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
		self.name = S.gethostname()
		self.threads = []
		self.running = True
		print("Server started on port %s" % port)	
		print("Host name: %s" %S.gethostname())

	def chatter(self):
		conn = Thread(target=self.handle_New_Conn)
		self.threads.append(conn)
		conn.start()
		while True:
			msg = input(self.name + ">")
			if msg == "/quit":
				self.quitAll()
				break
			self.broadcast((self.name + ">" + msg).encode(), None)
			

	def handle_New_Conn(self):
		self.ssoc.settimeout(0.5)
		while self.running:#spawns a new thread for every connection after handling
			   #the initial setup
			#print("Accepting Connections!!!")
			try:	
				client, client_address = self.ssoc.accept()#handle client connecting
			except:
				client = None
			if client:	
				#print("{} has connected".format(client_address))
				self.clients.append(client)#list of all clients for repeating msg
				self.client_addresses.append(client_address) #collect ips 
				client.send("connected!".encode())#all transmissions have 
							  #to be in bytes
				nt = self.client_Thread #get function alias
				conn = Thread(target=nt, args=(client,))
				self.threads.append(conn)
				conn.start()

	def client_Thread(self, client):
		while self.running:
			try:
				msg = client.recv(self.BUFFER_SZ)#if you recieve 0 bytes
								 #then client dc'ed
			except:
				try:
					client.close()
					self.clients.remove(client)
					break
				except ValueError:
					pass

			if msg:
				print(" ")
				print(msg.decode())
				print(self.name +">", end="", flush=True)
				
				self.broadcast(msg, client)
			else:
				client.close()
				try:
					self.clients.remove(client)
				except ValueError:
					pass

				break

	def broadcast(self, msg, client):
		#print(msg)
		for sock in self.clients:
			if sock != client:
				sock.send(msg)#  inc message is byte encoded no need to encode

	def quitAll(self):
		self.running = False

cs = Chat_Server(sys.argv[1], 5)
top_thread = Thread(target=cs.chatter)
top_thread.start()
top_thread.join()



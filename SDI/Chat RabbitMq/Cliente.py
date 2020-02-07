#!/usr/bin/env python
import pika
import sys
from threading import Thread
import os

nome = input("Digite o seu nome:")
cont_recebido = 0

class T(Thread):

	def __init__ (self, channel_saida_, nome_):
		Thread.__init__(self)
		self.channel_saida = channel_saida_
		self.nome = nome_
		self.cont_enviado = 0
		self.cont_recebido = 0

	def run(self):cd 
		envia_saida(self.channel_saida, self.nome, self.cont_enviado)
 
def main():

	cont_enviado = 0

	print("\nPara enviar uma mensagem digite o nome do arquivo:\n")

	#connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
	connection = pika.BlockingConnection(pika.ConnectionParameters(
        host='sdi-1',
        credentials=pika.PlainCredentials(username="sdi", password="sdi")))
	channel_saida = connection.channel()
	channel_saida.exchange_declare(exchange='saida', exchange_type='fanout')

	channel_entrada = connection.channel()
	channel_entrada.exchange_declare(exchange='entrada', exchange_type='fanout')
	result = channel_entrada.queue_declare('', exclusive=True)
	queue_name = result.method.queue
	channel_entrada.queue_bind(exchange='entrada', queue=queue_name)

	channel_entrada.basic_consume(callback, queue=queue_name, no_ack=True)

	t = T(channel_saida, nome)
	t.start()

	channel_entrada.start_consuming()


def callback(ch, method, properties, body):
	message = body.decode().split("&", 2)
	if(message[0]!=nome):
		n_arq = nome+"-"+message[0]+"-"+message[1]+".client"
		arq = open(n_arq, 'w')
		arq.write(message[2])
		print("Recebi uma mensagem de " +message[0]+". Salvei no arquivo: %r" % n_arq)
		print("\nPara enviar uma mensagem digite o nome do arquivo:")

def envia_saida(channel_saida, nome, cont_enviado):
	
	while(True):
		n_arquivo = input("")
		if os.path.isfile(n_arquivo):
			arq = open(n_arquivo, 'r')
			message = arq.read()
			message = nome+"&"+str(cont_enviado)+"&"+message
			cont_enviado+=1
			channel_saida.basic_publish(exchange='saida', routing_key='', body=message)
			print("Enviei a mensagem do arquivo "+n_arquivo+".\n\nPara enviar uma mensagem digite o nome do arquivo:")
		else:
			print("Nao existe arquivo com esse nome.\n\nPara enviar uma mensagem digite o nome do arquivo:")

#connection.close()

if __name__ == "__main__":
    main()
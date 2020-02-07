#SERVIDOR
#!/usr/bin/env python
import pika
import sys

#connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
connection = pika.BlockingConnection(pika.ConnectionParameters(
        host='sdi-1',
        credentials=pika.PlainCredentials(username="sdi", password="sdi")))

channel_send = connection.channel()
channel_send.exchange_declare(exchange='entrada', exchange_type='fanout')

def main():
	channel_receive = connection.channel()
	channel_receive.exchange_declare(exchange='saida', exchange_type='fanout')
	result = channel_receive.queue_declare('', exclusive=True)
	queue_name = result.method.queue
	channel_receive.queue_bind(exchange='saida', queue=queue_name)

	channel_send = connection.channel()
	channel_send.exchange_declare(exchange='entrada', exchange_type='fanout')

	channel_receive.basic_consume(callback, queue=queue_name, no_ack=True)
	channel_receive.start_consuming()

def callback(ch, method, properties, body):
    envio(channel_send, body)

def envio(channel_send, message):
	texto = message.decode().split("&", 2)
	n_arq = texto[0]+"-"+texto[1]+".serv"
	arq = open(n_arq, 'w')
	arq.write(texto[2])
	print(texto[0]+" enviou uma mensagem para sua fila de saida.\nO servidor salvou no arquivo: %r" % n_arq)
	print("O servidor enviou para a fila de entrada de todos.\n\n")
	channel_send.basic_publish(exchange='entrada', routing_key='', body=message)

#connection.close()

if __name__ == "__main__":
    main()
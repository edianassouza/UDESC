import json
import requests 
from dict import dict

class Catracas():
	def __init__(self, host_, port_, id_, tipo_, registers_):
		self.host = host_
		self.port = port_
		self.id = id_
		self.tipo = tipo_;
		self.registers = registers_

	def register(self, value):
		self._registers = value


	def getRegisters(self):
		response = requests.get("http://"+str(self.host)+":"+str(self.port)+"/getRegisters")
		if response:
			print("\nVoce recebeu a lista de todas as pessoas que pode usar sua catraca. Para verificar, acesse: 'http://localhost:5000/getRegisters'")
			json_response = response.json()	
			self.registers = json_response
			return True
		elif response:
			print('Voce ainda nao recebeu a lista de todas as pessoas que pode usar sua catraca. Estamos tentando resolver isso para voce. Em breve voce podera usar sua catraca')
			return False

	def novoAcesso(self, acesso):
		response = requests.post("http://"+str(self.host)+":"+str(self.port)+"/novosAcessos", json=acesso)
		if response:
			print('Servidor atualizado sobre os novos acessos na catraca')
		elif response:
			print('Servidor não recebeu a atualizacao sobre os novos acessos na catraca')
		

	def verificandoEntrada(self, identificadorEntrada):
		a = str(self.tipo)
		for i in self.registers['registers']:
			if (i['id']==identificadorEntrada) and (i['tipo']==a):
				return True
		return False

	def retornoRegistro(self, identificadorEntrada):
		for i in self.registers['registers']:
			if (i['id']==identificadorEntrada):
				return i

class CatracaProf(Catracas):
	def __init__ (self, host_, port_, id_, tipo_, registers_):
		super().__init__(host_, port_, id_, tipo_, registers_)

class CatracaAluno(Catracas):
	def __init__ (self, host_, port_, id_, tipo_, registers_):
		super().__init__(host_, port_, id_, tipo_, registers_)
	
class CatracaServidor(Catracas):
	def __init__ (self, host_, port_, id_, tipo_, registers_):
		super().__init__(host_, port_, id_, tipo_, registers_)

def main():
	CatracasList = []
	auxiliar = True

	print("\nSeja bem vindo ao sistema de catraca!\n")
	print("Em qualquer momento, enquanto este estiver executando, voce pode abrir seu navegador para consultar o retorno de acessos\n")
	print("Se voce acessar: 'http://localhost:5000/TodosAcessos', vai obter um registro de todos os acessos que todos os tipos de catracas obtiveram\n")
	print("\n")
	print("Vamos iniciar sua catraca!\n")


	while(auxiliar != False):
		tipo = int(input("Digite qual tipo de catraca deseja: 1(professor), 2(servidor), 3(aluno): \n"))
		identificador = input("Digite o numero identificador que voce deseja para a catraca (tres digitos numericos): \n")
		if (tipo==1):
			c_prof = CatracaProf("localhost", 5000, identificador, 1, None)
			if (c_prof.getRegisters()):
				auxiliar=False
				break
		if (tipo==2):
			c_serv = Catracas("localhost", 5000, identificador, 2, None)
			if (c_serv.getRegisters()):
				auxiliar=False
				break
		if (tipo==3):
			c_alu = Catracas("localhost", 5000, identificador, 3, None)
			if (c_alu.getRegisters()):
				auxiliar=False
				break

	auxiliar = True

	print("\n")
	print("Vamos iniciar a insercao de entradas na catraca. \n")

	while(auxiliar != False):
		identificadorEntrada = input("Digite o SEU identificador de entrada (nao é o da catraca, mas sim de cada pessoa: \n")
		if (tipo==1):
			if (c_prof.verificandoEntrada(identificadorEntrada)):
				print("Acesso liberado\n")
				c_prof.novoAcesso(c_prof.retornoRegistro(identificadorEntrada))
			else:
				print("Não foi possivel liberar seu acesso. Tente novamente")
		if (tipo==2):
			if (c_serv.verificandoEntrada(identificadorEntrada)):
				print("Acesso liberado")
				c_serv.novoAcesso(c_serv.retornoRegistro(identificadorEntrada))
			else:
				print("Não foi possivel liberar seu acesso. Tente novamente")
		if (tipo==3):
			if (c_alu.verificandoEntrada(identificadorEntrada)):
				print("Acesso liberado")
				c_alu.novoAcesso(c_alu.retornoRegistro(identificadorEntrada))
			else:
				print("Não foi possivel liberar seu acesso. Tente novamente")
		continuar = int(input("\n\nSe deseja continuar inserindo, digite 0. Se deseja parar, digite 1.\n"))
		if (continuar==1):
			auxiliar = False


if __name__ == "__main__":
	main()
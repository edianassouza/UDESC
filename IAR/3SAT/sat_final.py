import numpy as np
import matplotlib.pyplot as plt
import random as rd
import math
import csv
from scipy.constants import k

#ARQUIVO 1
# NUM_CLAUSULAS = 91
# NUM_VARIAVEIS = 20

#ARQUIVO 2
# NUM_CLAUSULAS = 430
# NUM_VARIAVEIS = 100

#ARQUIVO 3
NUM_CLAUSULAS = 1065
NUM_VARIAVEIS = 250

ITERACOES = 250000
CLAUSULAS = []
RESULTADO_CLAUSULA = []

class Clausula():
	def __init__ (self, var1_, var2_, var3_):
		self.var1 = var1_
		self.var2 = var2_
		self.var3 = var3_

def main():
	global ITERACOES, CLAUSULAS
	RESULTS_SA = []
	RESULTS_RS = []
	#execucao = 0, avalicao = 0

	#lendo arquivo de entrada
	with open("arquivo3.txt") as file:
		for line in file:
			var1, var2, var3, zero = line.split()
			clausula = Clausula(int(var1), int(var2), int(var3))
			CLAUSULAS.append(clausula)

	
	print("Simulated Annealing - Iterações")
	i = 1
	while(i<=1):
		print(i)
		simulatedAnnealing(i, RESULTS_SA)
		i +=1
	print("Melhor resultado = " + str(max(RESULTS_SA)))
	plota_grafico(RESULTS_SA)
	
	# print("Random Search - Iterações")
	# i = 1
	# while(i<=10):
	# 	print(i)
	# 	randomSearch(i, RESULTS_RS)
	# 	i +=1
	# plota_grafico(RESULTS_RS)


def randomSearch(i, RESULTS_RS):

	s_out = gera_solucao_inicial()
	s_out_FO = testa_variaveis(s_out)
	x = 0
	while (x<250000):
		s = gera_solucao_inicial()
		s_fo = testa_variaveis(s)

		if(i==1):
			RESULTS_RS.append(s_fo)
		else:
			if RESULTS_RS[x] < s_fo:
				RESULTS_RS[x] = s_fo

		if (s_fo > s_out_FO):
			s_out = s
			s_out_FO = s_fo
		
		x+=1

	return s_out


def simulatedAnnealing(i, RESULTS_SA):

	solucao = gera_solucao_inicial()
	maxInteracoes = 0
	iterT = 0
	T = 10
	energiaAtual = testa_variaveis(solucao)
	energiaSeguinte = 0

	while (maxInteracoes < 250000):
		while(iterT<7):
			
			iterT+=1
			
			solucao = gerarVizinho(solucao)
			energiaSeguinte = testa_variaveis(solucao)

			delta = energiaAtual - energiaSeguinte
			if (delta<0):
				energiaAtual = energiaSeguinte
			else:
				x = rd.random()
				prob = math.exp(-delta/(T)) #k*T zera tudo
				if (x<prob):
					energiaAtual = energiaSeguinte

			if(i==1):
				RESULTS_SA.append(energiaAtual)
			else:
				if RESULTS_SA[maxInteracoes] < energiaAtual:
					RESULTS_SA[maxInteracoes] = energiaAtual

			maxInteracoes+=1
		
		T = resfriamento1(maxInteracoes)
		iterT=0

	return solucao

def gera_csv(name,data):
    with open(name, mode='w') as file:
    	writer = csv.writer(file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    	
    	i = 1
    	for x in data: 
    		writer.writerow([str(i), str(x)])


def resfriamento1(maxInteracoes):
    t0 = float(10)
    tn = 0.00001

    ti = t0 * ((tn/t0)**(maxInteracoes/float(250000)))
    return ti

def gerarVizinho(data):
	x = np.random.randint(0, NUM_VARIAVEIS)
	data[x] = not data[x]
	return data


def gera_solucao_inicial():

	return np.random.choice([False, True], NUM_VARIAVEIS)

def testa_variaveis(solucao):

	global CLAUSULAS, RESULTADO_CLAUSULA
	soma=0
	num = 0

	for i in range(0, NUM_CLAUSULAS):

		#significa que é negativo (precisa usar not)
		if CLAUSULAS[i].var1 < 0:
			#recebe o numero da clausula sem sinal
			num = abs(CLAUSULAS[i].var1)
			#-1 porque comeca em 1
			var1 = not solucao[num-1]
		#se nao for negativo	
		else:
			num = CLAUSULAS[i].var1
			var1 = solucao[num-1]
		if CLAUSULAS[i].var2 < 0:
			#recebe o numero da clausula sem sinal
			num = abs(CLAUSULAS[i].var2)
			#-1 porque comeca em 1
			var2 = not solucao[num-1]
		#se nao for negativo	
		else:
			num = CLAUSULAS[i].var2
			var2 = solucao[num-1]
		if CLAUSULAS[i].var3 < 0:
			#recebe o numero da clausula sem sinal
			num = abs(CLAUSULAS[i].var3)
			#-1 porque comeca em 1
			var3 = not solucao[num-1]
		#se nao for negativo	
		else:
			num = CLAUSULAS[i].var3
			var3 = solucao[num-1]

		resultado = var1 or var2 or var3


		if resultado == True:
			soma+= 1
		
	return soma
			
def plota_grafico(data):
	array = np.array(data)
	eixo_x = range(len(array))
	plt.plot(eixo_x, array)
	plt.show()

if __name__ == "__main__":
    main()
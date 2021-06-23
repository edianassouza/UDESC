import numpy as np
import random

POP = 10
TAM_CROMO = 10

def gera_binario():
    populacao = []
    linha = []

    for i in range(0, POP):
        linha = []
        for j in range(0, TAM_CROMO):
            linha.append(random.randint(0, 1))
        populacao.append(linha)
    print(np.matrix(populacao))

def gera_inteiro():
    populacao = []
    linha = []

    for i in range(0, POP):
        linha = []
        for j in range(0, TAM_CROMO):
            linha.append(random.randint(-5, 10))
        populacao.append(linha)
    print(np.matrix(populacao))

def gera_real():
    populacao = []
    linha = []

    for i in range(0, POP):
        linha = []
        for j in range(0, TAM_CROMO):
            linha.append(random.uniform(-10, 10))
        populacao.append(linha)
    print(np.matrix(populacao))

def gera_inteiro_permutado():
    populacao = []
    linha = []
    marcados = np.zeros(TAM_CROMO+1, dtype = int)
    contador = 0

    for i in range(0, POP):
        linha = []
        marcados = np.zeros(TAM_CROMO+1, dtype = int)
        contador = 0
        while contador < TAM_CROMO:
            candidato = random.randint(1, TAM_CROMO)
            if(marcados[candidato] == 0):
                marcados[candidato] = 1
                contador+=1
                linha.append(candidato)
        populacao.append(linha)
    print(np.matrix(populacao))

#gera_binario()
#gera_inteiro()
#gera_real()
gera_inteiro_permutado()

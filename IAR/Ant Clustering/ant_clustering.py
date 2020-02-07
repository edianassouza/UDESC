import sys
import pygame
import numpy as np
import random as rd
import math

# Define colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
# Define sizes
WIDTH = 10
HEIGHT = 10
MARGIN = 0
# Define number of cells, grid and size of the window
NUMBER_CELLS = 60
GRID = np.zeros([NUMBER_CELLS, NUMBER_CELLS])
WINDOW_SIZE = [600, 600] #largura, altura
SCREEN = pygame.display.set_mode(WINDOW_SIZE)

NUMBER_ALIVE = 125
NUMBER_DEAD = 1000
NUMBER_MOVEMENTS = 0
VISION_RADIUS = 1
TIME = 100000
###

class Ant():
	def __init__(self, row_, column_):
		self.load = False
		self.row = row_
		self.column = column_
		self.last_row = -1
		self.last_column = -1
		self.div_neighbors = (VISION_RADIUS * 2 + 1) ** 2 - 1

	def move(self):
		global NUMBER_MOVEMENTS
		self.last_row = self.row
		self.last_column = self.column

		while True:
			row = rd.randint(-1, 1) + self.row
			column = rd.randint(-1, 1) + self.column
			if row >= 0 and row < NUMBER_CELLS and column >=0 and column < NUMBER_CELLS and row != self.last_row and column != self.last_column:
				break
		self.row = row
		self.column = column
		NUMBER_MOVEMENTS += 1

	def count_neighbors(self):
		global VISION_RADIUS
		neighbors = 0
		for i in range(self.row-VISION_RADIUS, self.row+VISION_RADIUS+1):
			for j in range(self.column-VISION_RADIUS, self.column+VISION_RADIUS+1):
				if i >= 0 and i < NUMBER_CELLS and j >=0 and j < NUMBER_CELLS:
					if GRID[i][j] == 1:
						neighbors += 1
		return neighbors

	def probability_drop(self):
		global VISION_RADIUS
		neighbors = self.count_neighbors()
		result = neighbors/self.div_neighbors
		result = result + 0.01 if result <= 0.5 else result - 0.01
		return rd.random() < result

	def probability_catch(self):
		global VISION_RADIUS
		neighbors = self.count_neighbors()
		result = neighbors/self.div_neighbors
		result = result + 0.01 if result <= 0.5 else result - 0.01
		return rd.random() < 1 - result

	def catch(self):
		if self.probability_catch():
			GRID[self.row][self.column] = 0
			self.load = True
			return True
		return False

	def drop(self):
		if self.probability_drop():
			GRID[self.row][self.column] = 1
			self.load = False
			return True
		return False

	def drop_anywhere(self):
		while(GRID[self.row][self.column] == 1):
			self.move()
		GRID[self.row][self.column] == 1
		self.load = False

### FUNCTIONS ENVIRONMENT

def draw_grid(row, column, color):
	pygame.draw.rect(SCREEN,
		color,
		[(MARGIN + WIDTH) * column + MARGIN,
		(MARGIN + HEIGHT) * row + MARGIN,
		WIDTH,
		HEIGHT]);

def draw_ant(ant):
	if ant.load:
		draw_grid(ant.row, ant.column, RED)
	else:
		draw_grid(ant.row, ant.column, BLUE)

def inicialize_grid():
	for row in range(NUMBER_CELLS):
		for column in range(NUMBER_CELLS):
			if GRID[row][column] == 1:
				draw_grid(row, column, GREEN)
			else:
				draw_grid(row, column, WHITE)

def close_window():
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			return True
	return False

def create_itens():
	for i in range(NUMBER_DEAD):
		while True:
			row = rd.randint(0, NUMBER_CELLS -1)
			column = rd.randint(0, NUMBER_CELLS - 1)
			if GRID[row][column] == 0:
				break
		GRID[row][column] = 1

def create_ants_return_anthill():
	anthill = []
	for i in range(NUMBER_ALIVE):
		row = rd.randint(0, NUMBER_CELLS -1)
		column = rd.randint(0, NUMBER_CELLS - 1)
		ant = Ant(row, column)
		anthill.append(ant)
		draw_ant(ant)
	return np.array(anthill)

def ants_behaviour(anthill):
	for ant in anthill:
		if GRID[ant.row][ant.column] == 0:
			if ant.load:
				ant.drop()
		elif GRID[ant.row][ant.column] == 1:
			if not ant.load:
				ant.catch()
		ant.move()
	return anthill


def count_isolated_dead_ants():
	count = 0
	for row in range(NUMBER_CELLS):
		for column in range(NUMBER_CELLS):
			if GRID[row][column] == 1 and verify_isoleted_dead_ant(row, column):
				count += 1
	return count

def verify_isoleted_dead_ant(row, column):
	neighbors = 0
	for i in range(row-1, row+2):
		for j in range(column-1, column+2):
			if i >= 0 and i < NUMBER_CELLS and j >=0 and j < NUMBER_CELLS:
				if GRID[i][j] == 1 and not equal_position(i, j, row, column):
					neighbors += 1
	return neighbors == 0

def equal_position(i, j, row, column):
	return i == row and j == column

def main():

	global VISION_RADIUS
	global TIME

	if len(sys.argv) > 2:
		if not int(sys.argv[1]) <= 5 or not int(sys.argv[1]) > 0:
			print("O raio de visão deve ser uma número positivo menor ou igual 5")
			return
		VISION_RADIUS = int(sys.argv[1])
		if not int(sys.argv[2]) > 0:
			print("O tempo deve ser um inteiro positivo")
			return
		TIME = int(sys.argv[2]) * 60000

	pygame.init()
	pygame.display.set_caption("Ants Clustering Initial")
	clock = pygame.time.Clock()
	flag = True
	meio = False

	create_itens()
	inicialize_grid()
	anthill = create_ants_return_anthill()
	pygame.image.save(SCREEN, "initial.jpeg")
	endtime = pygame.time.get_ticks() + TIME

	while not close_window():
		if(pygame.time.get_ticks()<endtime):
			anthill = ants_behaviour(anthill)
			if int(pygame.time.get_ticks()) > int(endtime/2) and not meio:
				pygame.display.set_caption("Ants Clustering Meio")
				inicialize_grid()
				for ant in anthill:
					draw_ant(ant)
				pygame.image.save(SCREEN, "meio.jpeg")
				meio = True
		elif flag:
			pygame.display.set_caption("Ants Clustering Final")
			inicialize_grid()
			for ant in anthill:
				if ant.load == True:
					ant.drop_anywhere()
				draw_ant(ant)
			print("Tempo máximo atingido: " + str(TIME/60000) + " minutos")
			print("Número de passos: " + str(NUMBER_MOVEMENTS))
			print("Número de formigas mortas isoladas: " + str(count_isolated_dead_ants()))
			print("Feche a tela para finalizar o programa")
			pygame.image.save(SCREEN, "final.jpeg")
			flag = False
		clock.tick(60)
		pygame.display.flip()
	pygame.quit()

if __name__ == "__main__":
    main()

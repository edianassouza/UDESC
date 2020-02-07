import pandas as pd
import numpy as np
import pygame
import csv
import random as rd
import math
import time
from graphviz import Digraph, render

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
BROWN = (139,69,19)
PINK = (255, 0, 127)
YELLOW = (255, 255, 0)
PURPLE = (142, 36, 170)
COLORS = [GREEN, BROWN, BLUE, RED, PINK, YELLOW, WHITE]
TERRAIN_COST = [1, 5, 10, 15]
TOTAL_TOOLS = 22
DEPTH = 0

WIDTH = 15
HEIGHT = 15
MARGIN = 1
NUMBER_CELLS = 42
VISION_RADIUS = 4

ROBOT_X = 0
ROBOT_Y = 0

PATHS = 0
DOT = Digraph(comment='Path' + str(PATHS))
NODES = []
LABEL = 0

class Cell():
	def __init__(self, x_, y_, terrain_):
		self.terrain = terrain_
		self.tool = -1
		self.factory = -1
		self.x = x_
		self.y = y_
		self.cost = TERRAIN_COST[terrain_]

	def color_cell(self):
		paint_terrain(self.x, self.y, self.terrain, self.factory)
		if self.factory != -1:
			if factory_repair[self.factory]:
				paint_factory(self.x, self.y, self.factory, True)
			paint_factory(self.x, self.y, self.factory, False)
		elif self.tool != -1:
			paint_tool(self.x, self.y, self.tool)

######

NUMBER_CELLS = 42
GRID = np.empty((NUMBER_CELLS, NUMBER_CELLS), dtype=Cell)
WINDOW_SIZE = [973, 673] #largura, altura
SCREEN = pygame.display.set_mode(WINDOW_SIZE)

######

class Robot():
	def __init__(self):
		self.x = ROBOT_X
		self.y = ROBOT_Y
		self.load = [0,0,0,0,0]
		self.last_x = -1
		self.last_y = -1
		self.total_cost = 0

	def move(self):

		while True:
			x = rd.randint(-1, 1) + self.x
			y = rd.randint(-1, 1) + self.y
			if x >= 0 and x < NUMBER_CELLS and y >=0 and y < NUMBER_CELLS and same_axis(self.x, self.y, x, y) and not equals_positions(self.last_x, self.last_y, x, y) and not equals_positions(self.x, self.y, x, y) and not is_obstacle(x, y):
				break
		self.last_x = self.x
		self.last_y = self.y
		self.x = x
		self.y = y

		self.total_cost += GRID[x][y].cost

	def localize_tool(self):
		global VISION_RADIUS
		tools_found = []
		for i in range(self.x-VISION_RADIUS, self.x+VISION_RADIUS+1):
			for j in range(self.y-VISION_RADIUS, self.y+VISION_RADIUS+1):
				if i >= 0 and i < NUMBER_CELLS and j >=0 and j < NUMBER_CELLS:
					c = GRID[i][j] 
					if c.tool != -1:
						tools_found.append((c.tool, i, j))
		return tools_found

	def closest_tool(self, tools):
		#indice da ferramenta, x, y
		t = tools[0]
		d = dist_manhattan(self.x, self.y, t[1], t[2])
		min_dist = (t, d)
		
		for t in tools:
			d = dist_manhattan(self.x, self.y, t[1], t[2])
			if d < min_dist[1]:
				min_dist = (t, d)

		return min_dist[0]

	def robot_behaviour(self):

		global TOTAL_TOOLS
		update_grid(self.x, self.y)
		tools_found = self.localize_tool()
		if(len(tools_found) == 0):
			self.move()
			update_grid(self.x, self.y)
		else:
			x = self.closest_tool(tools_found)
			if(self.verify_tool_necessary(x[0])):
				path = a_star(self.x, self.y, x[1], x[2])
				self.move_path(path)
				c = GRID[self.x, self.y]

				if(c.tool != -1):
					#pega a ferramenta
					self.load[c.tool] += 1
					TOTAL_TOOLS -= 1
					index_tool = c.tool
					c.tool = -1
					time.sleep(5)

					if(self.verify_has_all_tools(index_tool)):
						update_grid(self.x, self.y)	
						f = factory_positions[index_tool]
						path = a_star(self.x, self.y, f[0], f[1])
						self.move_path(path)
						factory_repair[index_tool] = True
						time.sleep(5)
								
			else:	
				self.move()
			


	def reverse_path(self, path):
		return [x for x in reversed(path)] 

	def move_path(self, path):
		path_cost = 0
		path = self.reverse_path(path)
		paint_robot(self.x, self.y)
		len_ = 0
		for a,b in path:
			self.last_x = self.x
			self.last_y = self.y
			self.x = a
			self.y = b
			c = GRID[self.last_x][self.last_y]
			paint_robot(self.x, self.y)
			self.total_cost += GRID[a][b].cost
			path_cost += GRID[a][b].cost
			len_ += 1
		draw_path_info(path_cost, len(path))
		i = path[0]
		f = path[len_ - 1]
		paint_init_final(i[0], i[1], f[0], f[1])
		pygame.image.save(SCREEN, 'captura' + str(PATHS-1) + '.jpeg')

	def verify_tool_necessary(self, t):
		x = tools[t]
		return factory_repair[t] == False and self.load[t] < x[1]

	def verify_has_all_tools(self, t):
		x = tools[t]
		return self.load[t] == x[1]

	def reverse_path(self, path):
		return [x for x in reversed(path)] 

	def status(self):
		draw_status(self.total_cost, self.load)

######

#Functions environment

def inicialize_grid():
	data = pd.read_csv('ambiente2.csv', sep=',',header=None)
	
	for row in range(0, NUMBER_CELLS):
		for column in range(0,NUMBER_CELLS):
			i = int(data.iloc[row,column])
			GRID[row][column] = Cell(row, column, i)

	inicialize_positions()
	inicialize_tools()

	for row in range(0, NUMBER_CELLS):
		for column in range(0,NUMBER_CELLS):
			c = GRID[row][column]
			c.color_cell()

def update_grid(robot_x, robot_y):
	for row in range(0, NUMBER_CELLS):
		for column in range(0,NUMBER_CELLS):
			c = GRID[row][column]
			c.color_cell()
	paint_robot(robot_x,robot_y)

def close_window():
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			return True
	return False

def inicialize_positions():
	i = 0
	with open('posicoes.csv') as csvfile:
		readCSV = csv.reader(csvfile, delimiter=",")
		for row in readCSV:
			if i == 5:
				ROBOT_X = int(row[1])
				ROBOT_Y = int(row[2])
				break
			c = GRID[int(row[1])][int(row[2])]
			c.factory = int(row[0])
			c.terrain = -1
			c.cost = 1
			factory_positions.append((int(row[1]),int(row[2])))
			i += 1

def inicialize_tools():
	index = 0
	for a, b in tools:
		for i in range(a):
			while(True):
				row = rd.randint(0, NUMBER_CELLS -1)
				column = rd.randint(0, NUMBER_CELLS - 1)
				c = GRID[row][column]
				if(c.terrain == 0 and c.factory == -1 and c.tool == -1):
					break
			c.tool = index
		index += 1

def all_factories_repaired():
	for s in factory_repair:
		if not s:
			return False
	return True

def is_obstacle(x, y):
	return GRID[x][y].terrain == -1 and GRID[x][y].factory == -1

#Functions draw

def paint_tool(x, y, index):
	rec_x = (MARGIN + WIDTH) * y + MARGIN + 7
	rec_y = (MARGIN + HEIGHT) * x + MARGIN + 7
	pygame.draw.circle(SCREEN, COLORS[index+2], (rec_x,rec_y), 4)

def paint_terrain(x, y, index, factory):
	if index == -1 and factory != -1:
		color = WHITE #Tem uma fábrica no lugar
	elif index == -1:
		color = BLACK
	else:
		color = COLORS[index] 
	pygame.draw.rect(SCREEN,
		color,
		[(MARGIN + WIDTH) * y + MARGIN,
		(MARGIN + HEIGHT) * x + MARGIN,
		WIDTH,
		HEIGHT]);

def paint_factory(x, y, index, status):
	rec_x = (MARGIN + WIDTH) * y + MARGIN + 2
	rec_y =(MARGIN + HEIGHT) * x + MARGIN
	if status:
		l = BASICFONT.render(str(index), True, GREEN)
	else:
		l = BASICFONT.render(str(index), True, RED)
	SCREEN.blit(l,(rec_x,rec_y))

def paint_init_final(xi, yi, xf, yf):
	rec_xi = (MARGIN + WIDTH) * yi + MARGIN + 2
	rec_yi =(MARGIN + HEIGHT) * xi + MARGIN
	rec_xf = (MARGIN + WIDTH) * yf + MARGIN + 2
	rec_yf =(MARGIN + HEIGHT) * xf + MARGIN
	l = BASICFONT.render('I', True, BLACK)
	SCREEN.blit(l,(rec_xi,rec_yi))
	l = BASICFONT.render('F', True, BLACK)
	SCREEN.blit(l,(rec_xf,rec_yf))

def paint_robot(x, y):
	pygame.draw.rect(SCREEN,
		YELLOW,
		[(MARGIN + WIDTH) * y + MARGIN,
		(MARGIN + HEIGHT) * x + MARGIN,
		WIDTH,
		HEIGHT]);

def draw_status(total_cost, tools_loaded):

	surf = pygame.Surface((300, 673))
	surf.fill((0, 0, 0))
	rect = surf.get_rect()
	SCREEN.blit(surf, (673, 0))
	
	text = ['Custo total: ' + str(total_cost),
		'',
		'Ferramentas restantes: ' + str(TOTAL_TOOLS),
		'Bateria de carga elétrica: ' + str(tools_loaded[0]),
		'Braços de solda:' + str(tools_loaded[1]), 
		'Bombas de sucção:' + str(tools_loaded[2]),
		'Dispositivo de refrigeração:' + str(tools_loaded[3]), 
		'Braço pneumático:' + str(tools_loaded[4]),
		'',
		'Estados das fábricas',
		'Fábrica 0: ' + status_factory(0),
		'Fábrica 1: ' + status_factory(1),
		'Fábrica 2: ' + status_factory(2),
		'Fábrica 3: ' + status_factory(3),
		'Fábrica 4: ' + status_factory(4)]
		
	label = []
	for line in text:
		label.append(BASICFONT.render(line, True, WHITE))

	i = 1
	for l in label:
		SCREEN.blit(l,(700, 24*i))
		if(i>3 and i<9):
			pygame.draw.circle(SCREEN, COLORS[i-2], (690,24*i+10), 4)
		i += 1

def draw_path_info(path_cost, size):
	
	surf = pygame.Surface((300, 373))
	surf.fill((0, 0, 0))
	rect = surf.get_rect()
	SCREEN.blit(surf, (673, 373))
	
	text = ['Caminho ' + str(PATHS), 
			'Custo do caminho: ' + str(path_cost),
			'Profundidade: ' + str(DEPTH),
			'Tamanho do caminho:' + str(size)]
		
	label = []
	for line in text:
		label.append(BASICFONT.render(line, True, WHITE))

	i = 20
	for l in label:
		SCREEN.blit(l,(700, 24*i))
		i += 1

def draw_finished():
	
	FONT = pygame.font.Font('freesansbold.ttf', 25)

	surf = pygame.Surface((300, 150))
	surf.fill((0, 0, 0))
	rect = surf.get_rect()
	SCREEN.blit(surf, (673, 500))
	
		
	l = FONT.render('Programa finalizado', True, WHITE)

	SCREEN.blit(l,(700, 24*24))

def status_factory(index):
	if factory_repair[index]:
		return "Consertada"
	return "Em manutenção"

def paint_border(x, y, color):
	pygame.draw.rect(SCREEN,
		color,
		[(MARGIN + WIDTH) * y + MARGIN,
		(MARGIN + HEIGHT) * x + MARGIN,
		WIDTH,
		HEIGHT]);

class Node():

	def __init__(self, father_, x_, y_, label_):
		self.x = x_
		self.y = y_
		self.father = father_
		self.g = GRID[x_][y_].cost
		self.h = 0
		self.f = 0
		self.neighboors = []
		self.label = label_
		self.depth = 0

	def define_cost(self, g, h, f):
		self.g = g
		self.h = h
		self. f = f

	def define_neighboors(self):
		global LABEL

		if(verify_valid_position(self.x, self.y+1)):
			n1 = Node(self, self.x, self.y+1, LABEL)
			n1.depth = self.depth + 1
			LABEL += 1
			self.neighboors.append(n1)
		if(verify_valid_position(self.x-1, self.y)):
			n2 = Node(self, self.x-1, self.y, LABEL)
			n2.depth = self.depth + 1
			LABEL += 1
			self.neighboors.append(n2)
		if(verify_valid_position(self.x+1, self.y)):
			n3 = Node(self, self.x+1, self.y, LABEL)
			n3.depth = self.depth + 1
			LABEL += 1
			self.neighboors.append(n3)
		if(verify_valid_position(self.x, self.y-1)):
			n4 = Node(self, self.x, self.y-1, LABEL)
			n4.depth = self.depth + 1
			LABEL += 1
			self.neighboors.append(n4)

	def node_already_in_data(self, data):
		for n in data:
			if(n.x == self.x and n.y == self.y and n.f <= self.f):
				return True
		return False

def add_tree(n, double):

	global NODES

	if double:
		DOT.attr('node', shape='doublecircle')
	else:
		DOT.attr('node', shape='circle')
		
	name = str(n.y) + ', ' + str(n.x) + ', ' + str(n.g) + ', ' + str(n.h) + ', ' + str(n.f)
	DOT.node(str(n.label), name)
	NODES.append((str(n.label), str(n.father.label)))

def draw_tree():
	for n in NODES:
		DOT.edge(n[1], n[0])

def dist_euclidean(x1, y1, x2, y2):
	return math.sqrt(pow(x1-x2, 2) + pow(y1-y2, 2))

def dist_manhattan(x1, y1, x2, y2):
	return abs(x1-x2) + abs(y1-y2)

def return_node_min_cost(data):
	node = None
	for x in data:
		if(node == None or x.f < node.f):
			node = x
	return node

def equals_positions(x1, y1, x2, y2):
	return x1 == x2 and y1 == y2

def same_axis(x1, y1, x2, y2):
	return x1 == x2 or y1 == y2

def verify_valid_position(x, y):
	return x < NUMBER_CELLS and x >= 0 and y < NUMBER_CELLS and y >= 0

def print_path(node):
	print("x = " + str(node.x) + " e y = " + str(node.y))
	while node.father != None:
		print("x = " + str(node.father.x) + " e y = " + str(node.father.y))
		node = node.father

def return_path(node):
	path = []
	path.append((node.x, node.y))
	while node.father != None:
		path.append((node.father.x, node.father.y))
		node = node.father
	return path

def a_star(x, y, obj_x, obj_y):
	
	global PATHS, DOT, NODES, LABEL

	DOT = Digraph(comment='Path' + str(PATHS))
	DOT.body.extend(['rankdir=TB'])

	q = Node(None, x, y, LABEL)
	LABEL += 1
	DOT.attr('node', shape='circle')
	name = str(q.y) + ', ' + str(q.x) + ', ' + str(q.g) + ', ' + str(q.h) + ', ' + str(q.f)
	DOT.node(str(q.label), name)

	open_ = []
	closed_ = []

	open_.append(q)

	while(len(open_) != 0):
		q = return_node_min_cost(open_)
		open_.remove(q)
		q.define_neighboors()
		
		for sucessor in q.neighboors:
			
			if is_obstacle(sucessor.x, sucessor.y):
			 	continue

			if(equals_positions(sucessor.x, sucessor.y, obj_x, obj_y)):
				
				#preciso adicionar o nó final
				sucessor.g = sucessor.g + sucessor.father.g
				sucessor.h = dist_manhattan(sucessor.x, sucessor.y, obj_x, obj_y)
				sucessor.f = sucessor.g + sucessor.h

				add_tree(sucessor, True)

				draw_tree()
				title = 'caminho' + str(PATHS) 
				DOT.render(title, view=False)
				PATHS += 1
				NODES = []
				DEPTH = sucessor.depth

				return return_path(sucessor)

			sucessor.g = sucessor.g + sucessor.father.g
			sucessor.h = dist_manhattan(sucessor.x, sucessor.y, obj_x, obj_y)
			sucessor.f = sucessor.g + sucessor.h

			add_tree(sucessor, False)

			if(sucessor.node_already_in_data(open_) or sucessor.node_already_in_data(closed_)):
				continue
			paint_border(sucessor.x, sucessor.y, WHITE)
			open_.append(sucessor)
		closed_.append(q)


def main():

	global BASICFONT, tools, factory_repair, factory_positions

	tools = [(20, 8), (10, 5), (8, 2), (6, 5), (4, 2)] #environment_have/factory_need
	factory_repair = [False, False, False, False, False]
	factory_positions = []

	pygame.init()
	BASICFONT = pygame.font.Font('freesansbold.ttf', 17)
	pygame.display.set_caption("Robot")
	clock = pygame.time.Clock()
	inicialize_grid()
	robot = Robot()
	paint_robot(robot.x, robot.y)

	pygame.image.save(SCREEN, 'inicio.jpeg')
	while not close_window():
		robot.status()
		if not all_factories_repaired():
			robot.robot_behaviour()
		else:
			draw_finished()
		clock.tick(60)
		pygame.display.flip()
	pygame.quit()

if __name__ == "__main__":
    main()
#-*-python-*-
from BaseAI import BaseAI
from GameObject import *
import random
import math
import heapq

class Cell(object):
	def _init_(self,x,y,parent,reachable)
		self.x = x
		self.y = y
		self.parent = parent
		self.reachable = reachable
		self.g = 0
		self.h = 0
		self.f = 0

class Astar(object):
	def _init_(self,creature,target):
		self.open = []
		self.closed = set()
		self.cells = []
		self.gridHeight = AI.mapHeight
		self.gridWidth = AI.mapWidth
		heapq.heapify(self.op)
		
	def init_grid(self):
		lifeforms = AI.plants+AI.creatures
		walls = [[lifeform.x,lifeform.y] for lifeform in lifeforms]
		for x in range(AI.gridWidth):
			for y in range(AI.gridHeight):
				if (x,y) in walls:
					reachable = False
				else:
					reachable = True
				self.cells.append(Cell(x,y,reachable))
				self.start = self.get_cell(creature.x,creature.y)
				self.end = self.get_cell(target.x,target.y)
	
	def get_heuristic(self,cell):
		return 10*(abs(cell.x-self.end.x)+abs(cell.y-self.end.y))
		
	def get_cell(self,x,y):
		return self.cell[x*AI.mapHeight+y]

	def get_adjacent_cells(self,cell):
		cells = []
		if cell.x < AI.gridWidth-1:
			cells.append(self.get_cell(x+1,cell.y))
		if cell.y > 0:
			cells.append(self.get_cell(cell.x,cell.y-1))
		if cell.x>0:
			cells.append(self.get_cell(cell.x-1,cell.y))
		if cell.y<AI.gridHeight-1:
			cells.append(self.get_cell(cell.x,cell.y+1))
		return cell
		
class Grid(object):
		def _init_(self):
 		  grid = [[0 for ii in range(self.mapWidth)] for iii in range(self.mapHeight)]
		  lifeforms = self.plants+self.creatures
			for lifeform in lifeforms:
				grid[lifeform.x][lifeform.y]=1	


class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "Shell AI"

  @staticmethod
  def password():
    return "password"

  ##This function is called once, before your first turn
  def init(self):
    pass

  ##This function is called once, after your last turn
  def end(self):
    pass

  def getObject(self,x,y):
    target = 1
    lifeforms = self.creatures+self.plants
    for lifeform in lifeforms:
      if lifeform.x == x and lifeform.y == y:
        target = lifeform
    return target
        
  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):   
    adjacent = [[1,0],[-1,0],[0,1],[0,-1]]
    for creature in self.creatures:
      if creature.owner == self.playerID:
        randx=random.randrange(-1,2); 
        randy = abs(randx)^1*((-1)**random.randrange(1,100)%6)
        x=0;y=0
        if isinstance(self.getObject(creature.x+randx,creature.y+randy),int) and (0<creature.x+randx<self.mapWidth) and (0<creature.y+randy<self.mapHeight):
          creature.move(creature.x+randx,creature.y+randy)
          x=creature.x+randx;
          y=creature.y+randy
        for location in adjacent:
          thing=self.getObject(x+location[0],y+location[1])
          if isinstance(thing,Plant):
            creature.eat(thing.x,thing.y)
          if isinstance(thing,Creature):
            if thing.owner==self.playerID:
              creature.breed(thing,thing.x,thing.y)
            else:
              creature.eat(thing.x,thing.y)
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)


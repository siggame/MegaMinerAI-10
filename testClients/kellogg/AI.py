#-*-python-*-
from BaseAI import BaseAI
from GameObject import *
import random
import math
import heapq

class Cell:
  def __init__(self,x,y,reachable):
   self.x = x
   self.y = y
   self.parent = None
   self.reachable = reachable
   self.g = 0
   self.h = 0
   self.f = 0

class Astar:
  def __init__(self,creatureX,creatureY,dest,lifeforms,gridHeight,gridWidth):
    self.open = []
    self.closed = set()
    self.cells = []
    heapq.heapify(self.open)
    self.dest=dest
    self.creatureX = creatureX
    self.creatureY = creatureY
    self.lifeforms = lifeforms
    self.gridHeight = gridHeight
    self.gridWidth= gridWidth
    	
  def init_grid(self):
    walls = [(lifeform.x,lifeform.y) for lifeform in self.lifeforms]
    for x in range(self.gridWidth):
      for y in range(self.gridHeight):
  	if (x,y) in walls:
          reachable = False
        else:
          reachable = True
        self.cells.append(Cell(x,y,reachable))
    self.start = self.get_cell(self.creatureX,self.creatureY)
    self.end = self.get_cell(self.dest[0],self.dest[1])
	
  def get_h(self,cell):
    return 10*(abs(cell.x-self.end.x)+abs(cell.y-self.end.y))		
  
  def get_cell(self,x,y):
    return self.cells[(x-1)*self.gridHeight+(y-1)]

  def get_adjacent(self,cell):
    cells = []
    if cell.x < self.gridWidth-1:
	cells.append(self.get_cell(cell.x+1,cell.y))
    if cell.y > 0:
    	cells.append(self.get_cell(cell.x,cell.y-1))
    if cell.x>0:
	cells.append(self.get_cell(cell.x-1,cell.y))
    if cell.y<self.gridHeight-1:
	cells.append(self.get_cell(cell.x,cell.y+1))
    return cells

  def get_path(self):
    cell = self.end
    path = []
    while cell is not self.start:
      path.append(cell)
      cell=cell.parent
    return path

  def update_cell(self,adj,cell):
    adj.g = cell.g+10
    adj.h = self.get_h(adj)
    adj.parent = cell
    adj.f = adj.h + adj.g
		
  def makePath(self):
    heapq.heappush(self.open,(self.start.f,self.start))
    path = []
    while len(self.open):
      f,cell = heapq.heappop(self.open)
      self.closed.add(cell)
      if cell is self.end:
	print "GO TO THE END OF THE PATH"
	return self.get_path()
	#break
      adj_cells = self.get_adjacent(cell)
      for adCell in adj_cells:
        if adCell.reachable and adCell not in self.closed:
	  if (adCell.f,adCell) in self.open:
	    if adCell.g > cell.g+10:
	      self.update_cell(adCell,cell)
          else:
	    self.update_cell(adCell,cell)
	    heapq.heappush(self.open,(adCell.f,adCell))
  #  return path	
		
class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "Kellogg"

  @staticmethod
  def password():
    return "password"

  ##This function is called once, before your first turn
  def init(self):
    pass
 
  ##This function is called once, after your last turn
  def end(self):
    pass

  def moveTo(self,creature,end):
    Apath = Astar(creature.x,creature.y,end,self.plants+self.creatures,self.mapHeight,self.mapWidth)
    Apath.init_grid()
    path = Apath.makePath()
#    print "---------Creature,plant--------",[(creature.x,creature.y,creature.id),(end[0],end[1])]
    print"-----path----",[(cell.x,cell.y,cell.parent.x,cell.parent.y) for cell in path]
	
  def distance(self,sourceX,sourceY,destX,destY):
    return int(math.sqrt((sourceX-destX)**2+(sourceY-destY)**2))
    
  def maxStat(self,creature):
    return max([creature.herbivorism,creature.carnivorism,creature.speed,creature.energy,creature.defense])
#TODO FIX    
  def findMate(self,creature):
      mateList = [mate for mate in self.creatures if mate.owner == creature.owner and mate.id != creature.id]
      if len(mateList)>0:
        mate = min(mateList,key = lambda x:self.distance(x.x,x.y,creature.x,creature.y))
        mate = [min({self.distance(creature.x,mate.x,creature.y,mate.y):mate} for mate in mateList)]
        print "mate is ",mate
        
  def getObject(self,x,y):
    lifeforms = self.creatures+self.plants
    return [lifeform for lifeform in lifeforms if lifeform.x == x and lifeform.y == y]
    
     ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):   
    adjacent = [[1,0],[-1,0],[0,1],[0,-1]]    
    herbivores = [creature for creature in self.creatures if creature.owner == self.playerID and creature.herbivorism == self.maxStat(creature)]  
    carnivores = [creature for creature in self.creatures if creature.owner == self.playerID and creature.carnivorism == self.maxStat(creature)] 

    for creature in self.creatures:
     if creature.owner == self.playerID:             
      randx=random.randrange(-1,2); 
      randy = abs(randx)^1*((-1)**random.randrange(1,100)%2)
      x=0;y=0
      if self.getObject(creature.x+randx,creature.y+randy) is None and (0<creature.x+randx<self.mapWidth) and (0<creature.y+randy<self.mapHeight):
        creature.move(creature.x+randx,creature.y+randy)
      for location in adjacent:
        thingList=self.getObject(creature.x+location[0],creature.y+location[1]); thing = None
        if len(thingList)>0:
          thing = thingList[0]
        if isinstance(thing,Plant) and thing.size>0 and creature.canEat:
         creature.eat(thing.x,thing.y)
        if isinstance(thing,Creature):
          if thing.owner==self.playerID and thing.currentHealth > self.healthPerBreed and creature.currentHealth > self.healthPerBreed:
            creature.breed(thing)
          elif creature.canEat:
            creature.eat(thing.x,thing.y)
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)


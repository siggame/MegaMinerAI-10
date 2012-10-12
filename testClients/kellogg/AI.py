#-*-python-*-
from BaseAI import BaseAI
from GameObject import *
import random

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

  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):
    
    lifeforms = self.creatures+self.plants
    #left,right,up,down
    adjacent = [[1,0],[-1,0],[0,1],[0,-1]]
    grid = [[None]*self.getMapHeight() for _ in range(self.getMapWidth())]
    for lifeform in lifeforms:
      grid[lifeform.x][lifeform.y] = lifeform    
        
    for creature in self.creatures:
      if creature.owner == self.playerID:
        randx=random.randrange(-1,1); randy=random.randrange(-1,1)
        grid[lifeform.x][lifeform.y] = None
        creature.move(creature.x+randx,creature.y+randy) 
        grid[lifeform.x+randx][lifeform.y+randy] = lifeform
        for location in adjacent:
          thing = grid[location[0]][location[1]]
          if isinstance(thing,Plant):
            creature.eat(thing.x,thing.y)
          elif isinstance(thing,Creature):
            if thing.owner==self.playerID:
              creature.breed(thing,thing.x,thing.y)
            else:
              creature.eat(thing.x,thing.y)
      print self.getTurnNumber()
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)

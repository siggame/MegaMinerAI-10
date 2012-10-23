#-*-python-*-
from BaseAI import BaseAI
from GameObject import *
import random
import math

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
              creature.breed(thing)
            else:
              creature.eat(thing.x,thing.y)
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)


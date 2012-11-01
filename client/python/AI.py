#-*-python-*-
from BaseAI import BaseAI
from GameObject import *

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
    for creature in self.creatures:
     if creature.owner == self.playerID:
      plantIn = self.getPlantAtLocation(creature.x+1,creature.y)
      if (plantIn == -1 or (plantIn!= -1 and self.plants[plantIn].size==0)) and self.getCreatureAtLocation(creature.x+1,creature.y)==-1:
        if (0<=creature.x+1<self.mapWidth) and (0<=creature.y<self.mapHeight) and creature.currentHealth>=self.healthPerMove and creature.movementLeft>0:
          creature.move(creature.x+1,creature.y)
      plantIn = self.getPlantAtLocation(creature.x+1,creature.y)
      creatIn = self.getCreatureAtLocation(creature.x+1,creature.y)
      if creature.canEat:
        if plantIn != -1 and self.plants[plantIn].size>0 and creature.currentHealth >= self.healthPerMove:
          creature.eat(self.plants[plantIn].x,self.plants[plantIn].y)
        elif creatIn != -1 and self.creatures[creatIn].owner != self.playerID and creature.currentHealth>= self.healthPerMove:
          creature.eat(self.creatures[creatIn].x,self.creatures[creatIn].y)
      elif creatIn != -1 and self.creatures[creatIn].owner == self.playerID:
        if  creature.currentHealth>self.healthPerBreed and self.creatures[creatIn].currentHealth>self.healthPerBreed:
          if creature.canBreed and self.creatures[creatIn].canBread:
            creature.breed(self.creatures[creatIn])
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)

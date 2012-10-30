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
    mine = [creature for creature in self.creatures if creature.owner == self.playerID]
    for creature in mine:
      if not isinstance(self.getPlantAtLocation(creature.x+1,creature.y),Plant) and not isinstance(self.getCreatureAtLocation(creature.x+1,creature.y),Creature):
        if (0<creature.x+1<self.mapWidth) and (0<creature.y<self.mapHeight) and creature.energyLeft>self.energyPerAction:
          creature.move(creature.x+1,creature.y)
      plant = self.getPlantAtLocation(creature.x+1,creature.y)
      creat = self.getCreatureAtLocation(creature.x+1,creature.y)
      if creature.canEat:
        if isinstance(plant,Plant) and plant.size>0 and creature.energyLeft >= self.energyPerAction:
          creature.eat(plant.x,plant.y)
        elif isinstance(creat,Creature) and creat.owner != self.playerID and creature.energyLeft >= self.energyPerAction:
          creature.eat(creat.x,creat.y)
      elif isinstance(creat,Creature) and creat.owner == self.playerID:
        if  creature.energyLeft>=self.energyPerBreed and creat.energyLeft>=self.energyPerBreed:
          if creature.canBreed and creat.canBread:
            creature.breed(creat)
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)

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
    #Constructs a list of creatures that belong to me
    mine = [creature for creature in self.creatures if creature.owner == self.playerID]
    
    #For each of the creatures I control
    for creature in mine:
      #Checks to see what is in the tile to the right of the creature
      if not isinstance(self.getPlantAtLocation(creature.x+1,creature.y),Plant) and not isinstance(self.getCreatureAtLocation(creature.x+1,creature.y),Creature):
        #If nothing is there, we move to the tile 
        if (0 < creature.x+1 < self.mapWidth) and (0 < creature.y < self.mapHeight) and creature.currentHealth > self.healthPerMove:
          creature.move(creature.x+1,creature.y)
      plant = self.getPlantAtLocation(creature.x+1,creature.y)
      creat = self.getCreatureAtLocation(creature.x+1,creature.y)
      
      if creature.canEat:
        if isinstance(plant,Plant) and plant.size > 0:
          creature.eat(plant.x,plant.y)
        elif isinstance(creat,Creature) and creat.owner != self.playerID:
          creature.eat(creat.x,creat.y)
          
      elif isinstance(creat,Creature) and creat.owner == self.playerID:
        if  creature.currentHealth >= self.energyPerBreed and creat.currentHealth >= self.healthPerBreed:
          if creature.canBreed and creat.canBread:
            creature.breed(creat)
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)

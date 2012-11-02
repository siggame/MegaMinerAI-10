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
    #iterate through every creature 
    for creature in self.creatures:
     #if I own the creature
     if creature.owner == self.playerID:
      #check if there is a plant to the creature's left
      plantIn = self.getPlantAtLocation(creature.x+1,creature.y)
      #if there is no plant, or there is a plant and its size is 0, and there is no creature to its left
      if (plantIn == -1 or (plantIn!= -1 and self.plants[plantIn].size==0)) and self.getCreatureAtLocation(creature.x+1,creature.y)== -1:
        #if x is in the range of the map, and y is in range of the map
        if (0<=creature.x+1<self.mapWidth) and (0<=creature.y<self.mapHeight):
          #check creature has enough energy tomove, and has movement left
          if creature.currentHealth>self.healthPerMove and creature.movementLeft>0:
            #move creature to the left
            creature.move(creature.x+1,creature.y)

      #check if there is a plant to the left
      plantIn = self.getPlantAtLocation(creature.x+1,creature.y)
      #check if there is a creature to the left
      creatIn = self.getCreatureAtLocation(creature.x+1,creature.y)
      #if there is a plant to my left, and its size is > 0
      if plantIn != -1 and self.plants[plantIn].size>0 and creature.canEat:
        #eat the plant, using its x and y
        creature.eat(self.plants[plantIn].x,self.plants[plantIn].y)
      #else if there is a creature to my left, and it is not my creature
      elif creatIn != -1 and self.creatures[creatIn].owner != self.playerID and creature.canEat:
        #take a bite out of the creature
        creature.eat(self.creatures[creatIn].x,self.creatures[creatIn].y)
      #else if there is a creature to my left, and it is my creature
      elif creatIn != -1 and self.creatures[creatIn].owner == self.playerID:
        #if both creatures have enough health to breed, and have not bred this turn
        if  creature.currentHealth>self.healthPerBreed and self.creatures[creatIn].currentHealth>self.healthPerBreed:
          if creature.canBreed and self.creatures[creatIn].canBreed:
            #breed with this creature
            creature.breed(self.creatures[creatIn])
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)

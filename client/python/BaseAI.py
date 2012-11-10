# -*- python -*-

from library import library

class BaseAI:
  """@brief A basic AI interface.

  This class implements most the code an AI would need to interface with the lower-level game code.
  AIs should extend this class to get a lot of builer-plate code out of the way
  The provided AI class does just that.
  """
  #\cond
  initialized = False
  iteration = 0
  runGenerator = None
  connection = None
  #\endcond
  mappables = []
  creatures = []
  plants = []
  players = []
  #\cond
  def startTurn(self):
    from GameObject import Mappable
    from GameObject import Creature
    from GameObject import Plant
    from GameObject import Player

    BaseAI.mappables = [Mappable(library.getMappable(self.connection, i)) for i in xrange(library.getMappableCount(self.connection))]
    BaseAI.creatures = [Creature(library.getCreature(self.connection, i)) for i in xrange(library.getCreatureCount(self.connection))]
    BaseAI.plants = [Plant(library.getPlant(self.connection, i)) for i in xrange(library.getPlantCount(self.connection))]
    BaseAI.players = [Player(library.getPlayer(self.connection, i)) for i in xrange(library.getPlayerCount(self.connection))]

    if not self.initialized:
      self.initialized = True
      self.init()
    BaseAI.iteration += 1;
    if self.runGenerator:
      try:
        return self.runGenerator.next()
      except StopIteration:
        self.runGenerator = None
    r = self.run()
    if hasattr(r, '__iter__'):
      self.runGenerator = r
      return r.next()
    return r
  #\endcond
  #\cond
  def getTurnNumber(self):
    return library.getTurnNumber(self.connection)
  #\endcond
  turnNumber = property(getTurnNumber)
  #\cond
  def getPlayerID(self):
    return library.getPlayerID(self.connection)
  #\endcond
  playerID = property(getPlayerID)
  #\cond
  def getGameNumber(self):
    return library.getGameNumber(self.connection)
  #\endcond
  gameNumber = property(getGameNumber)
  #\cond
  def getMapWidth(self):
    return library.getMapWidth(self.connection)
  #\endcond
  mapWidth = property(getMapWidth)
  #\cond
  def getMapHeight(self):
    return library.getMapHeight(self.connection)
  #\endcond
  mapHeight = property(getMapHeight)
  #\cond
  def getHealthPerBreed(self):
    return library.getHealthPerBreed(self.connection)
  #\endcond
  healthPerBreed = property(getHealthPerBreed)
  #\cond
  def getHealthPerMove(self):
    return library.getHealthPerMove(self.connection)
  #\endcond
  healthPerMove = property(getHealthPerMove)
  #\cond
  def getHealthPerTurn(self):
    return library.getHealthPerTurn(self.connection)
  #\endcond
  healthPerTurn = property(getHealthPerTurn)
  #\cond
  def getBaseHealth(self):
    return library.getBaseHealth(self.connection)
  #\endcond
  baseHealth = property(getBaseHealth)
  def __init__(self, connection):
    self.connection = connection
    
  #If there is a plant at location (x,y), returns the index of that plant.
  #If there is no plant there, returns -1.    
  def getPlantAtLocation(self,x,y):
    for plant in self.plants:
      if plant.x == x and plant.y == y:
        return self.plants.index(plant)
    return -1
    
  #If there is a creature at location (x,y), returns the index of that creature.
  #If there is no creature there, returns -1.
  def getCreatureAtLocation(self,x,y):
    for creature in self.creatures:
      if creature.x == x and creature.y == y:
        return self.creatures.index(creature)
    return -1

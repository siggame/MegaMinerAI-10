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
  def getEnergyPerBreed(self):
    return library.getEnergyPerBreed(self.connection)
  #\endcond
  energyPerBreed = property(getEnergyPerBreed)
  #\cond
  def getEnergyPerAction(self):
    return library.getEnergyPerAction(self.connection)
  #\endcond
  energyPerAction = property(getEnergyPerAction)
  #\cond
  def getEnergyPerTurn(self):
    return library.getEnergyPerTurn(self.connection)
  #\endcond
  energyPerTurn = property(getEnergyPerTurn)
  def __init__(self, connection):
    self.connection = connection

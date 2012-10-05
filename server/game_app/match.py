from base import *
from matchUtils import *
from objects import *
import networking.config.config
from collections import defaultdict
from networking.sexpr.sexpr import *
import os
import itertools
import scribe
import math
import random

Scribe = scribe.Scribe

def loadClassDefaults(cfgFile = "config/defaults.cfg"):
  cfg = networking.config.config.readConfig(cfgFile)
  for className in cfg.keys():
    for attr in cfg[className]:
      setattr(eval(className), attr, cfg[className][attr])

class Match(DefaultGameWorld):
  def __init__(self, id, controller):
    self.id = int(id)
    self.controller = controller
    DefaultGameWorld.__init__(self)
    self.scribe = Scribe(self.logPath())
    self.addPlayer(self.scribe, "spectator")

    #TODO: INITIALIZE THESE!
    self.turnNumber = -1
    self.playerID = -1
    self.gameNumber = id
    self.mapWidth = self.MapWidth
    self.mapHeight = self.MapHeight

  #initializes a game map
  def initGrid(self):
	self.grid = [[None]*self.mapHeight for _ in range(self.mapWidth)]
	for creature in self.objects.creatures:
		self.grid[creature.x][creature.y] = creature
	for plant in self.objects.plants:
		self.grid[plant.x][plant.y] = plant
		
  def getObject(self, x, y):
	return self.grid[x][y]
  
  def addPlayer(self, connection, type="player"):
    connection.type = type
    if len(self.players) >= 2 and type == "player":
      return "Game is full"
    if type == "player":
      self.players.append(connection)
      try:
        self.addObject(Player, [connection.screenName, self.startTime])
      except TypeError:
        raise TypeError("Someone forgot to add the extra attributes to the Player object initialization")
    elif type == "spectator":
      self.spectators.append(connection)
      #If the game has already started, send them the ident message
      if (self.turn is not None):
        self.sendIdent([connection])
    return True

  def removePlayer(self, connection):
    if connection in self.players:
      if self.turn is not None:
        winner = self.players[1 - self.getPlayerIndex(connection)]
        self.declareWinner(winner, 'Opponent Disconnected')
      self.players.remove(connection)
    else:
      self.spectators.remove(connection)

  def start(self):
    if len(self.players) < 2:
      return "Game is not full"
    if self.winner is not None or self.turn is not None:
      return "Game has already begun"
    
    #TODO: START STUFF
    self.turn = self.players[-1]
    self.turnNumber = -1

    #this is for testing if a plant should be made
    def makePlant(self,x,y):
      x1 = self.MapWidth
      x2 = x
      y1 = self.MapHeight
      y2 = y
      distance = math.sqrt((x1-x2)**2 + (y1-y2)**2)
      x2 = 0
      y2 = 0
      totaldistance = math.sqrt((x1-x2)**2 + (y1-y2)**2)
      prob = (1-distance/totaldistance)*self.plantModifier
      #prob is 0 to 1, make a plant if greater
      if random.random() > prob:
        toBeReturned = random.uniform(1,(6-6*(distance/totaldistance)))
        if toBeReturned == 0:
          toBeReturned = 1
        toBeReturned = math.floor(toBeReturned)
        return toBeReturned
      return -1
      
    plantsx = 0
    while plantsx < self.MapWidth:
      plantsy = 0
      while plantsy < self.MapHeight:
        checkMakePlant = makePlant(self,plantsx,plantsy)
        if not (checkMakePlant == -1):
          #FENERATE [generate] a plant here with checkMake Plant size.
          #[Mainly what to pass for game and id is unclear.]
          #I attempt this with the following:
          self.addObject(Plant,[plantsx,plantsy,checkMakePlant])
          #also mirror this plant [y stays the same, plantsx=mapSize-plantsx]

        plantsy += 1

      plantsx += 1

    self.nextTurn()
    return True


  def nextTurn(self):
    self.turnNumber += 1
    if self.turn == self.players[0]:
      self.turn = self.players[1]
      self.playerID = 1
    elif self.turn == self.players[1]:
      self.turn = self.players[0]
      self.playerID = 0

    else:
      return "Game is over."

    for obj in self.objects.values():
      obj.nextTurn()

    self.checkWinner()
    if self.winner is None:
      self.sendStatus([self.turn] +  self.spectators)
    else:
      self.sendStatus(self.spectators)
    self.animations = ["animations"]
    return True

  def checkWinner(self):
    #TODO: Make this check if a player won, and call declareWinner with a player if they did
    if self.turnNumber >= self.turnLimit:
       self.declareWinner(self.players[0], "Because I said so, this shold be removed")
    

  def declareWinner(self, winner, reason=''):
    print "Player", self.getPlayerIndex(self.winner), "wins game", self.id
    self.winner = winner

    msg = ["game-winner", self.id, self.winner.user, self.getPlayerIndex(self.winner), reason]
    self.scribe.writeSExpr(msg)
    self.scribe.finalize()
    self.removePlayer(self.scribe)

    for p in self.players + self.spectators:
      p.writeSExpr(msg)

    self.sendStatus([self.turn])
    self.playerID ^= 1
    self.sendStatus([self.players[self.playerID]])
    self.playerID ^= 1
    self.turn = None
    
  def logPath(self):
    return "logs/" + str(self.id) + ".glog"

  @derefArgs(Creature, None, None)
  def move(self, object, x, y):
    return object.move(x, y, )

  @derefArgs(Creature, None, None)
  def eat(self, object, x, y):
    return object.eat(x, y, )

  @derefArgs(Creature, Creature, None, None)
  def breed(self, object, mate, x, y):
    return object.breed(mate, x, y, )

  @derefArgs(Player, None)
  def talk(self, object, message):
    return object.talk(message, )


  def sendIdent(self, players):
    if len(self.players) < 2:
      return False
    list = []
    for i in itertools.chain(self.players, self.spectators):
      list += [[self.getPlayerIndex(i), i.user, i.screenName, i.type]]
    for i in players:
      i.writeSExpr(['ident', list, self.id, self.getPlayerIndex(i)])

  def getPlayerIndex(self, player):
    try:
      playerIndex = self.players.index(player)
    except ValueError:
      playerIndex = -1
    return playerIndex

  def sendStatus(self, players):
    for i in players:
      i.writeSExpr(self.status())
      i.writeSExpr(self.animations)
    return True


  def status(self):
    msg = ["status"]

    msg.append(["game", self.turnNumber, self.playerID, self.gameNumber, self.mapWidth, self.mapHeight])

    typeLists = []
    typeLists.append(["Creature"] + [i.toList() for i in self.objects.values() if i.__class__ is Creature])
    typeLists.append(["Plant"] + [i.toList() for i in self.objects.values() if i.__class__ is Plant])
    typeLists.append(["Player"] + [i.toList() for i in self.objects.values() if i.__class__ is Player])

    msg.extend(typeLists)

    return msg


loadClassDefaults()


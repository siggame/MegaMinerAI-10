from base import *
from matchUtils import *
from objects import *
import networking.config.config
from collections import defaultdict
from networking.sexpr.sexpr import *
import os
import itertools
import scribe

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
    self.turnNumber = None
    self.playerID = None
    self.gameNumber = id
    self.mapWidth = None
    self.mapHeight = None
  
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

    #TODO: Better formula? [uses cosine currently]
    plants = 0
    while plants < self.objects.startPlantsPerSide:
      #generates a number 0 to 1 and then multiply be the map size
      plantGenx = (-math.cos(random.uniform(0,math.pi)) + 1) / 2 * self.objects.mapSize
      plantGeny = (-math.cos(random.uniform(0,math.pi)) + 1) / 2 * self.objects.mapSize
      #get a better thing for distance [actually based on distance]
      plantSize = (-math.cos(random.uniform(0,math.pi)) + 1) / 2 * self.objects.plantMaxSize
      #I don't know if this is generating a plant correctly [what do I put for ID?]
      self.game.addObject(Plant,[self, self.game, 0, plantGenx, plantGeny, plantSize])
      #mirror this object
      self.game.addObject(Plant,[self, self.game, 0, self.objects.mapSize/2 - plantGenx, self.objects.mapSize/2 - plantGeny, plantSize])
      plants += 1
    self.game.addObject

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


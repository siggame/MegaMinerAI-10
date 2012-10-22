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

    self.turnNumber = -1
    self.playerID = -1
    self.gameNumber = id
    self.mapWidth = self.mapWidth
    self.mapHeight = self.mapHeight
    self.energyPerBreed = self.energyPerBreed
    self.energyPerAction = self.energyPerAction
    self.energyPerTurn = self.energyPerTurn  
    self.grid = [[[] for _ in range(self.mapHeight)] for _ in range(self.mapWidth)]

  def getObject(self, x, y):
    if len(self.grid[x][y]) > 0:
      return self.grid[x][y][0]
    else:
      return None  
    
  #this is here to be wrapped
  def __del__(self):
    pass

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
      
  def initialStats(self):
   # Start out with remaining stats equal to the max minus 5 because each stat has minimum of 1
   stats = self.totalStartStats - 5
   count = 0
   list = [1, 1, 1, 1, 1]
   while stats > 0:
      position = count % 5
      #between 1 and 7
      remaining = self.maxStartStat - list[position]
      # Random integer of stats to be added to current number. Can never exceed maxStat
      temp = random.randint(0, stats) % (remaining + 1)
      list[position] += temp
      # Decrementing stats, reflecting how many available stats are left
      stats -= temp
      # Incrementing count so as to step through each position in the list
      count += 1
   
   # First entry is energy, which is a multiple of 10 with a base energy
   list[0] = self.baseEnergy + list[0] * 10
   return list
      
  def makePlant(self,x,y): 
    x1 = self.mapWidth/2
    x2 = x
    y1 = self.mapHeight/2
    y2 = y
    distance = math.sqrt((x1-x2)**2 + (y1-y2)**2)
    x2 = 0
    y2 = 0
    totaldistance = math.sqrt((x1-x2)**2 + (y1-y2)**2)
    prob = (1-distance/totaldistance)*self.plantModifier
    #prob is 0 to 1, make a plant if greater
    randomNum = random.random()
    if randomNum < prob:
      toBeReturned = random.uniform(1,(6-6*(distance/totaldistance)))
      toBeReturned = math.floor(toBeReturned)
      if toBeReturned == 0:
        toBeReturned = 1
      return toBeReturned
    return -1

  def start(self):
    if len(self.players) < 2:
      return "Game is not full"
    if self.winner is not None or self.turn is not None:
      return "Game has already begun"
    
    #TODO: START STUFF
    self.turn = self.players[-1]
    self.turnNumber = -1
     
    plantsx = 0
    while plantsx < math.floor(self.mapWidth / 2):
      plantsy = 0
      while plantsy < self.mapHeight:
        checkMakePlant = self.makePlant(plantsx,plantsy)
        if not (checkMakePlant == -1):
          #Add objects on both players' sides.
          growthRate = math.floor(random.uniform(10, 30))
          turnsUntilGrowth = growthRate
          self.addObject(Plant,[plantsx,plantsy,checkMakePlant, growthRate, turnsUntilGrowth])
          self.addObject(Plant,[self.mapWidth - plantsx -1 ,plantsy, checkMakePlant, growthRate, turnsUntilGrowth])

        plantsy += 1
      plantsx += 1
    for plant in self.objects.plants:
      self.grid[plant.x][plant.y] = [plant]
      
    self.spawnCreatures()
    
    #Display initial game state for debugging
    for creature in self.objects.creatures:
      if creature.owner == 0:
        print "Creature"
        print "--------"
        print "Energy: ", (creature.maxEnergy - 100) / 10
        print "Carnivorism: ", creature.carnivorism
        print "Herbivorism: ", creature.herbivorism
        print "Defense: ", creature.defense
        print "Speed: ", creature.speed 
        print ""        
    
    for creature in self.objects.creatures:
      self.grid[creature.x][creature.y] = [creature]
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
    player1 = self.players[0]
    player2 = self.players[1]
    p1c = sum(creature.owner == self.objects.players[0].id for creature in self.objects.creatures)
    p2c = sum(creature.owner == self.objects.players[1].id for creature in self.objects.creatures)
    if p1c == 0 or p2c == 0 or self.turnNumber >= self.turnLimit:
      if p1c > p2c:
        self.declareWinner(player1, "Player 1 wins through creature domination")
      elif p1c < p2c:
        self.declareWinner(player2, "Player 2 wins through creature domination")
      #Defaults player 1 as winner if both players have same number of creatures at end
      else:
        self.declareWinner(player1, "The game was a tie.")       


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
    self.objects.clear()

  def logPath(self):
    return "logs/" + str(self.id) + ".glog"

  @derefArgs(Creature, None, None)
  def move(self, object, x, y):
    return object.move(x, y, )

  @derefArgs(Creature, None, None)
  def eat(self, object, x, y):
    return object.eat(x, y, )

  @derefArgs(Creature, Creature)
  def breed(self, object, mate):
    return object.breed(mate, )

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

    msg.append(["game", self.turnNumber, self.playerID, self.gameNumber, self.mapWidth, self.mapHeight, self.energyPerBreed, self.energyPerAction, self.energyPerTurn])

    typeLists = []
    typeLists.append(["Mappable"] + [i.toList() for i in self.objects.values() if i.__class__ is Mappable])
    typeLists.append(["Creature"] + [i.toList() for i in self.objects.values() if i.__class__ is Creature])
    typeLists.append(["Plant"] + [i.toList() for i in self.objects.values() if i.__class__ is Plant])
    typeLists.append(["Player"] + [i.toList() for i in self.objects.values() if i.__class__ is Player])

    msg.extend(typeLists)

    return msg
    
  #SpawnCreatures randomly, put team1 on one half of the map and mirror these locations on the other half of the map for team2
  #This function assumes the plants have been spawned symmetrically across the vertical axis
  def spawnCreatures(self):
    i = 0
    while i < self.startingCreatures:
      i+=1
      randStats = self.initialStats()
      # Change order of creature parameters to make this cleaner?
      statList = [randStats[0], randStats[0], randStats[1], randStats[2], randStats[3], randStats[3], randStats[4]]
    #Generate x,y for creature location
      while True:
        newX = int(random.uniform(0,self.mapWidth / 2))
        newY = int(random.uniform(0, self.mapHeight))
      #check map if the space is unoccupied, otherwise generate a new X,Y
        if self.getObject(newX, newY) is None:
          self.addObject(Creature,[newX, newY, 0]+statList+[0])  
          self.addObject(Creature,[(self.mapWidth-newX-1), newY, 1]+statList+[0])
          break          
    #end while
    return


loadClassDefaults()


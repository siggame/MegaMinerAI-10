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
    self.mapWidth = self.mapWidth
    self.mapHeight = self.mapHeight

#initializes a game map
  def initGrid(self):
    self.grid = [[None]*self.mapHeight for _ in range(self.mapWidth)]
    print len(self.grid[0])
    print len(self.grid)
    for creature in self.objects.creatures:
      self.grid[creature.x][creature.y] = creature
      #print 'Creature ' + str(creature.id) + ' added @ ' + str(creature.x) + ',' + str(creature.y)
    for plant in self.objects.plants:
      #print 'Plant ' + str(plant.id) + ' added @ ' + str(plant.x) + ',' + str(plant.y)
      self.grid[plant.x][plant.y] = plant
    print 'Number of plants: ' + str(len(self.objects.plants))

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

   #this is for testing if a plant should be made
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
    print prob
    #prob is 0 to 1, make a plant if greater
    randomNum = random.random()
    print 'Random nunmber: ' + str(randomNum)
    if randomNum < prob:
      toBeReturned = random.uniform(1,(6-6*(distance/totaldistance)))
      if toBeReturned == 0:
        toBeReturned = 1
      toBeReturned = math.floor(toBeReturned)
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
    while plantsx < self.mapWidth:
      plantsy = 0
      while plantsy < self.mapHeight:
        checkMakePlant = self.makePlant(plantsx,plantsy)
        if not (checkMakePlant == -1):
          #Add objects on both players' sides.
          self.addObject(Plant,[plantsx,plantsy,checkMakePlant])
          self.addObject(Plant,[self.mapWidth - plantsx -1,plantsy,checkMakePlant])

        plantsy += 1
      plantsx += 1

    self.initGrid()
    self.nextTurn()
    self.spawnCreatures()
    print len(self.objects.creatures)
    return True
  
  
  def initialStats(self):
   stats = self.totalStartStats
   count = 0
   list = [1, 1, 1, 1, 1]
   while stats > 0:
      position = count % 5
      remaining = self.maxStat - list[position]
      # Random integer of stats to be added to current number. Can never exceed maxStat
      temp = random.randint(0, stats) % (remaining + 1)
      list[position] += temp
      # Decrementing stats, reflecting how many available stats are left
      stats -= temp
      # Incrementing count so as to step through each position in the list
      count += 1
   
   return list
   

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
    #Defaults player 1 as winner if both players have same number of creatures at end
    player1 = self.players[0]
    player2 = self.players[1]
    winner = player1
    # number of player 1's creatures and player 2's creatures
    p1C = 0
    p2C = 0
    
    for c in self.objects.creatures:
      if c.owner == self.objects.players[0].id:
         p1C += 1
      else:
         p2C += 1
         
    if p1C > p2C:
      winner = player1
      if p2C == 0:
         self.declareWinner(winner, "Player 2's creatures are all died")
    elif p2C > p1C:
      winner = player2
      if p1C == 0:
         self.declareWinner(winner, "Player 1's creatures are all died")
    if self.turnNumber >= self.turnLimit:
      self.declareWinner(winner, "Time limit reached")
    

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


#SpawnCreatures randomly, put team1 on one half of the map and mirror these locations on the other half of the map for team2
#This function assumes the plants have been spawned symmetrically across the vertical axis
  def spawnCreatures(self):
    i = 0
    while i < self.startingCreatures:
      i+=1
    #Generate x,y for creature location
      while True:
        newX = int(random.uniform(0,1)*self.mapWidth / 2)
        newY = int(random.uniform(0,1)*self.mapHeight)
      #check map if the space is unoccupied, otherwise generate a new X,Y
        if self.getObject(newX, newY) is None:
          self.addObject(Creature,[0, newX, newY, 1, 1, 1, 1, 1, 1, 1])  
          self.addObject(Creature,[1, (self.mapWidth-newX), newY, 1, 1, 1, 1, 1, 1, 1])  
          break          
    #end while       
    #TODO: call creature stats generator instead of all 1's      
    return


loadClassDefaults()

#-*-python-*-
from BaseAI import BaseAI
from GameObject import *
from operator import itemgetter
import random
import math
import time

energyIndex = 0
carnIndex = 1
herbIndex = 2
defIndex = 3
speedIndex = 4

plantI = 1
CreatI = 2

class tileD(object):
  def __init__(self, x, y, dist):
    self.x = x
    self.y = y
    self.dist = dist

  def __repr__(self):
    return repr((self.x, self.y, self.dist))
    
class Cell(object):
  def __init__(self, x, y):
    self.x = x
    self.y = y

class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "phelps"

  @staticmethod
  def password():
    return "password"
    
  def distance(self,x1,y1,x2,y2):
    return math.sqrt((x2-x1)**2 + (y2-y1)**2)   
  
  def distancel(self, x, y):
    return math.sqrt((y[0]-x[0])**2 + (y[1]-x[1])**2)
  
  def distanceC(self, c1, c2):
    return math.sqrt((c2.x - c1.x)**2 + (c2.y - c1.y)**2)
  
  def init(self): 
    pass
  ##This function is called once, after your last turn
  def end(self):
    pass

  def getGridN(self, x):
    return self.grid[x[0]][x[1]]
  
  def getGrid(self, x, y):
    return self.grid[x][y]
  
  def neighborNodes(self, x, y):
    validNodes = []
    if x+1 < self.mapWidth:
      validNodes.append([x+1, y])
    if y+1 < self.mapHeight:
      validNodes.append([x, y+1])
    if x-1 >= 0:
      validNodes.append([x-1, y])
    if y-1 >= 0:
      validNodes.append([x, y-1])      
    return validNodes  
  
  def validMoves(self, x, y):
    moves = self.neighborNodes(x, y)
    
    for plant in self.plants:
      for cell in moves:
        if plant.x == cell[0] and plant.y == cell[1]:
          moves.remove(cell)
    
    for creature in self.creatures:
      for cell in moves:
        if creature.x == cell[0] and creature.y == cell[1]:
          moves.remove(cell)

    if len(moves) > 0:
      return moves
    else:
      return []
    
  def findDirection(self, startX, startY, goalX, goalY):
    
    s = [startX, startY]
    g = [goalX, goalY]
    
    pMoves = self.validMoves(s[0], s[1])    
    gMoves = [tileD(k[0], k[1], self.distance(k[0], k[1], g[0], g[1])) for k in pMoves]

    #for k in range(len(pMoves)):
    #  gMoves[0][k] = pMoves[k]
    #  gMoves[1][k] = (math.floor(self.distance(pMoves[k][0], pMoves[k][1], g[0], g[1])))  
      
#    dict = {cell:math.floor(self.distance(cell[0], cell[1], g[0], g[1])) for cell in pMoves}
       
   # bestMove = min(dict, key=dict.get)

    gMoves.sort(key=lambda tile: tile.dist)
    
    if len(gMoves) > 0:
      bestMove = [gMoves[0].x, gMoves[0].y]
      return bestMove
    else:
      return []
    
  def findAPathD(self, startX,startY, goalX, goalY, moveC):
    
    start = [startX, startY]
    goal = [goalX, goalY]
    
    closedSet = []
    openSet = [start]
    
    for k in range(moveC):      
      nextMove = self.findDirection(openSet[0][0], openSet[0][1], goal[0], goal[1])
      if len(nextMove) > 0:
        closedSet.append(nextMove)
        openSet.append(nextMove)
      else:
        return closedSet

      openSet.pop(0)

    return closedSet
  
  def findAPath(self, startX, startY, goalX, goalY):
    
    start = [startX, startY]
    goal = [goalX, goalY]
    
    closedSet = []
    openSet = [start]
    
    while len(openSet) > 0:
      if len(openSet) > 20:
        break
      nextMove = self.findDirection(openSet[0][0], openSet[0][1], goal[0], goal[1])
      closedSet.append(nextMove)
      if self.distance(openSet[0][0], openSet[0][1], goal[0], goal[1]) > 3:
        openSet.append(nextMove)
        
      openSet.pop(0)
    return closedSet
  
  def moveToMiddle(self, creature):
    x_mid = math.floor(self.mapWidth/2)
    y_mid = math.floor(self.mapHeight/2)
    
    path = self.findAPathD(creature.x, creature.y, x_mid, y_mid, creature.movementLeft)   
    while len(path) > 0 and creature.movementLeft > 0 and math.floor(self.distance(creature.x, creature.y, x_mid, y_mid)) > 2:
      creature.move(path[0][0], path[0][1])
      path.remove(path[0])
  
    return
  
  def findNearestFriendlyCreatureXY(self, creature):
    #Doesn't work if I only have one creature left
    myCreatures = [creature for creature in self.creatures if creature.owner == self.playerID and creature.currentHealth > 0]
    if len(myCreatures) > 1:    
      #Creates a dictionary of the distances to each creature I control that isn't at the calling creature's position
      dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in myCreatures if target.x != creature.x and creature.y != target.y}    
      #Returns the object that is the closest creature   
      if len(dict) > 0:      
        bestCreature = next(target for target in myCreatures if target.id == min(dict, key=dict.get))
        #Returns the x and y value of that creature's location
        return bestCreature     
    return None
  
  def findNearestEnemyCreatureXY(self, creature):   
    #Creates a dictionary of the distances to each enemy creature
    enemyCreatures = [creature for creature in self.creatures if creature.owner != self.playerID and creature.currentHealth > 0]
    dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in enemyCreatures if target.currentHealth > 0}  
    #Returns the object that is the closest creature      
    if len(dict) > 0:
      bestID = min(dict, key=dict.get)
      bestCreature = next(target for target in enemyCreatures if target.id == bestID)
      #Returns the x and y value of that creature's location
      return bestCreature 
    return None
  
  def findNearestFriendlyX(self, creature, X):
    myCreatures = [creature for creature in self.creatures if creature.owner == self.playerID and creature.currentHealth > 0]
    specializedCreatures = {"carnivorism":[], "herbivorism":[], "speed": []}
    
    for creature in myCreatures:
      stats = {"carnivorism": creature.carnivorism, "herbivorism" : creature.herbivorism}
      specializedCreatures[max(stats, key=stats.get)].append(creature)
    
    creatureList = specializedCreatures[X]
    
    dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in creatureList if target.x != creature.x and creature.y != target.y}
    
    if len(dict) > 0:
      bestCreature = next(target for target in myCreatures if target.id == min(dict, key=dict.get))
      return bestCreature
    return None
  
  def findClosestXAndBreed(self, creature, bestStat):
    father = self.findNearestFriendlyX(creature, bestStat)
    if father is not None:
      path = self.findAPathD(creature.x, creature.y, father.x, father.y, creature.movementLeft)
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, father.x, father.y) > 1 and creature.currentHealth > self.healthPerMove:
        creature.move(path[0][0], path[0][1])
        path.remove(path[0])
      if self.distance(creature.x, creature.y, father.x, father.y) == 1 and creature.canBreed == True and creature.currentHealth > self.healthPerBreed:
        if(len(self.grid[father.x][father.y])):
          creature.breed(self.grid[father.x][father.y][0])
    return

  def findClosestAllyAndBreed(self,creature):
    nearestCreature = self.findNearestFriendlyCreatureXY(creature) 
    if nearestCreature is not None: 
      path = self.findAPathD(creature.x, creature.y, nearestCreature.x, nearestCreature.y, creature.movementLeft)
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) > 1 and creature.currentHealth > self.healthPerMove:
        creature.move(path[0][0], path[0][1])         
        path.remove(path[0])
      if self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) == 1 and creature.canBreed == True and creature.currentHealth > self.healthPerBreed:
        if(len(self.grid[nearestCreature.x][nearestCreature.y])):
          creature.breed(self.grid[nearestCreature.x][nearestCreature.y][0])
    return
  
  def findNearestEdiblePlantXY(self, creature, plantSize):
    #Doesn't work if there are no plants left to eat
    ediblePlants = [target for target in self.plants if target.size > plantSize]
    if len(ediblePlants) > 0:
      #Creates a dictionary of the distances to each plant that has a size > 0
      dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in ediblePlants}       
      #Returns the object that is the closest creature  
      bestID = min(dict, key=dict.get)
      bestPlant = next(target for target in self.plants if target.id == bestID)
      #Returns the x and y value of that plant's location
      
      print "BEST Plant: " + str(bestPlant.x) + ", " + str(bestPlant.y)
      
      return bestPlant
    return None
  
  def findNearestEnemyAndEat(self, creature):
    nearestCreature = self.findNearestEnemyCreatureXY(creature) 
    if nearestCreature is not None:    
      path = self.findAPathD(creature.x, creature.y, nearestCreature.x, nearestCreature.y, creature.movementLeft)
      #Move until we are to the plant or can move no more
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) > 1:
        creature.move(path[0][0], path[0][1])         
        path.remove(path[0])
      if self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) == 1 and creature.canEat == True:
        print "Attacking enemy"
        creature.eat(nearestCreature.x, nearestCreature.y) 
  
  def findAndEatPlant(self, creature):
    plant = self.findNearestEdiblePlantXY(creature, 3) 
    if plant is not None: 
      plant = self.findNearestEdiblePlantXY(creature, 0)
    if plant is not None:
      path = self.findAPathD(creature.x, creature.y, plant.x, plant.y, creature.movementLeft)
      #Move until we are to the plant or can move no more
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, plant.x, plant.y) > 1:
        creature.move(path[0][0], path[0][1])         
        path.remove(path[0])
      #We are next to a plant! Eat it!
      if self.distance(creature.x, creature.y, plant.x, plant.y) == 1 and creature.canEat == True:
        creature.eat(plant.x, plant.y)
    return
  
  def findClosestHerbivoreAndBreed(self, creature):
    
    pass
  
  def herbAction(self, creature):
    x_mid = math.floor(self.mapWidth/2)
    y_mid = math.floor(self.mapHeight/2)
    if creature.currentHealth < math.floor((1.5*creature.maxHealth/3)):
      self.findAndEatPlant(creature)
    elif creature.currentHealth > self.healthPerBreed + 15:
      self.findClosestXAndBreed(creature, "herbivorism")
    if creature.movementLeft > 0:
      self.moveToMiddle(creature)
    return
  
  def carnAction(self, creature):
    x_mid = math.floor(self.mapWidth/2)
    y_mid = math.floor(self.mapHeight/2)
    
    
           
    self.moveToMiddle(creature)
    return
  
  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):
    self.grid = [[[] for _ in range(self.mapHeight)] for _ in range(self.mapWidth)] 
    self.statCreatureList = {"energy": [], "carnivorism": [], "herbivorism": [], "speed": [], "defense": []}  
    
    self.CHcreatureList = {"defense": [], "herbivorism": []}
    
    #fill grid with info     
    for plant in self.plants:
      self.grid[plant.x][plant.y] = [plant]
    for creature in self.creatures:
      self.grid[creature.x][creature.y] = [creature]  
      
      if creature.owner == self.playerID and creature.currentHealth > 0:
        stats = {"energy": creature.energy, "carnivorism": creature.carnivorism, "herbivorism": creature.herbivorism, "defense": creature.defense,"speed": creature.speed} 
        chstats = {"defense" : creature.defense, "herbivorism" : creature.herbivorism}
        self.statCreatureList[max(stats, key=stats.get)].append(creature)      
        self.CHcreatureList[max(chstats, key=chstats.get)].append(creature)
    
    myCreatures = [creature for creature in self.creatures if creature.owner == self.playerID]
        
    for creature in self.CHcreatureList["defense"]:
      self.carnAction(creature)
    
    for creature in self.CHcreatureList["herbivorism"]:
      self.herbAction(creature)
      
    print self.turnNumber
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)
   

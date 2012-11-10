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

class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "williams"

  @staticmethod
  def password():
    return "password"
    
  def distance(self,x1,y1,x2,y2):
    return math.sqrt((x2-x1)**2 + (y2-y1)**2)
    
  def neighborNodes(self,x,y):
    validNodes = []
    if x+1 < self.mapWidth:
      validNodes.append([x+1,y])
    if y+1 < self.mapHeight:
      validNodes.append([x,y+1])
    if x-1 >= 0:
      validNodes.append([x-1,y])
    if y-1 >= 0:
      validNodes.append([x,y-1])
    return validNodes  
    
  def findPath(self, startX, startY, goalX, goalY):   
    closedSet = {}
    #startx, starty, score, pathto
    openSet = [[[startX, startY], self.distance(startX, startY, goalX, goalY), [startX, startY]]]    
    #while we have nodes to iterate
    while len(openSet) > 0:
      current = openSet[0]
      #check to see if we have reached our goal
      if current[0] == [goalX, goalY]:
        lastNode = current[2]
        finalPath = [lastNode]          
        while lastNode != [startX, startY]:
          lastNode = closedSet[tuple(lastNode)]
          finalPath = finalPath + [lastNode]
        finalPath.remove(finalPath[len(finalPath)-1])
        finalPath = finalPath[::-1]
        return finalPath
      #loops through each neighboring node
      for node in self.neighborNodes(current[0][0], current[0][1]):
        #calculate their score and path and add them as possible paths         
        if any(oldnode for oldnode in openSet if oldnode[0][0] == node[0] and oldnode[0][1] == node[1]) or tuple(node) in closedSet:
          continue
        if (len(self.grid[node[0]][node[1]]) == 0) or (node == [goalX, goalY]):  
          openSet.append([node, self.distance(node[0], node[1], goalX, goalY), current[0]])
      closedSet[tuple(current[0])] = current[2]
      openSet.remove(current)
      openSet.sort(key=lambda x: float(x[1]))             
    return []
    
  def findNearestFriendlyBreedableCreatureXY(self, creature):
    #Doesn't work if I only have one creature left
    myCreatures = [creature for creature in self.creatures if creature.owner == self.playerID and creature.currentHealth > self.healthPerBreed + 15]
    if len(myCreatures) > 1:    
      #Creates a dictionary of the distances to each creature I control that isn't at the calling creature's position
      dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in myCreatures if target.x != creature.x and creature.y != target.y}    
      #Returns the object that is the closest creature   
      if len(dict) > 0:      
        bestCreature = next(target for target in myCreatures if target.id == min(dict, key=dict.get))
        #Returns the x and y value of that creature's location
        return bestCreature     
    return None
           
  #TODO Use some sort of method to find the BEST enemy to attack. This will be a function that takes in my creature and theirs
  #to see which has the best hueristic  
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
  
  def findNearestEdiblePlantXY(self, creature):
    #Doesn't work if there are no plants left to eat
    ediblePlants = [target for target in self.plants if target.size > 0]
    if len(ediblePlants) > 0:
      #Creates a dictionary of the distances to each plant that has a size > 0
      dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in ediblePlants}       
      #Returns the object that is the closest creature  
      bestID = min(dict, key=dict.get)
      bestPlant = next(target for target in self.plants if target.id == bestID)
      #Returns the x and y value of that plant's location
      return bestPlant
    return None

  def findAndEatPlant(self, creature):
    plant = self.findNearestEdiblePlantXY(creature) 
    if plant is not None: 
      path = self.findPath(creature.x, creature.y, plant.x, plant.y)
      #Move until we are to the plant or can move no more
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, plant.x, plant.y) > 1 and creature.currentHealth > self.healthPerMove:
        self.grid[creature.x][creature.y].remove(creature)  
        creature.move(path[0][0], path[0][1]) 
        self.grid[creature.x][creature.y].append(creature)         
        path.remove(path[0])
      #We are next to a plant! Eat it!
      if self.distance(creature.x, creature.y, plant.x, plant.y) == 1 and creature.canEat == True:
        creature.eat(plant.x, plant.y)  
    else:
      self.findNearestEnemyAndEat(creature)
      
  def testBreed(self, creature, mate):
    totalStats = 0
    totalStats += math.ceil(float(float(creature.energy + mate.energy)/2)) 
    totalStats += math.ceil(float(float(creature.carnivorism + mate.carnivorism)/2))
    totalStats += math.ceil(float(float(creature.herbivorism + mate.herbivorism)/2))
    totalStats += math.ceil(float(float(creature.defense + mate.defense)/2))
    totalStats += math.ceil(float(float(creature.speed + mate.speed)/2)) 
    return totalStats  
      
  def findBestAllyAndBreed(self,creature):
    allyCreatures = [ally for ally in self.creatures if ally.owner == self.playerID and ally.currentHealth > self.healthPerBreed + 15]
    dict = {target.id:self.testBreed(creature,target) for target in allyCreatures}
    bestID = max(dict, key=dict.get)
    bestMate = next(target for target in self.creatures if target.id == bestID)
    if self.distance(creature.x, creature.y, bestMate.x, bestMate.y) < (creature.speed + bestMate.speed)*2:
      nearestCreature = bestMate
      if nearestCreature is not None: 
        path = self.findPath(creature.x, creature.y, nearestCreature.x, nearestCreature.y)
        while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) > 1:
          self.grid[creature.x][creature.y].remove(creature)  
          creature.move(path[0][0], path[0][1]) 
          self.grid[creature.x][creature.y].append(creature)          
          path.remove(path[0])
        if self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) == 1 and creature.canBreed and creature.currentHealth > self.healthPerBreed:
          if nearestCreature.currentHealth > self.healthPerBreed and nearestCreature.canBreed:
            creature.breed(self.grid[nearestCreature.x][nearestCreature.y][0])
      else:
        self.findAndEatPlant(creature)
    else:
      self.findClosestAllyAndBreed(creature)

  def findClosestAllyAndBreed(self,creature):
    nearestCreature = self.findNearestFriendlyBreedableCreatureXY(creature) 
    if nearestCreature is not None: 
      path = self.findPath(creature.x, creature.y, nearestCreature.x, nearestCreature.y)
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) > 1 and creature.currentHealth > self.healthPerMove:
        self.grid[creature.x][creature.y].remove(creature)  
        creature.move(path[0][0], path[0][1]) 
        self.grid[creature.x][creature.y].append(creature)          
        path.remove(path[0])
      if self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) == 1 and creature.canBreed and creature.currentHealth > self.healthPerBreed:
        if nearestCreature.currentHealth > self.healthPerBreed and nearestCreature.canBreed:
          creature.breed(self.grid[nearestCreature.x][nearestCreature.y][0])
    else:
      self.findAndEatPlant(creature) 
                      
  def findNearestEnemyAndEat(self, creature):
    nearestCreature = self.findNearestEnemyCreatureXY(creature) 
    if nearestCreature is not None:    
      path = self.findPath(creature.x, creature.y, nearestCreature.x, nearestCreature.y)
      #Move until we are to the plant or can move no more
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) > 1 and creature.currentHealth > self.healthPerMove:
        self.grid[creature.x][creature.y].remove(creature)
        creature.move(path[0][0], path[0][1])  
        self.grid[creature.x][creature.y].append(creature)       
        path.remove(path[0])
      if self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) == 1 and creature.canEat:
        creature.eat(nearestCreature.x, nearestCreature.y) 
      if creature.canEat:
        locs = self.neighborNodes(creature.x, creature.y)
        for loc in locs:
          index = self.getCreatureAtLocation(loc[0],loc[1])
          if index != -1 and self.creatures[index].owner == self.playerID and creature.canEat and self.creatures[index].currentHealth < creature.carnivorism * 5:
            creature.eat(loc[0], loc[1])
    else:
      self.findAndEatPlant(creature)      
          
  def eatWeakAlly(self, creature):
    for ally in self.creatures:
      if ally.owner == self.playerID:
        if self.distance(creature.x, creature.y, ally.x, ally.y) <= creature.movementLeft + 1 and ((creature.carnivorism - ally.defense)*10 >= ally.currentHealth or ally.currentHealth <= 10):
          path = self.findPath(creature.x, creature.y, ally.x, ally.y)
          while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, ally.x, ally.y) > 1 and creature.currentHealth > self.healthPerMove:
            self.grid[creature.x][creature.y].remove(creature)
            creature.move(path[0][0], path[0][1])  
            self.grid[creature.x][creature.y].append(creature)       
            path.remove(path[0]) 
          if self.distance(creature.x, creature.y, ally.x, ally.y) == 1:
            creature.eat(ally.x, ally.y)
            
  def eatWeakEnemy(self, creature):
    for enemy in self.creatures:
      if enemy.owner != self.playerID:
        if self.distance(creature.x, creature.y, enemy.x, enemy.y) <= creature.movementLeft + 1 and ((creature.carnivorism - enemy.defense)*10 >= enemy.currentHealth or enemy.currentHealth <= 10):
          path = self.findPath(creature.x, creature.y, enemy.x, enemy.y)
          while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, enemy.x, enemy.y) > 1 and creature.currentHealth > self.healthPerMove:
            self.grid[creature.x][creature.y].remove(creature)
            creature.move(path[0][0], path[0][1])  
            self.grid[creature.x][creature.y].append(creature)       
            path.remove(path[0]) 
          if self.distance(creature.x, creature.y, enemy.x, enemy.y) == 1:
            creature.eat(enemy.x, enemy.y)

  def moveToNearestFriend(self, creature):
    nearestCreature = self.findNearestFriendlyBreedableCreatureXY(creature) 
    if nearestCreature is not None: 
      path = self.findPath(creature.x, creature.y, nearestCreature.x, nearestCreature.y)
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, nearestCreature.x, nearestCreature.y) > 1 and creature.currentHealth > self.healthPerMove:
        self.grid[creature.x][creature.y].remove(creature)  
        creature.move(path[0][0], path[0][1]) 
        self.grid[creature.x][creature.y].append(creature)          
        path.remove(path[0])  
                
  #If health is low, eat a plant.
  #If health is very high, breed
  def defaultActions(self,creature):   
    nearestEnemy = self.findNearestEnemyCreatureXY(creature)
    nearestAlly = self.findNearestFriendlyBreedableCreatureXY(creature)
    nearestPlant = self.findNearestEdiblePlantXY(creature)
    if nearestEnemy is not None and creature.canEat:
      self.eatWeakEnemy(creature)  
    if nearestAlly is not None and creature.canEat:
      self.eatWeakAlly(creature) 
    if self.distance(creature.x, creature.y, nearestEnemy.x, nearestEnemy.y) <= creature.movementLeft + 1:
      self.findNearestEnemyAndEat(creature)
    elif creature.currentHealth > (creature.maxHealth*.65) and nearestAlly is not None and nearestEnemy is not None:
      self.findBestAllyAndBreed(creature)
    if creature.currentHealth < (creature.maxHealth*.65) and creature.canEat and nearestPlant is not None \
    and self.distance(creature.x, creature.y, nearestPlant.x, nearestPlant.y) <= creature.movementLeft + 1:
      self.findAndEatPlant(creature)
    elif nearestAlly is not None and creature.currentHealth <= (20 - creature.defense) and creature.movementLeft > 0:
      self.moveToNearestFriend(creature)
    else:
      self.findAndEatPlant(creature)
          
  #Kill    
  def carnivorismActionSet(self,creature):
    nearestEnemy = self.findNearestEnemyCreatureXY(creature)
    if creature.currentHealth > self.healthPerBreed + 25 and self.distance(creature.x, creature.y, nearestEnemy.x, nearestEnemy.y) > 10:
      self.findClosestAllyAndBreed(creature)
    else:
      self.findNearestEnemyAndEat(creature) 

  #Eat minimum and make babies
  def herbivorismActionSet(self,creature):
    self.defaultActions(creature)
      
  #Breed all day long, awww yeah
  def energyActionSet(self,creature):
    self.defaultActions(creature)
    
  #Attack and breed 
  def defenseActionSet(self,creature):
    self.defaultActions(creature)
      
  #Eat all of the plants near the enemies
  def speedActionSet(self,creature):
    nearestEnemy = self.findNearestEnemyCreatureXY(creature)
    plant = self.findNearestEdiblePlantXY(nearestEnemy) 
    if plant is not None: 
      path = self.findPath(creature.x, creature.y, plant.x, plant.y)
      #Move until we are to the plant or can move no more
      while len(path) > 0 and creature.movementLeft > 0 and self.distance(creature.x, creature.y, plant.x, plant.y) > 1 and creature.currentHealth > self.healthPerMove:
        self.grid[creature.x][creature.y].remove(creature)
        creature.move(path[0][0], path[0][1])  
        self.grid[creature.x][creature.y].append(creature)       
        path.remove(path[0])
      #We are next to a plant! Eat it!
      if self.distance(creature.x, creature.y, plant.x, plant.y) == 1 and creature.canEat == True:
        creature.eat(plant.x, plant.y)  
        if plant.size == 0 and creature.movementLeft > 0 and creature.currentHealth > self.healthPerMove:
          creature.move(plant.x, plant.y) 
      if creature.canEat:
        locs = self.neighborNodes(creature.x, creature.y)
        for loc in locs:
          index = self.getCreatureAtLocation(loc[0],loc[1])
          if index != -1 and self.creatures[index].owner != self.playerID and creature.canEat:
            creature.eat(loc[0], loc[1])
          elif index != -1 and self.creatures[index].owner != self.playerID and creature.canEat and self.creatures[index].currentHealth < creature.carnivorism * 5:
            creature.eat(loc[0], loc[1])
    
    self.defaultActions(creature)
        
  def init(self): 
    pass
  ##This function is called once, after your last turn
  def end(self):
    pass

  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):
    self.grid = [[[] for _ in range(self.mapHeight)] for _ in range(self.mapWidth)] 
    self.statCreatureList = {"energy": [], "carnivorism": [], "herbivorism": [], "speed": [], "defense": []}  
    
    #Establishes a grid of the current objects   
    for plant in self.plants:
      self.grid[plant.x][plant.y].append(plant)
    for creature in self.creatures:
      self.grid[creature.x][creature.y].append(creature)
      if creature.owner == self.playerID and creature.currentHealth > 0:
        stats = {"energy": creature.energy, "carnivorism": creature.carnivorism, "herbivorism": creature.herbivorism, "defense": creature.defense,"speed": creature.speed} 
        self.statCreatureList[max(stats, key=stats.get)].append(creature)         
    
  
    print self.turnNumber
    myCreatures = [creature for creature in self.creatures if creature.owner == self.playerID]
    for creature in self.statCreatureList["carnivorism"]:
      #self.carnivorismActionSet(creature)
      self.herbivorismActionSet(creature)
    for creature in self.statCreatureList["herbivorism"]:
      self.herbivorismActionSet(creature)
    for creature in self.statCreatureList["energy"]:
      self.energyActionSet(creature)
    for creature in self.statCreatureList["defense"]:
      self.defenseActionSet(creature)
    for creature in self.statCreatureList["speed"]:
      self.herbivorismActionSet(creature)
      #self.speedActionSet(creature)
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)
   

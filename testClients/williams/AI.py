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
    return "Shell AI"

  @staticmethod
  def password():
    return "password"
    
  def distance(self,x1,y1,x2,y2):
    return math.sqrt((x1-x2)**2 + (y1-y2)**2)
    
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
    moveSet = [[startX, startY, self.distance(startX, startY, goalX, goalY), [startX, startY]]]
    checkedSet = []
    current = moveSet[0]
    while len(moveSet) > 0:   
      current = moveSet[0]
      if current[0] == goalX and current[1] == goalY:
        pathNode = current;
        finalPath = [[current[0],current[1]]]
        while pathNode[0] != startX and pathNode[1] != startY:
          pathNode = [node for node in checkedSet if(node[0] == pathNode[3][0] and node[1] == pathNode[3][1])][0]
          finalPath = finalPath + [[pathNode[3][0], pathNode[3][1]]]
        print startX, startY, goalX, goalY
        print finalPath
        return finalPath
      for node in self.neighborNodes(current[0], current[1]):      
        if any(oldNode for oldNode in moveSet if oldNode[0] == node[0] and oldNode[1] == node[1]) or any(oldNode for oldNode in checkedSet if oldNode[0] == node[0] and oldNode[1] == node[1]): 
          continue
        if (len(self.grid[node[0]][node[1]]) == 0) or (node[0] == goalX and node[1] == goalY): 
          moveSet.append([node[0], node[1], math.floor(self.distance(node[0], node[1], goalX, goalY)),[current[0], current[1]]])
      checkedSet.append(moveSet[0])
      moveSet.remove(moveSet[0])          
      moveSet.sort(key=itemgetter(2))
    
  def findNearestFriendlyCreatureXY(self, creature):
    #Doesn't work if I only have one creature left
    if len(self.myCreatures) > 1:     
      #Creates a dictionary of the distances to each creature I control that isn't at the calling creature's position
      dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in self.myCreatures if target.x != creature.x and creature.y != target.y}  
      #Returns the object that is the closest creature      
      bestCreature = next(target for target in self.myCreatures if target.id == min(dict, key=dict.get))
      #Returns the x and y value of that creature's location
      return [bestCreature.x, bestCreature.y]     
    return [-1,-1]
           
  #TODO Use some sort of method to find the BEST enemy to attack. This will be a function that takes in my creature and theirs
  #to see which has the best hueristic  
  def findNearestEnemyCreatureXY(self, creature):   
    #Creates a dictionary of the distances to each enemy creature
    dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in self.enemyCreatures}  
    #Returns the object that is the closest creature      
    bestCreature = next(target for target in self.enemyCreatures if target.id == min(dict, key=dict.get))
    #Returns the x and y value of that creature's location
    return [bestCreature.x, bestCreature.y]  
  
  def findNearestEdiblePlantXY(self, creature):
    #Doesn't work if there are no plants left to eat
    if sum(target for target in self.plants if target.size > 0) > 0:
      #Creates a dictionary of the distances to each plant that has a size > 0
      dict = {target.id:math.floor(self.distance(creature.x, creature.y, target.x, target.y)) for target in self.plants if target.size > 0}  
      #Returns the object that is the closest creature      
      bestPlant = next(target for target in self.plants if target.id == min(dict, key=dict.get))
      #Returns the x and y value of that plant's location
      return [bestPlant.x, bestPlant.y]
    return [-1, -1]      
    
  def init(self):
    #Set up lists for highest stats among creatures
    statCreatureList = [[] for _ in range(5)]
    self.grid = [[[] for _ in range(self.mapHeight)] for _ in range(self.mapWidth)] 
    
    #Establishes a grid of the current objects   
    for plant in self.plants:
      self.grid[plant.x][plant.y] = [plant]
    for creature in self.creatures:
      if creature.owner == self.playerID:
        self.grid[creature.x][creature.y] = [creature] 
        stats = [(creature.maxEnergy - 100)/10, creature.carnivorism, creature.herbivorism, creature.defense, creature.speed] 
        statCreatureList[stats.index(max(stats))].append(creature)               

  ##This function is called once, after your last turn
  def end(self):
    pass

  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):
    self.theirCreatures = [creature for creature in self.creatures if creature.owner != self.playerID]
    self.myCreatures = [creature for creature in self.creatures if creature.owner == self.playerID]
    print self.turnNumber
    for creature in self.myCreatures:
      coords = self.findNearestFriendlyCreatureXY(creature)
      path = self.findPath(creature.x, creature.y, coords[0], coords[1])
      time.sleep(3)
      tempCount = creature.movementLeft
      while tempCount > 0 and len(path) > 0:
        creature.move(path[0][0], path[0][0])
        path.remove(path[0]);
        tempCount -= 1
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)
   

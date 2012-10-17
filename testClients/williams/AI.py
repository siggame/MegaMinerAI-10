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
    closedSet = {}
    #startx, starty, score, pathto
    openSet = [[[startX, startY], self.distance(startX, startY, goalX, goalY), [startX, startY]]]    
    #while we have nodes to iterate
    print startX, startY, goalX, goalY
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
      openSet = sorted(openSet)        
    return []
    
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
      movementLeft = creature.movementLeft
      while len(path) > 0 and movementLeft > 0:   
        print creature.x, creature.y, path[0][0], path[0][1]
        path.remove(path[0])
        movementLeft -= 1
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)
   

#-*-python-*-
from BaseAI import BaseAI
from GameObject import *
import random
import math
import heapq
		
class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "Kellogg"

  @staticmethod
  def password():
    return "password"

  ##This function is called once, before your first turn
  def init(self):
    pass
 
  ##This function is called once, after your last turn
  def end(self):
    pass
    
  def getObject(self,x,y):
    if len(self.grid[x][y])>0:
      return self.grid[x][y][0]
    else:
      return []
    
  def removeGrid(self,target):
    self.grid[target.x][target.y]=[]
    
  def addGrid(self,creature):
    self.grid[creature.x][creature.y].append(creature)
    
  def moveTo(self,creature,target):
     path = self.pathFind(creature.x,creature.y,target.x,target.y)
     if path != None:
       while creature.movementLeft>0 and len(path)>0:
         next = path.pop()
         if next!=(creature.x,creature.y) and creature.movementLeft>0 and creature.currentHealth>self.healthPerMove:
           self.removeGrid(creature)
           creature.move(next[0],next[1])
           self.addGrid(creature)
      
  def distance(self,sourceX,sourceY,destX,destY):
    return math.sqrt((sourceX-destX)**2+(sourceY-destY)**2)
    
  def maxStat(self,creature):
    dict = {creature.herbivorism:"herb",creature.carnivorism:"carn",creature.speed:"speed",creature.energy:"energy",creature.defense:"defense"}
    return dict[max(dict)]

  def findNearest(self,source,list,ignore=[]):
    d = {self.distance(source.x,source.y,lifeform.x,lifeform.y):lifeform for lifeform in list if lifeform not in ignore}
    return d[min(d)]      
  
  
  def adjacent(self,x,y):
    adj = []
    if x+1<self.mapWidth:
        adj.append((x+1,y))
    if y-1>=0:
        adj.append((x,y-1))
    if x-1>=0:
        adj.append((x-1,y))
    if y+1<self.mapHeight:
        adj.append((x,y+1))
    return adj  
  
  def pathFind(self,startX,startY,goalX,goalY):
    closedSet = set();closedTup=set()
    open = [(self.distance(startX,startY,goalX,goalY),(startX,startY),(startX,startY),0)];openTup=[(startX,startY)]
    path = []
    while len(open)>0:
      #open.sort()
      current = heapq.heappop(open)
      if current[1] == (goalX,goalY):
        node = current
        path = []
        while node[2]!=(startX,startY):
          for closed in closedSet:
            if self.distance(node[1][0],node[1][1],closed[1][0],closed[1][1])==1 and node[2] == closed[1]:
              path.append(node[2])
              node = closed
        return path
      closedSet.add(current);closedTup.add(current[1])
      openTup.remove(current[1])
      for neighbor in self.adjacent(current[1][0],current[1][1]):#,[(startX,startY),(goalX,goalY)]):
       if self.getObject(neighbor[0],neighbor[1])==[] or (neighbor[0],neighbor[1])==(goalX,goalY) or (neighbor[0],neighbor[1])==(startX,startY):
        if neighbor in closedTup:
         continue
        g = current[3]+self.distance(neighbor[0],neighbor[1],current[1][0],current[1][1])
        if neighbor == (goalX,goalY) or self.distance(neighbor[0],neighbor[1],startX,startY)<=g+1 and neighbor not in openTup:
          neighborTup = (g+self.distance(neighbor[0],neighbor[1],goalX,goalY),(neighbor[0],neighbor[1]),(current[1]),g)
          heapq.heappush(open,neighborTup);openTup.append(neighbor)
    return None
    
  def avail(self,source):
    adjacent = [[1,0],[-1,0],[0,1],[0,-1]]
    room = False
    for adj in adjacent:
      if 0<=source.x+adj[0]<self.mapWidth and 0<=source.y+adj[1]<self.mapHeight:
        if self.getObject(source.x+adj[0],source.y+adj[1])==[]:
          room = True
    return room
    
  def pickPlant(self,creature):
       ignore = [creature]; loop = True;numP=0
       while numP<len(self.plants):
         numP+=1
         plant = self.findNearest(creature,self.plants,ignore)
         if self.avail(plant) and plant.size>=1 and plant != None:
           return plant
         else:
          ignore.append(plant)
  
  def herbControl(self,creature,enemies):
   plant = self.pickPlant(creature)
   if plant!=None:
     self.moveTo(creature,plant)
     if self.distance(plant.x,plant.y,creature.x,creature.y)==1 and plant.size>0 and plant in self.plants:
        creature.eat(plant.x,plant.y)    
  
  def carnControl(self,creature,enemies):
    if creature.movementLeft>0:
        enemy = self.findNearest(creature,enemies,[creature])
        if self.avail(enemy):
          self.moveTo(creature,enemy)
        if self.distance(creature.x,creature.y,enemy.x,enemy.y)==1 and creature.canEat and isinstance(self.getObject(enemy.x,enemy.y),Creature) and enemy in self.creatures:
          creature.eat(enemy.x,enemy.y)    
          if creature.currentHealth<=0:
            self.removeGrid(enemy)
            self.creatures.remove(enemy)
  
  def breedControl(self,creature,mine):
    pass
  
  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):  
    self.grid = [[[] for _ in range(self.mapHeight)] for _ in range(self.mapWidth)]
    for life in self.plants+self.creatures:
      self.addGrid(life)
          
    herbivores = [creature for creature in self.creatures if creature.owner == self.playerID and (self.maxStat(creature)=="herb" or self.maxStat(creature)=="speed")]  
    carnivores = [creature for creature in self.creatures if creature.owner == self.playerID and self.maxStat(creature)=="carn" and creature not in herbivores]
    breeders = [creature for creature in self.creatures if creature.owner == self.playerID and creature not in carnivores]
    mine= herbivores+carnivores
    enemies = [creature for creature in self.creatures if creature.owner != self.playerID]
    
    for creature in herbivores:
     if len(self.plants)>0:
       self.herbControl(creature,enemies)
     else:
      self.carnControl(creature)
      
    for creature in carnivores:
      self.carnControl(creature,enemies)
      
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)


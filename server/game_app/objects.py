import math
class Mappable:
  def __init__(self, game, id, x, y):
    self.game = game
    self.id = id
    self.x = x
    self.y = y

  def toList(self):
    value = [
      self.id,
      self.x,
      self.y,
      ]
    return value

  def nextTurn(self):
    pass



class Creature(Mappable):
  def __init__(self, game, id, x, y, owner, maxEnergy, energyLeft, carnivorism, herbivorism, speed, movementLeft, defense, parentID):
    self.game = game
    self.id = id
    self.x = x
    self.y = y
    self.owner = owner
    self.maxEnergy = maxEnergy
    self.energyLeft = energyLeft
    self.carnivorism = carnivorism
    self.herbivorism = herbivorism
    self.speed = speed
    self.movementLeft = movementLeft
    self.defense = defense
    self.canEat = True
    self.canBreed = True
    self.parentID = parentID

  def toList(self):
    value = [
      self.id,
      self.x,
      self.y,
      self.owner,
      self.maxEnergy,
      self.energyLeft,
      self.carnivorism,
      self.herbivorism,
      self.speed,
      self.movementLeft,
      self.defense,
      self.canEat,
      self.canBreed,
      self.parentID,
      ]
    return value

  #Decrements the energy of the creature by energyDec. If the creature runs out of energy, it dies.
  #Returns true if the creature lives, returns false if it dies.
  def decrementEnergy(self, energyDec, creature):
    creature.energyLeft -= energyDec
    if creature.energyLeft <= 0:
      creature.game.removeObject(creature)
      creature.game.animations.append(['death', creature.id])
      return False
    return True

  def nextTurn(self):
    #If creatures are stacked they are unable to perform any actions
    if len(self.game.grid[self.x][self.y]) > 1:
      if(self.decrementEnergy(self.game.energyPerAction, self)):
        self.canEat = False
        self.canBreed = False
        self.movementLeft = 0
    #Else, we decrement energy like normal and reset stats
    elif(self.decrementEnergy(self.game.energyPerAction, self)):
      self.movementLeft = self.speed
      self.canEat= True
      self.canBreed = True
    return True

  def move(self, x, y):
    if self.owner != self.game.playerID:
      return "You cannot move your oppenent's creature."
    #You can't move if you have no moves left
    elif self.movementLeft <= 0:
      return "That creature has no more movement available"
    #You can't move off the edge, the world is flat
    elif not (0 <= x < self.game.mapWidth) or not (0 <= y < self.game.mapHeight):
      return "Don't move off of the map"
    #You can't move more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Units can only move to adjacent locations"
    #You can't move into the space of another object
    #Make all objects into a map to reduce check times if isinstance(lifeform, Plant):
    elif isinstance(self.game.getObject(x,y), Plant):
      if self.game.getObject(x,y).size > 0:
        return "There is a plant there!."
    elif isinstance(self.game.getObject(x,y), Creature):
      return "There is a creature there!."     
    if(self.decrementEnergy(self.game.energyPerAction, self)):
      #Update the grid where the target is moving
      self.game.grid[self.x][self.y].remove(self)
      self.game.grid[x][y].append(self)
      self.x = x
      self.y = y
      self.movementLeft -= 1
      self.game.animations.append(['move', self.id, self.x, self.y, x, y])  
      if isinstance(self.game.getObject(x,y), Plant):
        self.game.removeObject(self.game.getObject(x,y))
        self.game.grid[x][y].remove(self.game.getObject(x,y))
      return True
    return "Your creature died of starvation as it tried to move"

  def eat(self, x, y): 
    #You can only move your creature
    if self.owner != self.game.playerID:
      return "You cannot eat with your oppenent's creature"
    #You can't move if you have no moves left
    elif self.energyLeft <= 0:
      return "That creature has no energy left"
    #You can't eat more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Units can only eat objects at adjacent locations"
    #You can't eat if you've already eaten this turn.
    elif self.canEat != True:
      return "You can't eat more than once per turn!"
    #Get whether a lifeform exists in the tile you want to eat.
    lifeform = self.game.getObject(x,y)
    #You can't use the eat command on a tile with nothing in it.
    if lifeform is None:
      return "No lifeforms at that location."  
    if isinstance(lifeform, Plant):
      plant = lifeform
      if plant.size == 0:
        return "That plant is too small to eat."
      self.energyLeft += self.herbivorism * 5
      if self.energyLeft > self.maxEnergy:
        self.energyLeft = self.maxEnergy
      plant.size -= 1
      self.game.animations.append(['eat', self.id, plant.id])
    else:
      creature = lifeform
      damage = self.carnivorism - creature.defense
      if damage < 1:
        damage = 1
      creature.energyLeft -= damage
     
      if creature.energyLeft <= 0:
        #Updating the grid
        self.game.removeObject(creature)
        self.energyLeft += self.carnivorism * 5
        self.game.animations.append(['death', creature.id])
      else:
        self.decrementEnergy(self.game.energyPerAction, self)
      if self.energyLeft > self.maxEnergy:
        self.energyLeft = self.maxEnergy
      
      self.game.animations.append(['eat', self.id, creature.id])
    self.canEat = False
    return True

  def breed(self, mate):
    #You can only breed your creature
    if self.owner != self.game.playerID:
      return "You cannot breed using your oppenent's creature!"
    #You can't breed if you don't have enough energy
    elif self.energyLeft <= self.game.energyPerBreed:
      return "That creature doesn't have enough energy to breed!"
    #You can't breed if your mate doesn't have enough energy
    elif mate.energyLeft <= self.game.energyPerBreed:
      return "Your mate doesn't have enough energy to breed!"
    #You can't breed more than one space away
    elif abs(self.x-mate.x) + abs(self.y-mate.y) != 1:
      return "You can only breed with adjacent creatures"
    #Check to make sure you're not breeding with an opponent's creature
    elif mate.owner != self.owner:
      return "No fraternizing with the enemy" 
    #You can't breed if either partner has already bred.
    elif self.canBreed != True or mate.canBreed !=True:
      return "You've already bred this turn! You can't do it again."
  
    print "giggity"
  # by default set all stats to average of parents
    newEnergy = ((self.maxEnergy - 100)  / 10 + (mate.maxEnergy - 100) / 10) / 2
    newDefense = (self.defense + mate.defense) / 2
    newCarnivorism = (self.carnivorism + mate.carnivorism) / 2
    newHerbivorism = (self.herbivorism + mate.herbivorism) / 2
    newSpeed = (self.speed + mate.speed) / 2
     
#    newbaby = self.game.addObject(Creature,[self.owner, self.x, self.y] + self.babyStats(newEnergy, newCarnivorism, newHerbivorism, newSpeed, newDefense) + [self.id])
    newbaby = self.game.addObject(Creature,[self.owner,self.x,self.y]+self.newBreed(mate)+[self.id])
    self.game.animations.append(['Breed', self.id, mate.id, newbaby.id])    
    self.canBreed = False
    mate.canBreed = False
    self.canAttack = False
    mate.canAttack = False
    self.movementLeft = 0
    mate.movementLeft = 0
    self.decrementEnergy(self.game.energyPerBreed, self)
    self.decrementEnergy(self.game.energyPerBreed, mate) 
     
    #Update the grid with the new baby
    self.game.grid[self.x][self.y].append(newbaby)       
    return True
   
   
  def newBreed(self,mate):
    #Create a dictionary of the parent's stats
    fatherStats = {'energy':(self.maxEnergy-100)/10,'carnivorism':self.carnivorism,'herbivorism':self.herbivorism,'speed':self.speed,'defense':self.defense}
    motherStats = {'energy':(mate.maxEnergy-100)/10,'carnivorism':mate.carnivorism,'herbivorism':mate.herbivorism,'speed':mate.speed,'defense':mate.defense}
    
    #Create a new baby based on the average of the parents stats
    babyStats = {ii:math.ceil(float((float(fatherStats[ii])+motherStats[ii])/2)) for ii in fatherStats}   

    print sum(fatherStats.values())
    print sum(fatherStats.values())
    print sum(babyStats.values())
    #Remove fringe cases from possibilities
    for ii in babyStats:
      if babyStats[ii]==self.game.minStat:
        del motherStats[ii]
      elif babyStats[ii] == self.game.maxStat:
        del fatherStats[ii]
    #Increment father's highest stat and lower the mother's lowest    
    babyStats[max(fatherStats,key=fatherStats.get)]+=1
    babyStats[min(motherStats,key=motherStats.get)]-=1
    babyList = [babyStats['energy']*10+100,0,babyStats['carnivorism'],babyStats['herbivorism'],babyStats['speed'],0,babyStats['defense']]
    return babyList
   
  def babyStats(self, energy, carnivorism, herbivorism, speed, defense):
    #Create a list of equivilent stats
    stats = [energy, 0, carnivorism, herbivorism, speed, 0, defense]
    #Identify which stat is the highest
    maxStatIndex = stats.index(max(stats))   
    count = 0
    #If the highest stat is maxed, we move until the nex
    #If all are maxed, we just have a perfect creature
    while stats[maxStatIndex] != self.game.maxStat and count < len(stats):
      maxStatIndex = (maxStatIndex + 1) % len(stats)
      count = count+1
    #Increase that stat by 1
    if count < 5:
      stats[maxStatIndex] = stats[maxStatIndex] + 1
    #Convert energy back to its normal format
    stats[0] = (stats[0]*10) + 100
    
    return stats

class Plant(Mappable):
  def __init__(self, game, id, x, y, size, growthRate, turnsUntilGrowth):
    self.game = game
    self.id = id
    self.x = x
    self.y = y
    self.size = size
    self.growthRate = growthRate
    self.turnsUntilGrowth = turnsUntilGrowth

  def toList(self):
    value = [
      self.id,
      self.x,
      self.y,
      self.size,
      self.growthRate,
      self.turnsUntilGrowth,
      ]
    return value

  def nextTurn(self):
    self.turnsUntilGrowth -= 1
    if self.turnsUntilGrowth == 0 and self.size < 10:
      self.size += 1
      self.turnsUntilGrowth = self.growthRate     
    return True

class Player:
  def __init__(self, game, id, playerName, time):
    self.game = game
    self.id = id
    self.playerName = playerName
    self.time = time

  def toList(self):
    value = [
      self.id,
      self.playerName,
      self.time,
      ]
    return value

  def nextTurn(self):
    pass

  def talk(self, message):
    pass




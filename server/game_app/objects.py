import math

class Mappable:
  def __init__(self, game, id, x, y):
    self.game = game
    self.id = id
    self.x = x
    self.y = y

  def toList(self):
    return [self.id, self.x, self.y, ]
    
  # This will not work if the object has variables other than primitives
  def toJson(self):
    return dict(id = self.id, x = self.x, y = self.y, )

  def nextTurn(self):
    pass



class Creature(Mappable):
  def __init__(self, game, id, x, y, owner, maxHealth, currentHealth, energy, carnivorism, herbivorism, speed, movementLeft, defense, parentID):
    self.game = game
    self.id = id
    self.x = x
    self.y = y
    self.owner = owner
    self.maxHealth = maxHealth
    self.currentHealth = currentHealth
    self.energy = energy
    self.carnivorism = carnivorism
    self.herbivorism = herbivorism
    self.speed = speed
    self.movementLeft = movementLeft
    self.defense = defense
    self.canEat = True
    self.canBreed = True
    self.parentID = parentID

  def toList(self):
    return [self.id, self.x, self.y, self.owner, self.maxHealth, self.currentHealth, self.energy, self.carnivorism, self.herbivorism, self.speed, self.movementLeft, self.defense, self.canEat, self.canBreed, self.parentID, ]
  
  # This will not work if the object has variables other than primitives
  def toJson(self):
    return dict(id = self.id, x = self.x, y = self.y, owner = self.owner, maxHealth = self.maxHealth, currentHealth = self.currentHealth, energy = self.energy, carnivorism = self.carnivorism, herbivorism = self.herbivorism, speed = self.speed, movementLeft = self.movementLeft, defense = self.defense, canEat = self.canEat, canBreed = self.canBreed, parentID = self.parentID, )
  

  #Decrements the energy of the creature by energyDec. If the creature runs out of energy, it dies.
  #Returns true if the creature lives, returns false if it dies.
  def decrementEnergy(self, energyDec, creature):
    creature.currentHealth -= energyDec
    if creature.currentHealth <= 0:
      self.game.grid[creature.x][creature.y].remove(creature)  
      self.game.removeObject(creature)    
      self.game.addAnimation(DeathAnimation(creature.id))
      return False
    return True

  def nextTurn(self):
    if len(self.game.grid[self.x][self.y]) > 1:
      #If the creature is stacked, it loses its ability to be productive
      if(self.decrementEnergy(self.game.healthPerTurn, self)):
        self.canEat = 0
        self.canBreed = 0
        self.movementLeft = self.speed
    #Else, we decrement energy like normal and reset stats
    elif(self.decrementEnergy(self.game.healthPerTurn, self)):
      self.movementLeft = self.speed
      self.canEat = 1
      self.canBreed = 1
    return True

  def move(self,x,y):
    if self.owner != self.game.playerID:
      return "You cannot move your oppenent's creature %i."%(self.id)
    #You can't move if you have no moves left
    elif self.movementLeft <= 0:
      return "The creature  %i has no more movement available"%(self.id)
    #You can't move off the edge, the world is flat
    elif not (0 <= x < self.game.mapWidth) or not (0 <= y < self.game.mapHeight):
      return "Don't move creature %i off of the map to (%i,%i)"%(self.id,x,y)
    #You can't move more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Units can only move to adjacent locations not from (%i,%i) to (%i,%i)"%(self.x, self.y, x, y)
    #You can't move into the space of another object
    #Make all objects into a map to reduce check times if isinstance(lifeform, Plant):
    elif isinstance(self.game.getObject(x,y), Plant) and self.game.getObject(x,y).size > 0:
      return "You can't move %i to (%i,%i) there is a plant %i there!"%(self.id,x,y,self.game.getObject(x,y).id)
    elif isinstance(self.game.getObject(x,y), Creature):
      return "You can't move %i to (%i,%i), there is a creature %i there!"%(self.id,x,y,self.game.getObject(x,y).id)
    #If the creature moved and didn't die in the process      
    if(self.decrementEnergy(self.game.healthPerMove, self)):
      if isinstance(self.game.getObject(x,y), Plant):
        self.game.removeObject(self.game.getObject(x,y))
        self.game.grid[x][y].remove(self.game.getObject(x,y))
      #Update the grid where the target is moving
      self.game.grid[self.x][self.y].remove(self)
      self.game.grid[x][y].append(self)
      self.movementLeft -= 1
      self.game.addAnimation(MoveAnimation(self.id, self.x, self.y, x, y))  
      self.x = x
      self.y = y       
      return True
    return "Your creature %i died of starvation as it tried to move"%(self.id)

  def eat(self, x, y): 
    #You can only move your creature
    if self.owner != self.game.playerID:
      return "You cannot eat with your oppenent's creature %i"%(self.id)
    #You can't move if you have no moves left
    elif self.currentHealth <= 0:
      return "That creature %i has no energy left"%(self.id)
    #You can't eat more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Creature %i can only eat objects at adjacent locations, not (%i,%i)"%(self.id,x,y)
    #You can't eat if you've already eaten this turn.
    elif self.canEat != True:
      return "Creature %i can't eat more than once per turn!"%(self.id)
    #Get whether a lifeform exists in the tile you want to eat.
    lifeform = self.game.getObject(x,y)
    #You can't use the eat command on a tile with nothing in it.
    if lifeform is None:
      return "No lifeforms at the location (%i,%i)."%(x,y)  
    if isinstance(lifeform, Plant):
      plant = lifeform
      if plant.size == 0:
        return "That plant %i is too small to eat."%(lifeform.id)
      self.currentHealth += self.herbivorism * 5
      if self.currentHealth > self.maxHealth:
        self.currentHealth = self.maxHealth
      plant.size -= 1
      #self.game.animations.append(['eat', self.id, plant.id])
      self.game.addAnimation(EatAnimation(self.id, plant.id))
    else:
      creature = lifeform
      damage = self.carnivorism - creature.defense
      if damage < 1:
        damage = 1
      damage = damage * self.game.damageMul
      #Damage the target creature
      if not self.decrementEnergy(damage, creature):
        self.currentHealth += self.carnivorism * 10    
        if self.currentHealth > self.maxHealth:
          self.currentHealth = self.maxHealth    
      self.game.addAnimation(EatAnimation(self.id, creature.id))
    self.canEat = False
    return True

  def breed(self, mate):
    #You can only breed your creature
    if self.owner != self.game.playerID:
      return "You cannot breed using your oppenent's creature %i!"%(self.id)
    #You can't breed if you don't have enough energy
    elif self.currentHealth <= self.game.healthPerBreed:
      return "Creature %i doesn't have enough energy to breed!"%(self.id)
    #You can't breed if your mate doesn't have enough energy
    elif mate.currentHealth <= self.game.healthPerBreed:
      return "Your mate %i doesn't have enough energy to breed with creature %i!"%(mate.id,self.id)
    #You can't breed more than one space away
    elif abs(self.x-mate.x) + abs(self.y-mate.y) != 1:
      return "Mate %i is too far away from Creature %i to breed"%(mate.id,self.id)
    #Check to make sure you're not breeding with an opponent's creature
    elif mate.owner != self.owner:
      return "No fraternizing with the enemy, creature %i cannot breed with enemy creature %i"%(self.id,mate.id)
    #You can't breed if either partner has already bred.
    elif not self.canBreed or not mate.canBreed:
      return "You already bred creature %i or mate %i this turn! You can't do it again."%(self.id,mate.id)
    # by default set all stats to average of parents
    elif len(self.game.grid[self.x][self.y])>2:
    #may not need any of this at all, bonus points to whomever triggers this case
      baby = [baby.id for baby in self.creatures if baby.parentID == self.id and baby.x == self.x and baby.y == self.y]
      return "The creature %i is already taking care of a baby %i"%(self.id,baby[0])
      
    newEnergy = (self.energy + mate.energy) / 2
    newDefense = (self.defense + mate.defense) / 2
    newCarnivorism = (self.carnivorism + mate.carnivorism) / 2
    newHerbivorism = (self.herbivorism + mate.herbivorism) / 2
    newSpeed = (self.speed + mate.speed) / 2

    #adding new baby to players breeding list, for game state reasons  
    creatureStats = [self.x,self.y,self.owner]+self.newBreed(mate)+[self.id]
    player = self.game.objects.players[self.game.playerID]      
    #need to keep track of baby's stats, and the mate id
    player.breeding.append((creatureStats,mate.id))
  
    self.canBreed = False
    mate.canBreed = False
    self.canEat = False
    mate.canEat = False
    self.movementLeft = 0
    mate.movementLeft = 0
    self.decrementEnergy(self.game.healthPerBreed, self)
    self.decrementEnergy(self.game.healthPerBreed, mate)   
    self.game.addAnimation(BreedAnimation(self.id, mate.id, 0))    
    return True
   
   
  def newBreed(self,mate):
    #Create a dictionary of the parent's stats
    fatherStats = {'energy':self.energy,'carnivorism':self.carnivorism,'herbivorism':self.herbivorism,'speed':self.speed,'defense':self.defense}
    motherStats = {'energy':mate.energy,'carnivorism':mate.carnivorism,'herbivorism':mate.herbivorism,'speed':mate.speed,'defense':mate.defense}
    
    #Create a new baby based on the average of the parents stats
    babyStats = {ii:math.ceil(float((float(fatherStats[ii])+motherStats[ii])/2)) for ii in fatherStats}   

#    print "Sum of the father's stats:", sum(fatherStats.values())
#    print "Sum of the mothers's stats:", sum(motherStats.values())
    print "Sum of the baby's stats:", sum(babyStats.values())
    #Remove fringe cases from possibilities
    for ii in babyStats:
      if babyStats[ii]==self.game.minStat:
        del motherStats[ii]
      elif babyStats[ii] == self.game.maxStat:
        del fatherStats[ii]
    #Increment father's highest stat and lower the mother's lowest    
    babyStats[max(fatherStats,key=fatherStats.get)]+=1
    babyStats[min(motherStats,key=motherStats.get)]-=1
    babyList = [self.game.baseHealth + int(babyStats['energy']*10),self.game.baseHealth,int(babyStats['energy']),babyStats['carnivorism'],babyStats['herbivorism'],babyStats['speed'],0,babyStats['defense']]
    return babyList

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
    return [self.id, self.x, self.y, self.size, self.growthRate, self.turnsUntilGrowth, ]
  
  # This will not work if the object has variables other than primitives
  def toJson(self):
    return dict(id = self.id, x = self.x, y = self.y, size = self.size, growthRate = self.growthRate, turnsUntilGrowth = self.turnsUntilGrowth, )
  

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
    self.breeding = []

  def toList(self):
    return [self.id, self.playerName, self.time, ]
  
  # This will not work if the object has variables other than primitives
  def toJson(self): 
    return dict(id = self.id, playerName = self.playerName, timee = self.time, )

  def nextTurn(self):
    if self.game.playerID == self.id:
      for eggStats,mateID in self.breeding:
        newBaby = self.game.addObject(Creature,eggStats)
        self.game.grid[newBaby.x][newBaby.y].append(newBaby)
      self.breeding = []

  def talk(self, message):
    if self.game.playerID == self.id:
      self.game.addAnimation(PlayerTalkAnimation(self.id,message))
    else:
      return "You cannot speak for your opponent"
    return True

# The following are animations and do not need to have any logic added
class MoveAnimation:
  def __init__(self, actingID, fromX, fromY, toX, toY):
    self.actingID = actingID
    self.fromX = fromX
    self.fromY = fromY
    self.toX = toX
    self.toY = toY

  def toList(self):
    return ["move", self.actingID, self.fromX, self.fromY, self.toX, self.toY, ]

  def toJson(self):
    return dict(type = "move", actingID = self.actingID, fromX = self.fromX, fromY = self.fromY, toX = self.toX, toY = self.toY)

class PlayerTalkAnimation:
  def __init__(self, actingID, message):
    self.actingID = actingID
    self.message = message

  def toList(self):
    return ["playerTalk", self.actingID, self.message, ]

  def toJson(self):
    return dict(type = "playerTalk", actingID = self.actingID, message = self.message)

class DeathAnimation:
  def __init__(self, actingID):
    self.actingID = actingID

  def toList(self):
    return ["death", self.actingID, ]

  def toJson(self):
    return dict(type = "death", actingID = self.actingID)

class EatAnimation:
  def __init__(self, actingID, targetID):
    self.actingID = actingID
    self.targetID = targetID

  def toList(self):
    return ["eat", self.actingID, self.targetID, ]

  def toJson(self):
    return dict(type = "eat", actingID = self.actingID, targetID = self.targetID)

class BreedAnimation:
  def __init__(self, actingID, targetID, childID):
    self.actingID = actingID
    self.targetID = targetID
    self.childID = childID

  def toList(self):
    return ["breed", self.actingID, self.targetID, self.childID, ]

  def toJson(self):
    return dict(type = "breed", actingID = self.actingID, targetID = self.targetID, childID = self.childID)


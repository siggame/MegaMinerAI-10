import networking.config.config
cfgCreature = networking.config.config.readConfig("config/creatureStats.cfg")
class Creature:
  def __init__(self, game, id, owner, x, y, maxEnergy, energyLeft, carnivorism, herbivorism, speed, movementLeft, defense):
    self.game = game
    self.id = id
    self.owner = owner
    self.x = x
    self.y = y
    self.maxEnergy = maxEnergy
    self.energyLeft = energyLeft
    self.carnivorism = carnivorism
    self.herbivorism = herbivorism
    self.speed = speed
    self.movementLeft = movementLeft
    self.defense = defense
    self.canAttack = True
    self.canBreed = True

  def toList(self):
    value = [
      self.id,
      self.owner,
      self.x,
      self.y,
      self.maxEnergy,
      self.energyLeft,
      self.carnivorism,
      self.herbivorism,
      self.speed,
      self.movementLeft,
      self.defense,
      self.canAttack,
      self.canBreed,
      ]
    return value

  def nextTurn(self):
    pass
  #Decrements the energy of the creature by energyDec. If the creature runs out of energy, it dies.
  #Returns true if the creature lives, returns false if it dies.
  def decrementEnergy(self, energyDec, creature):
    creature.energyLeft -= energyDec
    if creature.energyLeft <= 0:
      creature.game.removeObject(self)
      creature.game.animations.append(['death', self.id])
      return False
    return True

  def move(self, x, y):
    if self.owner != self.game.playerID:
      return "You cannot move your oppenent's creature."
    #You can't move if you have no moves left
    elif self.movesLeft <= 0:
      return "That creature has no more stamina"
    #You can't move off the edge, the world is flat
    elif not (0 <= x < self.game.mapWidth) or not (0 <= y < self.game.mapHeight):
      return "Don't move off of the map"
    #You can't move more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Units can only move to adjacent locations"
    #You can't move into the space of another object
    #Make all objects into a map to reduce check times
    if self.game.getObject(x,y) is not None:
      return "There is already an object in that location."
    #TODO Restrictions for movement when breeding
    self.x = x
    self.y = y
    self.movementLeft -= 1
    self.game.animations.append(['move', self.id, self.x, self.y, x, y])
    self.decrementEnergy(cfgCreature.EnergyPerAction, self)
    return True

  def eat(self, x, y): 
    #You can only move your creature
    if self.owner != self.game.playerID:
      return "You cannot eat with your oppenent's creature"
    #You can't move if you have no moves left
    elif self.energyLeft <= 0:
      return "That creature has no energy left"
    #You can't eat more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Units can only move to adjacent locations"
    #You can't eat if you've already eaten this turn.
    elif self.canAttack != True:
      return "You can't eat more than once per turn!"
    #Get whether a lifeform exists in the tile you want to eat.
    lifeform = self.game.getObject(x,y)
    #You can't use the eat command on a tile with nothing in it.
    if lifeform is None:
      return "No lifeforms at that location."  
    if isinstance(lifeform, Plant):
      plant = lifeform
      if size == 0:
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
        self.game.removeObject(creature)
        self.energyLeft += self.carnivorism * 5
        self.game.animations.append(['death', creature.id])
      else:
        self.decrementEnergy(cfgCreature.EnergyPerAction, self)
      if self.energyLeft > self.maxEnergy:
        self.energyLeft = self.maxEnergy
      
      self.game.animations.append(['eat', self.id, creature.id])
    self.canAttack = False
    return True

  def breed(self, mate, x, y):
    #You can only breed your creature
     if self.owner != self.id:
       return "You cannot breed using your oppenent's creature!"
    #You can't breed if you don't have enough energy
     elif self.energyLeft <= cfgCreature.EnergyPerBreed:
       return "That creature doesn't have enough energy to breed!"
    #You can't breed if your mate doesn't have enough energy
     elif mate.energyLeft <= cfgCreature.EnergyPerBreed:
       return "Your mate doesn't have enough energy to breed!"
    #You can't breed more than one space away
     elif abs(self.x-mate.x) + abs(self.y-mate.y) != 1:
       return "You can only breed with adjacent creatures"
    #Check to make sure you're not breeding with an opponent's creature
     elif mate.owner != self.id:
       return "No fraternizing with the enemy" 
    #Check to make sure baby is adjacent to parents
     elif not (abs(self.x-x) + abs(self.y-y) != 1 or abs(mate.x-x) + abs(mate.y-y) != 1):
       return "Your baby must be within 1 tile of one of the parents."
    #You can't spawn the baby off the edge, the world is flat
     elif not (0 <= x < self.game.mapSize) or not (0 <= y < self.game.mapSize):
       return "Don't spawn the baby off of the map"
    #Check that baby spawn location is empty    
     if self.game.getObject(x,y) is not None:
      return "Invalid location to spawn baby"
    #You can't breed if either partner has already bred.
     elif self.canBreed != True or mate.canBreed !=True:
       return "You've already bred this turn! You can't do it again."
    #TODO make baby better than parents
     # self.game.addObject(Creature, makeBaby(self, mate, x, y) )
  
  # by default set all stats to average of parents
     newEnergy = (self.maxEnergy + mate.maxEnergy) / 2
     newDefense = (self.defense + mate.defense) / 2
     newCarnivorism = (self.carnivorism + mate.canivorism) / 2
     newHerbivorism = (self.herbivorism + mate.herbivorism) / 2
     newSpeed = (self.speed + mate.speed) / 2
     newDefense = (self.defense + mate.defense) / 2
 
     totalStatsSoFar = newEnergy + newDefense + newCarnivorism + newHerbivorism + newSpeed
  
    # The child will have a total stat of the best parent's total plus one
     parentSumStat1 = self.energy + self.defense + self.carnivorism + self.herbivorism + self.speed
     parentSumStat2 = mate.energy + mate.defense + mate.carnivorism + mate.herbivorism + mate.speed
     targetStat = max(parentSumStat1, parentSumStat2) + 1
  
    # This loop will add all the unallocated stat points
    # TODO - finish this while loop
     while totalStatsSoFar < targetStat:
       totalSoFar += 1
  
     newbaby = self.game.addObject(Creature,[
       self.owner, 
       x, 
       y, 
       newEnergy, 
       newEnergy/3, 
       newCarnivorism, 
       newHerbivorism, 
       newSpeed, 
       0, 
       newDefense])
    #newbaby = self.game.addObject(Creature,[self.owner, x, y, 1, 1, 1, 1, 1, 1, 1, 1])
     self.game.animations.append(['Breed', self.id, mate.id, newbaby.id])
    #TODO amount of stamina necessary to breed     
     self.canBreed = False
     mate.canBreed = False
     self.decrementEnergy(cfgCreature.EnergyPerBreed, self)
     self.decrementEnergy(cfgCreature.EnergyPerBreed, mate) 

      
     return True
    
class Plant:
  def __init__(self, game, id, x, y, size):
    self.game = game
    self.id = id
    self.x = x
    self.y = y
    self.size = size

  def toList(self):
    value = [
      self.id,
      self.x,
      self.y,
      self.size,
      ]
    return value

  def nextTurn(self):
	#Grow slower if at zero
	if self.size == 0:
	  if self.game.turnNumber % self.game.plantGrowthRate *2 == 0:
	    if self.x < self.game.mapWidth /2:
	      self.size += 1
	  if self.game.turnNumber +1 % self.game.plantGrowthRate *2 == 0:
	    if self.x >+ self.game.mapWidth /2:
	      self.size += 1
	#Grow normall if not at zero
	else:
	  if self.game.turnNumber % self.game.plantGrowthRate == 0:
	    if self.x < self.game.mapWidth /2:
	      self.size += 1
	  if self.game.turnNumber +1 % self.game.plantGrowthRate == 0:
	    if self.x >+ self.game.mapWidth /2:
	      self.size += 1

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




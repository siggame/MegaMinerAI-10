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
    #self.canAttack = True
	#self.canBreed = True
	#self.movementLeft = self.speed
	#self.energy += self.game.hungerPerTurn
	
	#if self.energy >= self.game.maxHunger:
	  #die
    pass
  #TODO: remove this comment!
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
    #TODO Hunger cost of movement will be defined in a config file
    self.game.animations.append(['move', self.id, self.x, self.y, x, y])
    return True

  def eat(self, x, y): 
    # #You can only move your creature
    if self.owner != self.game.playerID:
      return "You cannot eat with your oppenent's creature"
    # #You can't move if you have no moves left
    elif self.energyLeft <= 0:
      return "That creature has no more stamina"
    # #You can't eat more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Units can only move to adjacent locations"
    # elif abs(self.x-plant.x) + abs(self.y-plant.y) != 1:
      # return "You can only eat adjacent plants"
    targetId = -1
    lifeform = self.game.getObject(x,y)
    if lifeform is not None:
      return "No lifeforms at that location."  
    if isinstance(lifeform, Plant):
      plant = self.game.objects[targetId]
      if size == 0:
        return "That plant is too small to eat."
      self.energyLeft += self.herbivorism * 5
      if self.energyLeft > self.maxEnergy:
        self.energyLeft = self.maxEnergy
      plant.size -= 1
      self.game.animations.append(['eat', self.id, plant.id])
    else:
      creature = self.game.getObject(x,y)
      damage = self.carnivorism - creature.defense
      if damage < 1:
        damage = 1
      creature.health -= damage
     
      if creature.health <= 0:
        self.game.removeObject(creature)
      self.energyLeft += self.carnivorism * 5

      if self.energyLeft > self.maxEnergy:
        self.energyLeft = self.maxEnergy

      self.game.animations.append(['eat', self.id, creature.id])
    return True

  def breed(self, mate, x, y):
    #You can only breed your creature
     if self.owner != self.id:
       return "You cannot breed using your oppenent's creature!"
    #You can't breed if you don't have enough energy
     elif self.energyLeft <= self.maxEnergy/3:
       return "That creature doesn't have enough energy to breed!"
    #You can't breed if your mate doesn't have enough energy
     elif mate.energyLeft <= mate.maxEnergy/3:
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
    #TODO make baby better than parents
     # self.game.addObject(Creature, makeBaby(self, mate, x, y) )
  
  # by default set all stats to average of parents
     newEnergy = (self.maxEnergy + mate.maxEnergy) / 2
     newDefense = (self.defense + mate.defense) / 2
     newCarnivorism = (self.carnivorism + mate.canivorism) / 2
     newHerbivorism = (self.herbivorism + mate.herbivorism) / 2
     newSpeed = (self.speed + mate.speed) / 2
     newDefense = (self.defense + mate.defense) / 2
 
     totalStatsSoFar = newEnergy + newDefense + newCarnivorism + newHerbivorishm + newSpeed
  
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
     self.energyLeft -= (self.maxEnergy / 3)
     mate.energyLeft -= (mate.maxEnergy / 3)
     if self.energyLeft <= 0:
       self.game.removeObject(self)
     if mate.energyLeft <= 0:
       self.game.removeObject(mate)
      
     return True

	
	
  # def makeBaby(self, mate, x, y):
    # #TODO: Figure out breeding algorithm
    
    # newBaby = Creature(x, y, 1, 1, 1, 1, 1, 1, 1, 0)
    
    # newBaby.health = self.object.maxHealth
    # newBaby.hunger = self.object.maxHunger
    # newBaby.carnivorism = math.ceil((self.carnivorism + mate.carnivorism) / 2)
    # newBaby.herbivorism = math.ceil((self.herbivorism + mate.herbivorism) / 2)
    # newBaby.speed = math.ceil((self.speed + mate.speed) / 2)
    # newBaby.maxStamina = math.ceil((self.stamina + mate.stamina) / 2)
    # newBaby.defense = math.ceil((self.defense + mate.defense) / 2)
    
    # statList = [newBaby.carnivorism, newBaby.herbivorism, newBaby.speed, newBaby.maxStamina, newBaby.defense]
    # bestStat = statList.index(max(statList))
    # #TODO increment best stat by one, make sure it doesn't go over self.objects.maxStat
    
    # return newBaby
  #pass
    
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
    #self.game.turnNumber
    #Grow slower if 0
    if self.size == 0:
      if self.game.turnNumber % self.game.plantGrowthRate * 2 == 0:
        if self.x < self.game.mapWidth/2:
          self.size += 1
      if self.game.turnNumber+1 % self.game.plantGrowthRate * 2 == 0:
        if self.x > self.game.mapWidth/2:
          self.size += 1
    #Grow normally
    else:
      if self.game.turnNumber % self.game.plantGrowthRate == 0:
        if self.x < self.game.mapWidth/2:
          if self.size <= self.game.plantMaxSize:
            self.size += 1
      if self.game.turnNumber+1 % self.game.plantGrowthRate == 0:
        if self.x > self.game.mapWidth/2:
          if self.size <= self.game.plantMaxSize:
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




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
      ]
    return value

  def nextTurn(self):
    pass

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
    for object in self.game.objects:
      if object.x == x and object.y == y:
        return "There is already an object in that location."
    #TODO Restrictions for movement when breeding
    self.x = x
    self.y = y
    self.movementLeft -= 1
    #TODO Hunger cost of movement will be defined in a config file
    self.game.animations.append(['move', self.id, self.x, self.y, x, y])
    return True

  def eat(self, x, y):  
    #You can only move your creature
     if self.owner != self.id:
       return "You cannot eat with your oppenent's creature."
    #You can't move if you have no moves left
     elif self.movementLeft <= 0:
       return "That creature has no more moves left."
    # #You can't eat more than one space away
    # elif abs(self.x-plant.x) + abs(self.y-plant.y) != 1:
      # return "You can only eat adjacent plants"
    
    # #TODO: Tweak this value
    # #TODO: 0 Hunger = Starvation or full?
    # #TODO: Check hunger is not greater than config max OR smaller than config min
    # self.hunger += 5*self.herbivorism
    # if self.hunger > self.objects.maxHunger:
      # self.hunger = self.objects.maxHunger
    # plant.size -= 1
    
    # #Remove the plant if needed
    # if plant.size == 0:
      # self.game.removeObject(plant)
    # self.game.animations.append(['eat', self.id, plant.id])
    # return True
	
	# #You can only eat with your creature
    # if self.owner != self.game.playerID:
      # return "You cannot eat with your oppenent's creature"
    # #You can't eat if you have no moves left
    # elif self.stamina <= 0:
      # return "That creature has no more stamina"
    # #You can't eat more than one space away
    # elif abs(self.x-target.x) + abs(self.y-target.y) != 1:
      # return "You can only eat adjacent animals"
      
    # #TODO damage
    # damage = self.carnivorism - target.defense
    # if damage < 1:
      # damage = 1
    # target.health -= damage
    
    # if target.health<=0:
      # self.game.removeObject(target)
      # #TODO adjust this hungry stuff
      # self.hunger += 25 + self.carnivorism * 5
      # if self.hunger > self.objects.maxHunger:
        # self.hunger = self.objects.maxHunger
    
    # self.game.animations.append(['eat', self.id, target.id])
    # return True
     pass

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
     for object in self.game.objects:
       if object.x == x and object.y == y:
         return "Invalid location to spawn baby"
    #TODO make baby better than parents
     # self.game.addObject(Creature, makeBaby(self, mate, x, y) )
     newbaby = self.game.addObject(Creature,[self.owner, x, y, 1, 1, 1, 1, 1, 1, 1, 1])
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
    pass



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




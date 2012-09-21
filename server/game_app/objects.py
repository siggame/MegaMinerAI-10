class Creature:
  def __init__(self, game, id, owner, x, y, health, hunger, carnivorism, herbivorism, speed, maxStamina, defense, age):
    self.game = game
    self.id = id
    self.owner = owner
    self.x = x
    self.y = y
    self.health = health
    self.hunger = hunger
    self.carnivorism = carnivorism
    self.herbivorism = herbivorism
    self.speed = speed
    self.maxStamina = maxStamina
    self.defense = defense
    self.age = age

  def toList(self):
    value = [
      self.id,
      self.owner,
      self.x,
      self.y,
      self.health,
      self.hunger,
      self.carnivorism,
      self.herbivorism,
      self.speed,
      self.maxStamina,
      self.defense,
      self.age,
      ]
    return value

  def nextTurn(self):
    self.age += 1
    if self.age == self.game.maxAge:
      self.game.removeObject(self)
    self.hunger -= self.game.hungerPerTurn
    if self.hunger <= 0:
      self.game.removeObject(self)
        

  def move(self, x, y):
    if self.owner != self.game.playerID:
      return "You cannot move your oppenent's creature."
    #You can't move if you have no moves left
    elif self.stamina <= 0:
      return "That creature has no more stamina"
    #You can't move off the edge, the world is flat
    elif not (0 <= x < self.game.mapSize) or not (0 <= y < self.game.mapSize):
      return "Don't move off of the map"
    #You can't move more than one space away
    elif abs(self.x-x) + abs(self.y-y) != 1:
      return "Units can only move to adjacent locations"
    #You can't move into the space of another object
    for object in self.game.objects:
      if object.x == x and object.y == y:
        return "There is already an object in that location."
    #TODO Restrictions for movement when breeding
    self.x = x
    self.y = y
    self.stamina -= 1
    #TODO Hunger cost of movement will be defined in a config file
    self.game.animations.append(['move', self.id, self.x, self.y, x, y])
    return True

  def eat(self, plant):
    #You can only move your creature
    if self.owner != self.game.playerID:
      return "You cannot eat with your oppenent's creature"
    #You can't move if you have no moves left
    elif self.stamina <= 0:
      return "That creature has no more stamina"
    #You can't eat more than one space away
    elif abs(self.x-plant.x) + abs(self.y-plant.y) != 1:
      return "You can only eat adjacent plants"
    
    #TODO: Tweak this value
    #TODO: 0 Hunger = Starvation or full?
    #TODO: Check hunger is not greater than config max OR smaller than config min
    self.hunger += 5*self.herbivorism
    if self.hunger > self.objects.maxHunger:
      self.hunger = self.objects.maxHunger
    plant.size -= 1
    
    #Remove the plant if needed
    if plant.size == 0:
      self.game.removeObject(plant)
    self.game.animations.append(['eat', self.id, plant.id])
    return True

  def breed(self, mate, x, y):
    #You can only breed your creature
    if self.owner != self.game.playerID:
      return "You cannot breed using your oppenent's creature"
    #You can't breed if you have no stamina left
    elif self.stamina <= 0:
      return "That creature has no more stamina"
    #You can't breed more than one space away
    elif abs(self.x-mate.x) + abs(self.y-mate.y) != 1:
      return "You can only breed with adjacent creatures"
    #Check to make sure you're not breeding with an opponent's creature
    elif mate.owner != self.game.playerID:
      return "No fraternizing with the enemy"
    #Check to make sure baby is adjacent to parents
    elif not (abs(self.x-x) + abs(self.y-y) != 1 or abs(mate.x-x) + abs(mate.y-y) != 1):
      return "You spawned your baby too far away"
    #You can't spawn the baby off the edge, the world is flat
    elif not (0 <= x < self.game.mapSize) or not (0 <= y < self.game.mapSize):
      return "Don't spawn the baby off of the map"
    #Check that baby spawn location is empty    
    for object in self.game.objects:
      if object.x == x and object.y == y:
        return "Invalid location to spawn baby"
    
    
    #TODO make baby better than parents
    self.game.addObject(Creature, makeBaby(self, mate, x, y) )
    #self.game.addObject(Creature,[self.owner, x, y, 1, 1, 1, 1, 1, 1, 1, 1])
    self.game.animations.append(['Breed', self.id, mate.id])
    #TODO amount of stamina necessary to breed
    self.stamina -= (self.stamina / 3)
    mate.stamina -= (mate.stamina / 3)
    self.hunger -= self.objects.hungerPerBreed
    mate.hunger -= self.objects.hungerPerBreed
    if self.hunger <= 0:
      self.game.removeObject(self)
    if mate.hunger <= 0:
      self.game.removeObject(mate)
      
    return True

  def eat(self, target):
    #You can only eat with your creature
    if self.owner != self.game.playerID:
      return "You cannot eat with your oppenent's creature"
    #You can't eat if you have no moves left
    elif self.stamina <= 0:
      return "That creature has no more stamina"
    #You can't eat more than one space away
    elif abs(self.x-target.x) + abs(self.y-target.y) != 1:
      return "You can only eat adjacent animals"
      
    #TODO damage
    damage = self.carnivorism - target.defense
    if damage < 1:
      damage = 1
    target.health -= damage
    
    if target.health<=0:
      self.game.removeObject(target)
      #TODO adjust this hungry stuff
      self.hunger += 25 + self.carnivorism * 5
      if self.hunger > self.objects.maxHunger:
        self.hunger = self.objects.maxHunger
    
    self.game.animations.append(['eat', self.id, target.id])

    return True

  def makeBaby(self, mate, x, y):
    #TODO: Figure out breeding algorithm
    
    newBaby = new Creature(x, y, 1, 1, 1, 1, 1, 1, 1, 0)
    
    newBaby.health = self.object.maxHealth
    newBaby.hunger = self.object.maxHunger
    newBaby.carnivorism = math.ceil((self.carnivorism + mate.carnivorism) / 2)
    newBaby.herbivorism = math.ceil((self.herbivorism + mate.herbivorism) / 2)
    newBaby.speed = math.ceil((self.speed + mate.speed) / 2)
    newBaby.maxStamina = math.ceil((self.stamina + mate.stamina) / 2)
    newBaby.defense = math.ceil((self.defense + mate.defense) / 2)
    
    statList = [newBaby.carnivorism, newBaby.herbivorism, newBaby.speed, newBaby.maxStamina, newBaby.defense]
    bestStat = statList.index(max(statList))
    #TODO increment best stat by one, make sure it doesn't go over self.objects.maxStat
    
    return newBaby
    
    
class Plant:
  def __init__(self, game, id, x, y, size, age):
    self.game = game
    self.id = id
    self.x = x
    self.y = y
    self.size = size
    self.age = age

  def toList(self):
    value = [
      self.id,
      self.x,
      self.y,
      self.size,
      self.age,
      ]
    return value

  def nextTurn(self):
    self.age += 1
    if self.age % self.game.plantGrowthRate == 0:
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




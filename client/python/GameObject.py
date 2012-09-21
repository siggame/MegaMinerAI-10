# -*- python -*-

from library import library

from ExistentialError import ExistentialError

class GameObject(object):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration


##A basic creature!
class Creature(GameObject):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.creatureGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.creatures:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  ##Command a creature to move to a specified position
  def move(self, x, y):
    self.validify()
    return library.creatureMove(self._ptr, x, y)

  ##Command a creature to eat a specified plant
  def eat(self, plant):
    self.validify()
    if not isinstance(plant, Plant):
      raise TypeError('plant should be of [Plant]')
    plant.validify()
    return library.creatureEat(self._ptr, plant._ptr)

  ##Breed with target adjacent creature. Spawn new creature at input location
  def breed(self, mate, x, y):
    self.validify()
    if not isinstance(mate, Creature):
      raise TypeError('mate should be of [Creature]')
    mate.validify()
    return library.creatureBreed(self._ptr, mate._ptr, x, y)

  ##Attack and try to eat target creature
  def eat(self, target):
    self.validify()
    if not isinstance(target, Creature):
      raise TypeError('target should be of [Creature]')
    target.validify()
    return library.creatureEat(self._ptr, target._ptr)

  #\cond
  def getId(self):
    self.validify()
    return library.creatureGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getOwner(self):
    self.validify()
    return library.creatureGetOwner(self._ptr)
  #\endcond
  ##The owner of the creature
  owner = property(getOwner)

  #\cond
  def getX(self):
    self.validify()
    return library.creatureGetX(self._ptr)
  #\endcond
  ##X position of the creature
  x = property(getX)

  #\cond
  def getY(self):
    self.validify()
    return library.creatureGetY(self._ptr)
  #\endcond
  ##Y position of the creature
  y = property(getY)

  #\cond
  def getHealth(self):
    self.validify()
    return library.creatureGetHealth(self._ptr)
  #\endcond
  ##The health of the creature
  health = property(getHealth)

  #\cond
  def getHunger(self):
    self.validify()
    return library.creatureGetHunger(self._ptr)
  #\endcond
  ##The current hunger of the creature
  hunger = property(getHunger)

  #\cond
  def getCarnivorism(self):
    self.validify()
    return library.creatureGetCarnivorism(self._ptr)
  #\endcond
  ##The carnivore level of the creature
  carnivorism = property(getCarnivorism)

  #\cond
  def getHerbivorism(self):
    self.validify()
    return library.creatureGetHerbivorism(self._ptr)
  #\endcond
  ##The herbivore level of the creature
  herbivorism = property(getHerbivorism)

  #\cond
  def getSpeed(self):
    self.validify()
    return library.creatureGetSpeed(self._ptr)
  #\endcond
  ##The speed of the creature
  speed = property(getSpeed)

  #\cond
  def getMaxStamina(self):
    self.validify()
    return library.creatureGetMaxStamina(self._ptr)
  #\endcond
  ##The max stamina of the creature
  maxStamina = property(getMaxStamina)

  #\cond
  def getDefense(self):
    self.validify()
    return library.creatureGetDefense(self._ptr)
  #\endcond
  ##The defense of the creature
  defense = property(getDefense)

  #\cond
  def getAge(self):
    self.validify()
    return library.creatureGetAge(self._ptr)
  #\endcond
  ##The age of the creature
  age = property(getAge)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "owner: %s\n" % self.getOwner()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    ret += "health: %s\n" % self.getHealth()
    ret += "hunger: %s\n" % self.getHunger()
    ret += "carnivorism: %s\n" % self.getCarnivorism()
    ret += "herbivorism: %s\n" % self.getHerbivorism()
    ret += "speed: %s\n" % self.getSpeed()
    ret += "maxStamina: %s\n" % self.getMaxStamina()
    ret += "defense: %s\n" % self.getDefense()
    ret += "age: %s\n" % self.getAge()
    return ret

##A basic plant!
class Plant(GameObject):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.plantGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.plants:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  #\cond
  def getId(self):
    self.validify()
    return library.plantGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getX(self):
    self.validify()
    return library.plantGetX(self._ptr)
  #\endcond
  ##X position of the plant
  x = property(getX)

  #\cond
  def getY(self):
    self.validify()
    return library.plantGetY(self._ptr)
  #\endcond
  ##Y position of the plant
  y = property(getY)

  #\cond
  def getSize(self):
    self.validify()
    return library.plantGetSize(self._ptr)
  #\endcond
  ##The size of the plant
  size = property(getSize)

  #\cond
  def getAge(self):
    self.validify()
    return library.plantGetAge(self._ptr)
  #\endcond
  ##The age of the plant
  age = property(getAge)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    ret += "size: %s\n" % self.getSize()
    ret += "age: %s\n" % self.getAge()
    return ret

##
class Player(GameObject):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.playerGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.players:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  ##Allows a player to display messages on the screen
  def talk(self, message):
    self.validify()
    return library.playerTalk(self._ptr, message)

  #\cond
  def getId(self):
    self.validify()
    return library.playerGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getPlayerName(self):
    self.validify()
    return library.playerGetPlayerName(self._ptr)
  #\endcond
  ##Player's Name
  playerName = property(getPlayerName)

  #\cond
  def getTime(self):
    self.validify()
    return library.playerGetTime(self._ptr)
  #\endcond
  ##Time remaining, updated at start of turn
  time = property(getTime)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "playerName: %s\n" % self.getPlayerName()
    ret += "time: %s\n" % self.getTime()
    return ret

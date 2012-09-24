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

  ##Eat plant or creature at input location
  def eat(self, x, y):
    self.validify()
    return library.creatureEat(self._ptr, x, y)

  ##Breed with target adjacent creature. Spawn new creature at input location
  def breed(self, mate, x, y):
    self.validify()
    if not isinstance(mate, Creature):
      raise TypeError('mate should be of [Creature]')
    mate.validify()
    return library.creatureBreed(self._ptr, mate._ptr, x, y)

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
  def getMaxEnergy(self):
    self.validify()
    return library.creatureGetMaxEnergy(self._ptr)
  #\endcond
  ##The maximum amount of energy this creature can have
  maxEnergy = property(getMaxEnergy)

  #\cond
  def getEnergyLeft(self):
    self.validify()
    return library.creatureGetEnergyLeft(self._ptr)
  #\endcond
  ##The current amount of energy this creature has.
  energyLeft = property(getEnergyLeft)

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
  def getMovementLeft(self):
    self.validify()
    return library.creatureGetMovementLeft(self._ptr)
  #\endcond
  ##The amount of moves this creature has left this turn
  movementLeft = property(getMovementLeft)

  #\cond
  def getDefense(self):
    self.validify()
    return library.creatureGetDefense(self._ptr)
  #\endcond
  ##The defense of the creature
  defense = property(getDefense)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "owner: %s\n" % self.getOwner()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    ret += "maxEnergy: %s\n" % self.getMaxEnergy()
    ret += "energyLeft: %s\n" % self.getEnergyLeft()
    ret += "carnivorism: %s\n" % self.getCarnivorism()
    ret += "herbivorism: %s\n" % self.getHerbivorism()
    ret += "speed: %s\n" % self.getSpeed()
    ret += "movementLeft: %s\n" % self.getMovementLeft()
    ret += "defense: %s\n" % self.getDefense()
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


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    ret += "size: %s\n" % self.getSize()
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

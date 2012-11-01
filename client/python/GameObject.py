# -*- python -*-

from library import library

from ExistentialError import ExistentialError

class GameObject(object):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration


##A mappable object!
class Mappable(GameObject):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.mappableGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.mappables:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  #\cond
  def getId(self):
    self.validify()
    return library.mappableGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getX(self):
    self.validify()
    return library.mappableGetX(self._ptr)
  #\endcond
  ##X position of the object
  x = property(getX)

  #\cond
  def getY(self):
    self.validify()
    return library.mappableGetY(self._ptr)
  #\endcond
  ##Y position of the object
  y = property(getY)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    return ret

##A basic creature!
class Creature(Mappable):
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
  def breed(self, mate):
    self.validify()
    if not isinstance(mate, Creature):
      raise TypeError('mate should be of [Creature]')
    mate.validify()
    return library.creatureBreed(self._ptr, mate._ptr)

  #\cond
  def getId(self):
    self.validify()
    return library.creatureGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getX(self):
    self.validify()
    return library.creatureGetX(self._ptr)
  #\endcond
  ##X position of the object
  x = property(getX)

  #\cond
  def getY(self):
    self.validify()
    return library.creatureGetY(self._ptr)
  #\endcond
  ##Y position of the object
  y = property(getY)

  #\cond
  def getOwner(self):
    self.validify()
    return library.creatureGetOwner(self._ptr)
  #\endcond
  ##The owner of the creature
  owner = property(getOwner)

  #\cond
  def getMaxHelth(self):
    self.validify()
    return library.creatureGetMaxHelth(self._ptr)
  #\endcond
  ##The maximum amount of health this creature can have
  maxHealth = property(getMaxHelth)

  #\cond
  def getCurrentHealth(self):
    self.validify()
    return library.creatureGetCurrentHealth(self._ptr)
  #\endcond
  ##The current amount of health that this creature has.
  currentHealth = property(getCurrentHealth)

  #\cond
  def getEnergy(self):
    self.validify()
    return library.creatureGetEnergy(self._ptr)
  #\endcond
  ##The energy level of the creature. This helps your max health
  energy = property(getEnergy)

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

  #\cond
  def getCanEat(self):
    self.validify()
    return library.creatureGetCanEat(self._ptr)
  #\endcond
  ##Indicated whether or not this creature can eat this turn.
  canEat = property(getCanEat)

  #\cond
  def getCanBreed(self):
    self.validify()
    return library.creatureGetCanBreed(self._ptr)
  #\endcond
  ##Indicated whether or not this creature can breed this turn.
  canBreed = property(getCanBreed)

  #\cond
  def getParentID(self):
    self.validify()
    return library.creatureGetParentID(self._ptr)
  #\endcond
  ##ID of the creature that gave birth to this one.
  parentID = property(getParentID)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    ret += "owner: %s\n" % self.getOwner()
    ret += "maxHealth: %s\n" % self.getMaxHelth()
    ret += "currentHealth: %s\n" % self.getCurrentHealth()
    ret += "energy: %s\n" % self.getEnergy()
    ret += "carnivorism: %s\n" % self.getCarnivorism()
    ret += "herbivorism: %s\n" % self.getHerbivorism()
    ret += "speed: %s\n" % self.getSpeed()
    ret += "movementLeft: %s\n" % self.getMovementLeft()
    ret += "defense: %s\n" % self.getDefense()
    ret += "canEat: %s\n" % self.getCanEat()
    ret += "canBreed: %s\n" % self.getCanBreed()
    ret += "parentID: %s\n" % self.getParentID()
    return ret

##A basic plant!
class Plant(Mappable):
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
  ##X position of the object
  x = property(getX)

  #\cond
  def getY(self):
    self.validify()
    return library.plantGetY(self._ptr)
  #\endcond
  ##Y position of the object
  y = property(getY)

  #\cond
  def getSize(self):
    self.validify()
    return library.plantGetSize(self._ptr)
  #\endcond
  ##The size of the plant
  size = property(getSize)

  #\cond
  def getGrowthRate(self):
    self.validify()
    return library.plantGetGrowthRate(self._ptr)
  #\endcond
  ##The number of turns it takes this plant to grow in size.
  growthRate = property(getGrowthRate)

  #\cond
  def getTurnsUntilGrowth(self):
    self.validify()
    return library.plantGetTurnsUntilGrowth(self._ptr)
  #\endcond
  ##The number of turns left until this plant will grow again.
  turnsUntilGrowth = property(getTurnsUntilGrowth)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    ret += "size: %s\n" % self.getSize()
    ret += "growthRate: %s\n" % self.getGrowthRate()
    ret += "turnsUntilGrowth: %s\n" % self.getTurnsUntilGrowth()
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

class Creature:
  def __init__(self, game, id, owner, x, y, health, hunger, carnivorism, herbivorism, speed, stamina, defense, age):
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
    self.stamina = stamina
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
      self.stamina,
      self.defense,
      self.age,
      ]
    return value

  def nextTurn(self):
    pass

  def move(self, x, y):
    pass

  def eat(self, plant):
    pass

  def breed(self, mate, x, y):
    pass

  def eat(self, target):
    pass



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




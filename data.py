# -*- coding: iso-8859-1 -*-
from structures import *

aspects = ['timer']

gameName = "Galapagos"

globals = [
  Variable('turnNumber', int, 'How many turns it has been since the beginning of the round'),
  Variable('playerID', int, 'Player Number; either 0 or 1'),
  Variable('gameNumber', int, 'What number game this is for the server'),
  Variable('mapWidth', int, 'The width of the map'),
  Variable('mapHeight', int, 'The height of the map')
]

constants = [
]

playerData = [
]

playerFunctions = [
  Function('talk', [Variable('message', str)], doc='Allows a player to display messages on the screen'),
]

Plant = Model('Plant',
  data=[
    Variable('x', int, 'X position of the plant'),
    Variable('y', int, 'Y position of the plant'),
    Variable('size', int, 'The size of the plant'),
  ],
  doc="A basic plant!",
)

Creature = Model('Creature',
  data=[
    Variable('owner', int, 'The owner of the creature'),
    Variable('x', int, 'X position of the creature'),
    Variable('y', int, 'Y position of the creature'),
    Variable('maxEnergy', int, 'The maximum amount of energy this creature can have'),
    Variable('energyLeft', int, 'The current amount of energy this creature has.'),
    Variable('carnivorism', int, 'The carnivore level of the creature'),
    Variable('herbivorism', int, 'The herbivore level of the creature'),
    Variable('speed', int, 'The speed of the creature'),
    Variable('movementLeft', int, 'The amount of moves this creature has left this turn'),
    Variable('defense', int, 'The defense of the creature'),
    Variable('canAttack', int, 'Indicated whether or not this creature can attack this turn.'),
    Variable('canBreed', int, 'Indicated whether or not this creature can breed this turn.'),
    Variable('parentID', int, 'ID of the creature that gave birth to this one.'),
  ],
  functions=[ 
    Function('move', [Variable('x', int), Variable('y', int)], 
    doc='Command a creature to move to a specified position'),
    Function('eat', [Variable('x', int), Variable('y', int)], 
    doc='Eat plant or creature at input location'),
  ],
  doc="A basic creature!",
)

Creature.addFunctions(
  [
    Function ('breed', [Variable('mate', Creature)], 
    doc='Breed with target adjacent creature. Spawn new creature at input location'),
  ]
)

playerTalk = Animation('playerTalk',
  data=[
    Variable('actingID', int),
    Variable('message', str),
  ],
)

move = Animation('move',
  data=[
    Variable('actingID', int),
    Variable('fromX', int),
    Variable('fromY', int),
    Variable('toX', int),
    Variable('toY', int),
  ],
)

eat = Animation('eat',
  data=[
    Variable('actingID', int),
    Variable('targetID', int),
  ],
)

death = Animation('death',
  data=[
    Variable('actingID', int)
  ],
)

breed = Animation('breed',
  data=[
    Variable('actingID', int),
    Variable('targetID', int),
    Variable('childID', int)
  ],
)
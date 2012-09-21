# -*- coding: iso-8859-1 -*-
from structures import *

aspects = ['timer']

gameName = "Galapagos"

globals = [ Variable('turnNumber', int, 'How many turns it has been since the beginning of the round'),
  Variable('playerID', int, 'Player Number; either 0 or 1'),
]

constants = [
  ]
  
playerData = []

playerFunctions = [
  Function('talk', [Variable('message', str)], doc='Allows a player to display messages on the screen'),
  ]


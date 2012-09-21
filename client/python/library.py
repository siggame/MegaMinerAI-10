# -*-python-*-

import os

from ctypes import *

try:
  if os.name == 'posix':
    library = CDLL("./libclient.so")
  elif os.name == 'nt':
    library = CDLL("./client.dll")
  else:
    raise Exception("Unrecognized OS: "+os.name)
except OSError:
  raise Exception("It looks like you didn't build libclient. Run 'make' and try again.")

# commands

library.createConnection.restype = c_void_p
library.createConnection.argtypes = []

library.serverConnect.restype = c_int
library.serverConnect.argtypes = [c_void_p, c_char_p, c_char_p]

library.serverLogin.restype = c_int
library.serverLogin.argtypes = [c_void_p, c_char_p, c_char_p]

library.createGame.restype = c_int
library.createGame.argtypes = [c_void_p]

library.joinGame.restype = c_int
library.joinGame.argtypes = [c_void_p, c_int, c_char_p]

library.endTurn.restype = None
library.endTurn.argtypes = [c_void_p]

library.getStatus.restype = None
library.getStatus.argtypes = [c_void_p]

library.networkLoop.restype = c_int
library.networkLoop.argtypes = [c_void_p]

#Functions
library.creatureMove.restype = c_int
library.creatureMove.argtypes = [c_void_p, c_int, c_int]

library.creatureEat.restype = c_int
library.creatureEat.argtypes = [c_void_p, c_void_p]

library.creatureBreed.restype = c_int
library.creatureBreed.argtypes = [c_void_p, c_void_p, c_int, c_int]

library.creatureEat.restype = c_int
library.creatureEat.argtypes = [c_void_p, c_void_p]

library.playerTalk.restype = c_int
library.playerTalk.argtypes = [c_void_p, c_char_p]

# accessors

#Globals
library.getTurnNumber.restype = c_int
library.getTurnNumber.argtypes = [c_void_p]

library.getPlayerID.restype = c_int
library.getPlayerID.argtypes = [c_void_p]

library.getMapSize.restype = c_int
library.getMapSize.argtypes = [c_void_p]

library.getCreature.restype = c_void_p
library.getCreature.argtypes = [c_void_p, c_int]

library.getCreatureCount.restype = c_int
library.getCreatureCount.argtypes = [c_void_p]

library.getPlant.restype = c_void_p
library.getPlant.argtypes = [c_void_p, c_int]

library.getPlantCount.restype = c_int
library.getPlantCount.argtypes = [c_void_p]

library.getPlayer.restype = c_void_p
library.getPlayer.argtypes = [c_void_p, c_int]

library.getPlayerCount.restype = c_int
library.getPlayerCount.argtypes = [c_void_p]

# getters

#Data
library.creatureGetId.restype = c_int
library.creatureGetId.argtypes = [c_void_p]

library.creatureGetOwner.restype = c_int
library.creatureGetOwner.argtypes = [c_void_p]

library.creatureGetX.restype = c_int
library.creatureGetX.argtypes = [c_void_p]

library.creatureGetY.restype = c_int
library.creatureGetY.argtypes = [c_void_p]

library.creatureGetHealth.restype = c_int
library.creatureGetHealth.argtypes = [c_void_p]

library.creatureGetHunger.restype = c_int
library.creatureGetHunger.argtypes = [c_void_p]

library.creatureGetCarnivorism.restype = c_int
library.creatureGetCarnivorism.argtypes = [c_void_p]

library.creatureGetHerbivorism.restype = c_int
library.creatureGetHerbivorism.argtypes = [c_void_p]

library.creatureGetSpeed.restype = c_int
library.creatureGetSpeed.argtypes = [c_void_p]

library.creatureGetStamina.restype = c_int
library.creatureGetStamina.argtypes = [c_void_p]

library.creatureGetDefense.restype = c_int
library.creatureGetDefense.argtypes = [c_void_p]

library.creatureGetAge.restype = c_int
library.creatureGetAge.argtypes = [c_void_p]

library.plantGetId.restype = c_int
library.plantGetId.argtypes = [c_void_p]

library.plantGetX.restype = c_int
library.plantGetX.argtypes = [c_void_p]

library.plantGetY.restype = c_int
library.plantGetY.argtypes = [c_void_p]

library.plantGetSize.restype = c_int
library.plantGetSize.argtypes = [c_void_p]

library.plantGetAge.restype = c_int
library.plantGetAge.argtypes = [c_void_p]

library.playerGetId.restype = c_int
library.playerGetId.argtypes = [c_void_p]

library.playerGetPlayerName.restype = c_char_p
library.playerGetPlayerName.argtypes = [c_void_p]

library.playerGetTime.restype = c_float
library.playerGetTime.argtypes = [c_void_p]


#Properties

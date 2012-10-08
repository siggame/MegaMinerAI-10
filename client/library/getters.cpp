#include "getters.h"

DLLEXPORT int creatureGetId(_Creature* ptr)
{
  return ptr->id;
}
DLLEXPORT int creatureGetOwner(_Creature* ptr)
{
  return ptr->owner;
}
DLLEXPORT int creatureGetX(_Creature* ptr)
{
  return ptr->x;
}
DLLEXPORT int creatureGetY(_Creature* ptr)
{
  return ptr->y;
}
DLLEXPORT int creatureGetMaxEnergy(_Creature* ptr)
{
  return ptr->maxEnergy;
}
DLLEXPORT int creatureGetEnergyLeft(_Creature* ptr)
{
  return ptr->energyLeft;
}
DLLEXPORT int creatureGetCarnivorism(_Creature* ptr)
{
  return ptr->carnivorism;
}
DLLEXPORT int creatureGetHerbivorism(_Creature* ptr)
{
  return ptr->herbivorism;
}
DLLEXPORT int creatureGetSpeed(_Creature* ptr)
{
  return ptr->speed;
}
DLLEXPORT int creatureGetMovementLeft(_Creature* ptr)
{
  return ptr->movementLeft;
}
DLLEXPORT int creatureGetDefense(_Creature* ptr)
{
  return ptr->defense;
}
DLLEXPORT int creatureGetCanAttack(_Creature* ptr)
{
  return ptr->canAttack;
}
DLLEXPORT int creatureGetCanBreed(_Creature* ptr)
{
  return ptr->canBreed;
}
DLLEXPORT int plantGetId(_Plant* ptr)
{
  return ptr->id;
}
DLLEXPORT int plantGetX(_Plant* ptr)
{
  return ptr->x;
}
DLLEXPORT int plantGetY(_Plant* ptr)
{
  return ptr->y;
}
DLLEXPORT int plantGetSize(_Plant* ptr)
{
  return ptr->size;
}
DLLEXPORT int playerGetId(_Player* ptr)
{
  return ptr->id;
}
DLLEXPORT char* playerGetPlayerName(_Player* ptr)
{
  return ptr->playerName;
}
DLLEXPORT float playerGetTime(_Player* ptr)
{
  return ptr->time;
}


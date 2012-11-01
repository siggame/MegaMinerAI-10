#include "getters.h"

DLLEXPORT int mappableGetId(_Mappable* ptr)
{
  return ptr->id;
}
DLLEXPORT int mappableGetX(_Mappable* ptr)
{
  return ptr->x;
}
DLLEXPORT int mappableGetY(_Mappable* ptr)
{
  return ptr->y;
}
DLLEXPORT int creatureGetId(_Creature* ptr)
{
  return ptr->id;
}
DLLEXPORT int creatureGetX(_Creature* ptr)
{
  return ptr->x;
}
DLLEXPORT int creatureGetY(_Creature* ptr)
{
  return ptr->y;
}
DLLEXPORT int creatureGetOwner(_Creature* ptr)
{
  return ptr->owner;
}
DLLEXPORT int creatureGetMaxHelth(_Creature* ptr)
{
  return ptr->maxHealth;
}
DLLEXPORT int creatureGetCurrentHealth(_Creature* ptr)
{
  return ptr->currentHealth;
}
DLLEXPORT int creatureGetEnergy(_Creature* ptr)
{
  return ptr->energy;
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
DLLEXPORT int creatureGetCanEat(_Creature* ptr)
{
  return ptr->canEat;
}
DLLEXPORT int creatureGetCanBreed(_Creature* ptr)
{
  return ptr->canBreed;
}
DLLEXPORT int creatureGetParentID(_Creature* ptr)
{
  return ptr->parentID;
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
DLLEXPORT int plantGetGrowthRate(_Plant* ptr)
{
  return ptr->growthRate;
}
DLLEXPORT int plantGetTurnsUntilGrowth(_Plant* ptr)
{
  return ptr->turnsUntilGrowth;
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


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
DLLEXPORT int creatureGetHealth(_Creature* ptr)
{
  return ptr->health;
}
DLLEXPORT int creatureGetHunger(_Creature* ptr)
{
  return ptr->hunger;
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
DLLEXPORT int creatureGetMaxStamina(_Creature* ptr)
{
  return ptr->maxStamina;
}
DLLEXPORT int creatureGetDefense(_Creature* ptr)
{
  return ptr->defense;
}
DLLEXPORT int creatureGetAge(_Creature* ptr)
{
  return ptr->age;
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
DLLEXPORT int plantGetAge(_Plant* ptr)
{
  return ptr->age;
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


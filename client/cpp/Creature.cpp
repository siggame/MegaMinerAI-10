// -*-c++-*-

#include "Creature.h"
#include "game.h"

#include "Creature.h"

Creature::Creature(_Creature* pointer)
{
    ptr = (void*) pointer;
}

int Creature::id()
{
  return ((_Creature*)ptr)->id;
}

int Creature::owner()
{
  return ((_Creature*)ptr)->owner;
}

int Creature::x()
{
  return ((_Creature*)ptr)->x;
}

int Creature::y()
{
  return ((_Creature*)ptr)->y;
}

int Creature::maxEnergy()
{
  return ((_Creature*)ptr)->maxEnergy;
}

int Creature::energyLeft()
{
  return ((_Creature*)ptr)->energyLeft;
}

int Creature::carnivorism()
{
  return ((_Creature*)ptr)->carnivorism;
}

int Creature::herbivorism()
{
  return ((_Creature*)ptr)->herbivorism;
}

int Creature::speed()
{
  return ((_Creature*)ptr)->speed;
}

int Creature::movementLeft()
{
  return ((_Creature*)ptr)->movementLeft;
}

int Creature::defense()
{
  return ((_Creature*)ptr)->defense;
}

int Creature::canAttack()
{
  return ((_Creature*)ptr)->canAttack;
}

int Creature::canBreed()
{
  return ((_Creature*)ptr)->canBreed;
}

int Creature::parentID()
{
  return ((_Creature*)ptr)->parentID;
}


int Creature::move(int x, int y)
{
  return creatureMove( (_Creature*)ptr, x, y);
}

int Creature::eat(int x, int y)
{
  return creatureEat( (_Creature*)ptr, x, y);
}

int Creature::breed(Creature& mate)
{
  return creatureBreed( (_Creature*)ptr, (_Creature*) mate.ptr);
}



std::ostream& operator<<(std::ostream& stream,Creature ob)
{
  stream << "id: " << ((_Creature*)ob.ptr)->id  <<'\n';
  stream << "owner: " << ((_Creature*)ob.ptr)->owner  <<'\n';
  stream << "x: " << ((_Creature*)ob.ptr)->x  <<'\n';
  stream << "y: " << ((_Creature*)ob.ptr)->y  <<'\n';
  stream << "maxEnergy: " << ((_Creature*)ob.ptr)->maxEnergy  <<'\n';
  stream << "energyLeft: " << ((_Creature*)ob.ptr)->energyLeft  <<'\n';
  stream << "carnivorism: " << ((_Creature*)ob.ptr)->carnivorism  <<'\n';
  stream << "herbivorism: " << ((_Creature*)ob.ptr)->herbivorism  <<'\n';
  stream << "speed: " << ((_Creature*)ob.ptr)->speed  <<'\n';
  stream << "movementLeft: " << ((_Creature*)ob.ptr)->movementLeft  <<'\n';
  stream << "defense: " << ((_Creature*)ob.ptr)->defense  <<'\n';
  stream << "canAttack: " << ((_Creature*)ob.ptr)->canAttack  <<'\n';
  stream << "canBreed: " << ((_Creature*)ob.ptr)->canBreed  <<'\n';
  stream << "parentID: " << ((_Creature*)ob.ptr)->parentID  <<'\n';
  return stream;
}

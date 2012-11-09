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

int Creature::x()
{
  return ((_Creature*)ptr)->x;
}

int Creature::y()
{
  return ((_Creature*)ptr)->y;
}

int Creature::owner()
{
  return ((_Creature*)ptr)->owner;
}

int Creature::maxHealth()
{
  return ((_Creature*)ptr)->maxHealth;
}

int Creature::currentHealth()
{
  return ((_Creature*)ptr)->currentHealth;
}

int Creature::energy()
{
  return ((_Creature*)ptr)->energy;
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

int Creature::canEat()
{
  return ((_Creature*)ptr)->canEat;
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
  stream << "x: " << ((_Creature*)ob.ptr)->x  <<'\n';
  stream << "y: " << ((_Creature*)ob.ptr)->y  <<'\n';
  stream << "owner: " << ((_Creature*)ob.ptr)->owner  <<'\n';
  stream << "maxHealth: " << ((_Creature*)ob.ptr)->maxHealth  <<'\n';
  stream << "currentHealth: " << ((_Creature*)ob.ptr)->currentHealth  <<'\n';
  stream << "energy: " << ((_Creature*)ob.ptr)->energy  <<'\n';
  stream << "carnivorism: " << ((_Creature*)ob.ptr)->carnivorism  <<'\n';
  stream << "herbivorism: " << ((_Creature*)ob.ptr)->herbivorism  <<'\n';
  stream << "speed: " << ((_Creature*)ob.ptr)->speed  <<'\n';
  stream << "movementLeft: " << ((_Creature*)ob.ptr)->movementLeft  <<'\n';
  stream << "defense: " << ((_Creature*)ob.ptr)->defense  <<'\n';
  stream << "canEat: " << ((_Creature*)ob.ptr)->canEat  <<'\n';
  stream << "canBreed: " << ((_Creature*)ob.ptr)->canBreed  <<'\n';
  stream << "parentID: " << ((_Creature*)ob.ptr)->parentID  <<'\n';
  return stream;
}

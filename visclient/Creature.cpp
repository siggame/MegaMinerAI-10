// -*-c++-*-

#include "Creature.h"
#include "game.h"

#include "Plant.h"
#include "Creature.h"

namespace client
{

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

int Creature::health()
{
  return ((_Creature*)ptr)->health;
}

int Creature::hunger()
{
  return ((_Creature*)ptr)->hunger;
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

int Creature::maxStamina()
{
  return ((_Creature*)ptr)->maxStamina;
}

int Creature::defense()
{
  return ((_Creature*)ptr)->defense;
}

int Creature::age()
{
  return ((_Creature*)ptr)->age;
}


int Creature::move(int x, int y)
{
  return creatureMove( (_Creature*)ptr, x, y);
}

int Creature::eat(Plant& plant)
{
  return creatureEat( (_Creature*)ptr, (_Plant*) plant.ptr);
}

int Creature::breed(Creature& mate, int x, int y)
{
  return creatureBreed( (_Creature*)ptr, (_Creature*) mate.ptr, x, y);
}

int Creature::eat(Creature& target)
{
  return creatureEat( (_Creature*)ptr, (_Creature*) target.ptr);
}



std::ostream& operator<<(std::ostream& stream,Creature ob)
{
  stream << "id: " << ((_Creature*)ob.ptr)->id  <<'\n';
  stream << "owner: " << ((_Creature*)ob.ptr)->owner  <<'\n';
  stream << "x: " << ((_Creature*)ob.ptr)->x  <<'\n';
  stream << "y: " << ((_Creature*)ob.ptr)->y  <<'\n';
  stream << "health: " << ((_Creature*)ob.ptr)->health  <<'\n';
  stream << "hunger: " << ((_Creature*)ob.ptr)->hunger  <<'\n';
  stream << "carnivorism: " << ((_Creature*)ob.ptr)->carnivorism  <<'\n';
  stream << "herbivorism: " << ((_Creature*)ob.ptr)->herbivorism  <<'\n';
  stream << "speed: " << ((_Creature*)ob.ptr)->speed  <<'\n';
  stream << "maxStamina: " << ((_Creature*)ob.ptr)->maxStamina  <<'\n';
  stream << "defense: " << ((_Creature*)ob.ptr)->defense  <<'\n';
  stream << "age: " << ((_Creature*)ob.ptr)->age  <<'\n';
  return stream;
}

}

// -*-c++-*-

#include "structures.h"

#include <iostream>

namespace parser
{


std::ostream& operator<<(std::ostream& stream, Creature ob)
{
  stream << "id: " << ob.id  <<'\n';
  stream << "owner: " << ob.owner  <<'\n';
  stream << "x: " << ob.x  <<'\n';
  stream << "y: " << ob.y  <<'\n';
  stream << "maxEnergy: " << ob.maxEnergy  <<'\n';
  stream << "energyLeft: " << ob.energyLeft  <<'\n';
  stream << "carnivorism: " << ob.carnivorism  <<'\n';
  stream << "herbivorism: " << ob.herbivorism  <<'\n';
  stream << "speed: " << ob.speed  <<'\n';
  stream << "movementLeft: " << ob.movementLeft  <<'\n';
  stream << "defense: " << ob.defense  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, Plant ob)
{
  stream << "id: " << ob.id  <<'\n';
  stream << "x: " << ob.x  <<'\n';
  stream << "y: " << ob.y  <<'\n';
  stream << "size: " << ob.size  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, Player ob)
{
  stream << "id: " << ob.id  <<'\n';
  stream << "playerName: " << ob.playerName  <<'\n';
  stream << "time: " << ob.time  <<'\n';
  return stream;
}



std::ostream& operator<<(std::ostream& stream, move ob)
{
  stream << "move" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "fromX: " << ob.fromX  <<'\n';
  stream << "fromY: " << ob.fromY  <<'\n';
  stream << "toX: " << ob.toX  <<'\n';
  stream << "toY: " << ob.toY  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, eat ob)
{
  stream << "eat" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "targetID: " << ob.targetID  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, breed ob)
{
  stream << "breed" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "targetID: " << ob.targetID  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, playerTalk ob)
{
  stream << "playerTalk" << "\n";
  stream << "actingID: " << ob.actingID  <<'\n';
  stream << "message: " << ob.message  <<'\n';
  return stream;
}


std::ostream& operator<<(std::ostream& stream, GameState ob)
{
  stream << "turnNumber: " << ob.turnNumber  <<'\n';
  stream << "playerID: " << ob.playerID  <<'\n';
  stream << "mapSize: " << ob.mapSize  <<'\n';

  stream << "\n\nCreatures:\n";
  for(std::map<int,Creature>::iterator i = ob.creatures.begin(); i != ob.creatures.end(); i++)
    stream << i->second << '\n';
  stream << "\n\nPlants:\n";
  for(std::map<int,Plant>::iterator i = ob.plants.begin(); i != ob.plants.end(); i++)
    stream << i->second << '\n';
  stream << "\n\nPlayers:\n";
  for(std::map<int,Player>::iterator i = ob.players.begin(); i != ob.players.end(); i++)
    stream << i->second << '\n';
  stream << "\nAnimation\n";
  for
    (
    std::map< int, std::vector< SmartPointer< Animation > > >::iterator j = ob.animations.begin(); 
    j != ob.animations.end(); 
    j++ 
    )
  {
  for(std::vector< SmartPointer< Animation > >::iterator i = j->second.begin(); i != j->second.end(); i++)
  {
//    if((*(*i)).type == MOVE)
//      stream << *((move*)*i) << "\n";
//    if((*(*i)).type == EAT)
//      stream << *((eat*)*i) << "\n";
//    if((*(*i)).type == BREED)
//      stream << *((breed*)*i) << "\n";
//    if((*(*i)).type == PLAYERTALK)
//      stream << *((playerTalk*)*i) << "\n";
  }
  

  }
  return stream;
}

Game::Game()
{
  winner = -1;
}

} // parser

#include "parser.h"
#include "sexp/sexp.h"
#include "sexp/parser.h"
#include "sexp/sfcompat.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

using namespace std;

namespace parser
{

char *ToLower( char *str )
{
  for( int i = 0; i < strlen( str ); i++ )
  {
    str[ i ] = tolower( str[ i ] );
  }
  return str;
}


static bool parseMappable(Mappable& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  sub = expression->list;

  if ( !sub ) 
  {
    cerr << "Error in parseMappable.\n Parsing: " << *expression << endl;
    return false;
  }

  object.id = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseMappable.\n Parsing: " << *expression << endl;
    return false;
  }

  object.x = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseMappable.\n Parsing: " << *expression << endl;
    return false;
  }

  object.y = atoi(sub->val);
  sub = sub->next;

  return true;

}
static bool parseCreature(Creature& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  sub = expression->list;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.id = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.x = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.y = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.owner = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.maxHealth = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.currentHealth = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.energy = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.carnivorism = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.herbivorism = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.speed = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.movementLeft = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.defense = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.canEat = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.canBreed = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parseCreature.\n Parsing: " << *expression << endl;
    return false;
  }

  object.parentID = atoi(sub->val);
  sub = sub->next;

  return true;

}
static bool parsePlant(Plant& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  sub = expression->list;

  if ( !sub ) 
  {
    cerr << "Error in parsePlant.\n Parsing: " << *expression << endl;
    return false;
  }

  object.id = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parsePlant.\n Parsing: " << *expression << endl;
    return false;
  }

  object.x = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parsePlant.\n Parsing: " << *expression << endl;
    return false;
  }

  object.y = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parsePlant.\n Parsing: " << *expression << endl;
    return false;
  }

  object.size = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parsePlant.\n Parsing: " << *expression << endl;
    return false;
  }

  object.growthRate = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parsePlant.\n Parsing: " << *expression << endl;
    return false;
  }

  object.turnsUntilGrowth = atoi(sub->val);
  sub = sub->next;

  return true;

}
static bool parsePlayer(Player& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  sub = expression->list;

  if ( !sub ) 
  {
    cerr << "Error in parsePlayer.\n Parsing: " << *expression << endl;
    return false;
  }

  object.id = atoi(sub->val);
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parsePlayer.\n Parsing: " << *expression << endl;
    return false;
  }

  object.playerName = new char[strlen(sub->val)+1];
  strncpy(object.playerName, sub->val, strlen(sub->val));
  object.playerName[strlen(sub->val)] = 0;
  sub = sub->next;

  if ( !sub ) 
  {
    cerr << "Error in parsePlayer.\n Parsing: " << *expression << endl;
    return false;
  }

  object.time = atof(sub->val);
  sub = sub->next;

  return true;

}

static bool parseMove(move& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  object.type = MOVE;
  sub = expression->list->next;
  if( !sub ) 
  {
    cerr << "Error in parsemove.\n Parsing: " << *expression << endl;
    return false;
  }
  object.actingID = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parsemove.\n Parsing: " << *expression << endl;
    return false;
  }
  object.fromX = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parsemove.\n Parsing: " << *expression << endl;
    return false;
  }
  object.fromY = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parsemove.\n Parsing: " << *expression << endl;
    return false;
  }
  object.toX = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parsemove.\n Parsing: " << *expression << endl;
    return false;
  }
  object.toY = atoi(sub->val);
  sub = sub->next;
  return true;

}
static bool parsePlayerTalk(playerTalk& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  object.type = PLAYERTALK;
  sub = expression->list->next;
  if( !sub ) 
  {
    cerr << "Error in parseplayerTalk.\n Parsing: " << *expression << endl;
    return false;
  }
  object.actingID = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parseplayerTalk.\n Parsing: " << *expression << endl;
    return false;
  }
  object.message = new char[strlen(sub->val)+1];
  strncpy(object.message, sub->val, strlen(sub->val));
  object.message[strlen(sub->val)] = 0;
  sub = sub->next;
  return true;

}
static bool parseDeath(death& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  object.type = DEATH;
  sub = expression->list->next;
  if( !sub ) 
  {
    cerr << "Error in parsedeath.\n Parsing: " << *expression << endl;
    return false;
  }
  object.actingID = atoi(sub->val);
  sub = sub->next;
  return true;

}
static bool parseEat(eat& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  object.type = EAT;
  sub = expression->list->next;
  if( !sub ) 
  {
    cerr << "Error in parseeat.\n Parsing: " << *expression << endl;
    return false;
  }
  object.actingID = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parseeat.\n Parsing: " << *expression << endl;
    return false;
  }
  object.targetID = atoi(sub->val);
  sub = sub->next;
  return true;

}
static bool parseBreed(breed& object, sexp_t* expression)
{
  sexp_t* sub;
  if ( !expression ) return false;
  object.type = BREED;
  sub = expression->list->next;
  if( !sub ) 
  {
    cerr << "Error in parsebreed.\n Parsing: " << *expression << endl;
    return false;
  }
  object.actingID = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parsebreed.\n Parsing: " << *expression << endl;
    return false;
  }
  object.targetID = atoi(sub->val);
  sub = sub->next;
  if( !sub ) 
  {
    cerr << "Error in parsebreed.\n Parsing: " << *expression << endl;
    return false;
  }
  object.childID = atoi(sub->val);
  sub = sub->next;
  return true;

}

static bool parseSexp(Game& game, sexp_t* expression)
{
  sexp_t* sub, *subsub;
  if( !expression ) return false;
  expression = expression->list;
  if( !expression ) return false;
  if(expression->val != NULL && strcmp(expression->val, "status") == 0)
  {
    GameState gs;
    while(expression->next != NULL)
    {
      expression = expression->next;
      sub = expression->list;
      if ( !sub ) return false;
      if(string(sub->val) == "game")
      {
          sub = sub->next;
          if ( !sub ) return false;
          gs.turnNumber = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.playerID = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.gameNumber = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.mapWidth = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.mapHeight = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.healthPerBreed = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.healthPerMove = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.healthPerTurn = atoi(sub->val);
          sub = sub->next;
          if ( !sub ) return false;
          gs.baseHealth = atoi(sub->val);
          sub = sub->next;
      }
      else if(string(sub->val) == "Mappable")
      {
        sub = sub->next;
        bool flag = true;
        while(sub && flag)
        {
          Mappable object;
          flag = parseMappable(object, sub);
          gs.mappables[object.id] = object;
          sub = sub->next;
        }
        if ( !flag ) return false;
      }
      else if(string(sub->val) == "Creature")
      {
        sub = sub->next;
        bool flag = true;
        while(sub && flag)
        {
          Creature object;
          flag = parseCreature(object, sub);
          gs.creatures[object.id] = object;
          sub = sub->next;
        }
        if ( !flag ) return false;
      }
      else if(string(sub->val) == "Plant")
      {
        sub = sub->next;
        bool flag = true;
        while(sub && flag)
        {
          Plant object;
          flag = parsePlant(object, sub);
          gs.plants[object.id] = object;
          sub = sub->next;
        }
        if ( !flag ) return false;
      }
      else if(string(sub->val) == "Player")
      {
        sub = sub->next;
        bool flag = true;
        while(sub && flag)
        {
          Player object;
          flag = parsePlayer(object, sub);
          gs.players[object.id] = object;
          sub = sub->next;
        }
        if ( !flag ) return false;
      }
    }
    game.states.push_back(gs);
  }
  else if(string(expression->val) == "animations")
  {
    std::map< int, std::vector< SmartPointer< Animation > > > animations;
    while(expression->next)
    {
      expression = expression->next;
      sub = expression->list;
      if ( !sub ) return false;
      if(string(ToLower( sub->val ) ) == "move")
      {
        SmartPointer<move> animation = new move;
        if ( !parseMove(*animation, expression) )
          return false;

        animations[ ((AnimOwner*)&*animation)->owner ].push_back( animation );
      }
      if(string(ToLower( sub->val ) ) == "player-talk")
      {
        SmartPointer<playerTalk> animation = new playerTalk;
        if ( !parsePlayerTalk(*animation, expression) )
          return false;

        animations[ ((AnimOwner*)&*animation)->owner ].push_back( animation );
      }
      if(string(ToLower( sub->val ) ) == "death")
      {
        SmartPointer<death> animation = new death;
        if ( !parseDeath(*animation, expression) )
          return false;

        animations[ ((AnimOwner*)&*animation)->owner ].push_back( animation );
      }
      if(string(ToLower( sub->val ) ) == "eat")
      {
        SmartPointer<eat> animation = new eat;
        if ( !parseEat(*animation, expression) )
          return false;

        animations[ ((AnimOwner*)&*animation)->owner ].push_back( animation );
      }
      if(string(ToLower( sub->val ) ) == "breed")
      {
        SmartPointer<breed> animation = new breed;
        if ( !parseBreed(*animation, expression) )
          return false;

        animations[ ((AnimOwner*)&*animation)->owner ].push_back( animation );
      }
    }
    game.states[game.states.size()-1].animations = animations;
  }
  else if(string(expression->val) == "ident")
  {
    expression = expression->next;
    if ( !expression ) return false;
    sub = expression->list;
    while(sub)
    {
      subsub = sub->list;
      if ( !subsub ) return false;
      int number = atoi(subsub->val);
      if(number >= 0)
      {
        subsub = subsub->next;
        if ( !subsub ) return false;
        subsub = subsub->next;
        if ( !subsub ) return false;
        game.players[number] = subsub->val;
      }
      sub = sub->next;
    }
  }
  else if(string(expression->val) == "game-winner")
  {
    expression = expression->next;
    if ( !expression ) return false;
    expression = expression->next;
    if ( !expression ) return false;
    expression = expression->next;
    if ( !expression ) return false;
    game.winner = atoi(expression->val);
		expression = expression->next;
		if( !expression ) return false;
		game.winReason = expression->val;
  }

  return true;
}


bool parseFile(Game& game, const char* filename)
{
  //bool value;
  FILE* in = fopen(filename, "r");
  //int size;
  if(!in)
    return false;

  parseFile(in);

  sexp_t* st = NULL;

  while((st = parse()))
  {
    if( !parseSexp(game, st) )
    {
      while(parse()); //empty the file, keep Lex happy.
      fclose(in);
      return false;
    }
    destroy_sexp(st);
  }

  fclose(in);

  return true;
}


bool parseGameFromString(Game& game, const char* string)
{

  parseString( string );

  sexp_t* st = NULL;

  while((st = parse()))
  {
    if( !parseSexp(game, st) )
    {
      while(parse()); //empty the file, keep Lex happy.
      return false;
    }
    destroy_sexp(st);
  }

  return true;
}

} // parser

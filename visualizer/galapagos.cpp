#include "galapagos.h"
#include "galapagosAnimatable.h"
#include "frame.h"
#include "version.h"
#include "animations.h"
#include <utility>
#include <time.h>
#include <list>
#include <glm/glm.hpp>
#include <stack>

namespace visualizer
{
  // todo: move this function elsewhere
  float GetRandFloat(float a, float b)
  {
    float fRand = rand() / (RAND_MAX + 1.0f);
    return fRand*(b - a) + a;
  } 
  
  Galapagos::Galapagos()
  {
    m_game = 0;
    m_suicide=false;
  } // Galapagos::Galapagos()

  Galapagos::~Galapagos()
  {
    destroy();
  }

  void Galapagos::destroy()
  {
    m_suicide=true;
    wait();
    animationEngine->registerGame(0, 0);

    clear();
    delete m_game;
    m_game = 0;
    
    // Clear your memory here
    
    programs.clear();

  } // Galapagos::~Galapagos()
 
  void Galapagos::GetSelectedRect(Rect& R) const
  {
    const Input& input = gui->getInput();
    
    int x = input.x;
    int y = input.y;
    int width = input.sx - x;
    int height = input.sy - y;
    
    int right = x + width;
    int bottom = y + height;
    
    R.left = min(x,right);
    R.top = min(y,bottom);
    R.right = max(x,right);
    R.bottom = max(y,bottom);
  }

  void Galapagos::preDraw()
  {
    const Input& input = gui->getInput();
    if( input.leftRelease )
    {
      int turn = timeManager->getTurn();
      //float t = timeManager->getTurnPercent();
      
      Rect selectedRect;
      GetSelectedRect(selectedRect);
      
      m_selectedUnitIDs.clear();
      
      AddSelectedObjsToList(m_game->states[ turn ].creatures,selectedRect);
      AddSelectedObjsToList(m_game->states[ turn ].plants,selectedRect);
   
      
    }
  }

  void Galapagos::postDraw()
  {
    int turn = timeManager->getTurn();
    
    for(auto iter = m_selectedUnitIDs.begin(); iter != m_selectedUnitIDs.end(); ++iter)
    {
      // If polymorphism was used, we would not have to search both lists.....................
      if(!DrawQuadAroundObj(m_game->states[turn].creatures,*iter))
      {
        DrawQuadAroundObj(m_game->states[turn].plants,*iter);
      }
    }
       
  }


  PluginInfo Galapagos::getPluginInfo()
  {
    PluginInfo i;
    i.searchLength = 1000;
    i.gamelogRegexPattern = "Galapagos";
    i.returnFilename = false;
    i.spectateMode = false;
    i.pluginName = "MegaMinerAI: Galapagos Plugin";


    return i;
  } // PluginInfo Galapagos::getPluginInfo()

  void Galapagos::setup()
  {
    gui->checkForUpdate( "Galapagos", "./plugins/galapagos/checkList.md5", VERSION_FILE );
    options->loadOptionFile( "./plugins/galapagos/galapagos.xml", "galapagos" );
    resourceManager->loadResourceFile( "./plugins/galapagos/resources.r" );
  }
  
  // Give the Debug Info widget the selected object IDs in the Gamelog
  list<int> Galapagos::getSelectedUnits()
  {
    return m_selectedUnitIDs;
  }
  
  void Galapagos::SeedRand() const
  {
    std::hash<std::string> hasher;
    unsigned int seed = hasher(m_game->states[0].players[0].playerName) + hasher(m_game->states[0].players[1].playerName) + m_game->states[0].gameNumber;
    srand(seed);
    
    cout<<"Seed: "<<seed<<endl;
  }

  void Galapagos::loadGamelog( std::string gamelog )
  {
    if(isRunning())
    {
      m_suicide = true;
      wait();
    }
    m_suicide = false;

    // BEGIN: Initial Setup
    setup();

    delete m_game;
    m_game = new parser::Game;

    if( !parser::parseGameFromString( *m_game, gamelog.c_str() ) )
    {
      delete m_game;
      m_game = 0;
      WARNING(
          "Cannot load gamelog, %s", 
          gamelog.c_str()
          );
    }
    // END: Initial Setup
    
    SeedRand();
    
    m_IslandVisualOffset = 1;
    m_GUIHeight = 4;

    //renderer->setCamera( m_IslandVisualOffset, m_IslandVisualOffset, m_game->states[0].mapWidth-m_IslandVisualOffset, m_game->states[0].mapHeight+m_GUIHeight-m_IslandVisualOffset);
    renderer->setCamera( 0, 0, m_game->states[0].mapWidth, m_game->states[0].mapHeight+m_GUIHeight);
    renderer->setGridDimensions( m_game->states[0].mapWidth, m_game->states[0].mapHeight+m_GUIHeight );
    
    start();
  } // Galapagos::loadGamelog()
  
  // The "main" function
  void Galapagos::run()
  {
    
    // Build the Debug Table's Headers
    QStringList header;
    header << "ID" << "Owner" << "X" << "Y" << "Energy" << "Max Energy" << "Carn" << "Herb" << "Speed" << "Defence";
    gui->setDebugHeader( header );
    timeManager->setNumTurns( 0 );

    animationEngine->registerGame(0, 0);
    
    Map* pPrevMap = nullptr;

    std::stack<SmartPointer<Animatable>> turnAni;

    // Look through each turn in the gamelog
    for(int state = 0; state < (int)m_game->states.size() && !m_suicide; state++)
    {
      Frame turn;  // The frame that will be drawn
      SmartPointer<Map> map;

      float mapColor = 0.3f*sin((float)state*0.1f) + 0.5f;
      float halfWidth = m_game->states[state].mapWidth / 2.0f;
      float xPos = halfWidth*sin((float)state*0.1f)+halfWidth;
      
      if(pPrevMap == nullptr)
      {
        map = new Map(m_game->states[state].mapWidth,m_game->states[state].mapHeight,0.6f,mapColor,xPos);
      }
      else
      {
        map = new Map(*pPrevMap,mapColor,xPos);
      }
     
      pPrevMap = map;
 

      map->addKeyFrame( new DrawMap( map ) );
      turn.addAnimatable( map );

      while(!turnAni.empty())
      {
          turn.addAnimatable(turnAni.top());
          turnAni.pop();
      }

      // for each plant in the current turn
      for( auto& p : m_game->states[ state ].plants )
      {
        SmartPointer<Plant> plant = new Plant();
        plant->x = p.second.x;
        plant->y = p.second.y;
        plant->size = p.second.size;
        plant->growth = state > 0 ? p.second.size - m_game->states[ state - 1 ].plants[p.second.id].size : 0;
        plant->addKeyFrame( new DrawPlant( plant ) );
        turn.addAnimatable( plant );
        
        turn[p.second.id]["ID"] = p.second.id;
        turn[p.second.id]["X"] = p.second.x;
        turn[p.second.id]["Y"] = p.second.y;
      }
      
      // for each creature in the current turn
      for( auto& p : m_game->states[ state ].creatures )
      {
      	SmartPointer<Creature> creature = new Creature();

        for(auto& j : m_game->states[state].animations[p.second.id])
        {
            switch(j->type)
            {
                case parser::MOVE:
                {
                    parser::move& move = (parser::move&)*j;
                    creature->m_moves.push_back(Creature::Moves(glm::vec2(move.toX, move.toY),glm::vec2(move.fromX, move.fromY)));

                    // todo: fix this
                    (*map)(move.toY,move.toX) = Map::Tile("sand",state);

                    break;
                }
            }
        }

        if(creature->m_moves.empty())
        {
            creature->m_moves.push_back(Creature::Moves(glm::vec2(p.second.x, p.second.y),glm::vec2(p.second.x, p.second.y)));
        }

        if( (state + 1) < m_game->states.size() )
        {
          if(m_game->states[ state + 1 ].creatures.find( p.second.id ) == m_game->states[ state + 1 ].creatures.end())
          {
              SmartPointer<SpriteAnimation> deathAni = new SpriteAnimation();
              deathAni->x = p.second.x;
              deathAni->y = p.second.y;
              deathAni->frame = 7; // todo: need to make this tweakable
              deathAni->addKeyFrame(new DrawAnimation(deathAni));

              cout<<"Death, energy left: "<<p.second.energyLeft<<" turn: "<<state<<endl;

              turnAni.push(deathAni);
          }

        }

        creature->energyLeft = p.second.energyLeft;
        creature->maxEnergy = p.second.maxEnergy;
        creature->owner = p.second.owner;

        //more stats, not sure how many are needed purely for animation
        creature->carnivorism = p.second.carnivorism;
        creature->herbivorism = p.second.herbivorism;
        creature->speed = p.second.speed;
        creature->movementLeft = p.second.movementLeft;
        creature->defense = p.second.defense;
        creature->canEat = p.second.canEat;
        creature->canBreed = p.second.canBreed;
        creature->parentID = p.second.parentID;

        creature->addKeyFrame( new DrawCreature( creature ) );

        (*map)(p.second.y,p.second.x) = Map::Tile("sand",state);

        turn.addAnimatable( creature );
        
        turn[p.second.id]["ID"] = p.second.id;
        turn[p.second.id]["Owner"] = p.second.owner;
        turn[p.second.id]["X"] = p.second.x;
        turn[p.second.id]["Y"] = p.second.y;
        turn[p.second.id]["Energy"] = p.second.energyLeft;
        turn[p.second.id]["Max Energy"] = p.second.maxEnergy;
        turn[p.second.id]["Carn"] = p.second.carnivorism;
        turn[p.second.id]["Herb"] = p.second.herbivorism;
        turn[p.second.id]["Speed"] = p.second.speed;
        turn[p.second.id]["Defence"] = p.second.defense;
      }
      
      // for each eat animation in the turn, add an EatAnimation to be drawn
      for(auto& creaturesAnims : m_game->states[state].animations)
      {
        for(auto& a : creaturesAnims.second)
        {
          if(a->type == parser::EAT)
          {
            parser::eat& eat = (parser::eat&)*a;
            // the mappables map in the glog isn't currently working :(
            int x = 0, y = 0;
            auto creature = m_game->states[state].creatures.find( eat.targetID );
            if( creature != m_game->states[state].creatures.end() )
            {
              x = creature->second.x;
              y = creature->second.y;
            }
            auto plant = m_game->states[state].plants.find( eat.targetID );
            if( plant != m_game->states[state].plants.end() )
            {
              x = plant->second.x;
              y = plant->second.y;
            }
            //SmartPointer<EatAnimation> eatAnimation = new EatAnimation( m_game->states[ state ].mappables[ eat.targetID ].x, m_game->states[ state ].mappables[ eat.targetID ].y );
            SmartPointer<EatAnimation> eatAnimation = new EatAnimation( x, y );
            eatAnimation->addKeyFrame( new DrawEatAnimation( eatAnimation ) );
            turn.addAnimatable( eatAnimation );
            cout << "added an eat animation at: " << eatAnimation->x << "," << eatAnimation->y << " to from the targetID: " << eat.targetID << endl;
          }
        }
      }

      if( m_game->states.size() == state + 1 )
      {
        SmartPointer<SplashScreen> ss = new SplashScreen();

        ss->winner = m_game->states[0].players[m_game->winner].playerName;
        ss->width = m_game->states[0].mapWidth;
        ss->height = m_game->states[0].mapHeight;

        ss->addKeyFrame( new DrawSplashScreen( ss ) );

        turn.addAnimatable( ss );
      }

      // end of parsing this state in the glog, build the turn
      animationEngine->buildAnimations(turn);
      addFrame(turn);
      
      // Register the game and begin playing delayed due to multithreading
      if(state > 5)
      {
        timeManager->setNumTurns(state - 5);
        animationEngine->registerGame( this, this );
        if(state == 6)
        {
          animationEngine->registerGame(this, this);
          timeManager->setTurn(0);
          timeManager->play();
        }
      }
    }    
    
    if(!m_suicide)
    {
      timeManager->setNumTurns( m_game->states.size() );
      timeManager->play();
    }

  } // Galapagos::run()

} // visualizer

Q_EXPORT_PLUGIN2( Galapagos, visualizer::Galapagos );

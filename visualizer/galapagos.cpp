#include "galapagos.h"
#include "galapagosAnimatable.h"
#include "frame.h"
#include "version.h"
#include "animations.h"
#include <utility>
#include <time.h>
#include <list>

namespace visualizer
{ 
  
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
    int width = input.sx - input.x;
    int height = input.sy - input.y;
    
    int right = input.x+width;
    int bottom = input.y+height;
    
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

    // Setup the renderer as a 4 x 4 map by default
    // TODO: Change board size to something useful
    renderer->setCamera( 0, 0, m_game->states[0].mapWidth, m_game->states[0].mapHeight );
    renderer->setGridDimensions( m_game->states[0].mapWidth, m_game->states[0].mapHeight );
 
    start();
  } // Galapagos::loadGamelog()
  
  // The "main" function
  void Galapagos::run()
  {
    
    // Build the Debug Table's Headers
    QStringList header;
    header << "ID" << "Owner" << "X" << "Y" << "Energy" << "Energy Left" << "Carn" << "Herb" << "Speed" << "Defence";
    gui->setDebugHeader( header );
    timeManager->setNumTurns( 0 );

    animationEngine->registerGame(0, 0);
    
    // Starting color
    float fPrevColor = 0.6f;

    // Look through each turn in the gamelog
    for(int state = 0; state < (int)m_game->states.size() && !m_suicide; state++)
    {
      float halfWidth = m_game->states[state].mapWidth / 2.0f;
    
      Frame turn;  // The frame that will be drawn
      SmartPointer<Map> map = new Map();
      map->width = m_game->states[state].mapWidth;
      map->height = m_game->states[state].mapHeight;
      map->color = 0.3f*sin((float)state*0.1f) + 0.5f;
      map->xPos = halfWidth*sin((float)state*0.1f)+halfWidth;
      map->prevColor = fPrevColor;
      map->addKeyFrame( new DrawMap( map ) );
      turn.addAnimatable( map );
      
      fPrevColor = map->color;
      
      for( auto& p : m_game->states[ state ].plants )
      {
        SmartPointer<Plant> plant = new Plant();
        plant->x = p.second.x;
        plant->y = p.second.y;
        plant->size = p.second.size;
        plant->addKeyFrame( new DrawPlant( plant ) );
        turn.addAnimatable( plant );
        
        turn[p.second.id]["ID"] = p.second.id;
        turn[p.second.id]["X"] = p.second.x;
        turn[p.second.id]["Y"] = p.second.y;
      }
      
      for( auto& p : m_game->states[ state ].creatures )
      {
      	SmartPointer<Creature> creature = new Creature();
        creature->x = p.second.x;
        creature->y = p.second.y;
        creature->owner = p.second.owner;
        creature->addKeyFrame( new DrawCreature( creature ) );
        turn.addAnimatable( creature );
        
        turn[p.second.id]["ID"] = p.second.id;
        turn[p.second.id]["Owner"] = p.second.owner;
        turn[p.second.id]["X"] = p.second.x;
        turn[p.second.id]["Y"] = p.second.y;
        turn[p.second.id]["Energy"] = -1;
        turn[p.second.id]["Energy Left"] = -1;
        turn[p.second.id]["Carn"] = -1;
        turn[p.second.id]["Herb"] = -1;
        turn[p.second.id]["Speed"] = -1;
        turn[p.second.id]["Defence"] = -1;
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

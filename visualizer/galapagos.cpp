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

  void Galapagos::preDraw()
  {
    const Input& input = gui->getInput();
    
    // Handle player input here
  }

  void Galapagos::postDraw()
  {
    if( renderer->fboSupport() )
    {
#if 0
      renderer->useShader( programs["post"] ); 
      renderer->swapFBO();
      renderer->useShader( 0 );
#endif

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
    // TODO Selection logic
    return list<int>();  // return the empty list
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
    header << "one" << "two" << "three";
    gui->setDebugHeader( header );
    timeManager->setNumTurns( 0 );

    animationEngine->registerGame(0, 0);

    // Look through each turn in the gamelog
    for(int state = 0; state < (int)m_game->states.size() && !m_suicide; state++)
    {
      Frame turn;  // The frame that will be drawn
      SmartPointer<Map> map = new Map();
      map->width = m_game->states[state].mapWidth;
      map->height = m_game->states[state].mapHeight;
      map->addKeyFrame( new DrawMap( map ) );
      turn.addAnimatable( map );
      
      for( auto& p : m_game->states[ state ].plants )
      {
        SmartPointer<Plant> plant = new Plant();
        plant->x = p.second.x;
        plant->y = p.second.y;
        plant->size = p.second.size;
        plant->addKeyFrame( new DrawPlant( plant ) );
        turn.addAnimatable( plant );
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

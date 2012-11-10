#include "animations.h"
#include "galapagos.h"
#include "easing_equations.h"
#include "glm/gtx/fast_square_root.hpp"
#include <sstream>
#include <string>
#include <math.h>

namespace visualizer
{

  static Color PlayerColor(int id, float trans = 1.0f)
  {
    return id == 0 ? Color( 0.8, 0.1, 0.1, trans ) : Color( 0.1, 0.1, 0.8, trans );
  }
   
  void StartAnim::animate( const float& /* t */, AnimData * /* d */, IGame* /*game*/ )
  {
  }

  void DrawIsland(float atX, float atY, float atWidth, float atHeight, int tile, IGame* game)
  {
    game->renderer->setColor(Color(1,1,1,1));
    for(int x = -Galapagos::IslandOffset() + atX; x < atX+atWidth+Galapagos::IslandOffset(); x++)
    {
      // draw the island sides
      if( x >= atX && x < atX+atWidth )
      {
        game->renderer->drawAnimQuad( x, -Galapagos::IslandOffset() + atY, 1, 1, "island_sides", 1);
        game->renderer->drawAnimQuad( x, atY+atHeight, 1, 1, "island_sides", 0);
      }
    }

    for(int y = -Galapagos::IslandOffset() + atY; y < atY+atHeight+Galapagos::IslandOffset(); y++)
    {
      // draw the island sides
      if( y >= atY && y < atY+atHeight )
      {
        game->renderer->drawAnimQuad( -Galapagos::IslandOffset() + atX, y, 1, 1, "island_sides", 2);
        game->renderer->drawAnimQuad( atX+atWidth, y, 1, 1, "island_sides", 3);
      }
    }

    // draw the island side corners
    game->renderer->drawAnimQuad( atX-1, atY-1, 1, 1, "island_sides", 6);
    game->renderer->drawAnimQuad( atX+atWidth, atY-1, 1, 1, "island_sides", 7);
    game->renderer->drawAnimQuad( atX-1, atY+atHeight, 1, 1, "island_sides", 4);
    game->renderer->drawAnimQuad( atX+atWidth, atY+atHeight, 1, 1, "island_sides", 5);

    for( int x = atX; x < atX+atWidth; x++ )
    {
      for( int y = atY; y < atY+atHeight; y++ )
      {
        game->renderer->drawAnimQuad( x, y, 1, 1, "tile_ground", tile );
      }
    }
  }

  void DrawMap::animate( const float& t, AnimData *, IGame* game )
  {

    game->renderer->setColor( Color( 1, 1, 1, 1 ) );
    // ugly
    string water_tiles[] = { "tile_water", "tile_lava", "tile_toxic", "tile_space" };
    float waterT = game->options->getNumber("Enable Water Movement") * t;
    // Draw the water!
    for(int x = -Galapagos::IslandOffset(); x < m_Map->GetWidth()+Galapagos::IslandOffset(); x++)
    {
      // top and bottom
      for(int y = 0; y < Galapagos::IslandOffset(); y++)
      {
        game->renderer->drawSubTexturedQuad( x, y - Galapagos::IslandOffset(), 1, 1, waterT*0.5f, waterT*0.5f, 0.5f, 0.5f, water_tiles[m_Map->waterTile] );
        game->renderer->drawSubTexturedQuad( x, y + m_Map->GetHeight(), 1, 1, waterT*0.5f, waterT*0.5f, 0.5f, 0.5f, water_tiles[m_Map->waterTile] );
      }

      // behind the HUD
      for(int y = 0; y < m_Map->GetHUDHeight(); y++)
      {
        game->renderer->drawSubTexturedQuad( x, y + m_Map->GetHeight() + Galapagos::IslandOffset(), 1, 1, waterT*0.5f, waterT*0.5f, 0.5f, 0.5f, water_tiles[m_Map->waterTile] );
      }
    }

    for(int y = -Galapagos::IslandOffset(); y < m_Map->GetHeight()+Galapagos::IslandOffset(); y++)
    {
      // left and right
      for(int x = 0; x < Galapagos::IslandOffset(); x++)
      {
        game->renderer->drawSubTexturedQuad( x - Galapagos::IslandOffset(), y, 1, 1, waterT*0.5f, waterT*0.5f, 0.5f, 0.5f, water_tiles[m_Map->waterTile] );
        game->renderer->drawSubTexturedQuad( x + m_Map->GetWidth(), y, 1, 1, waterT*0.5f, waterT*0.5f, 0.5f, 0.5f, water_tiles[m_Map->waterTile] );
      }
    }

    DrawIsland(0,0,m_Map->GetWidth(),m_Map->GetHeight(),m_Map->groundTile,game);

    // Draw the tiles creatures have moved over
    for (int x = 0; x < m_Map->GetWidth(); x++)
    {
      for (int y = 0; y < m_Map->GetHeight(); y++)
      {
        Map::Tile& tile = (*m_Map)(y,x);

        game->renderer->setColor(Color(1,1,0.5f,1.0f));

        if(tile.turn - game->timeManager->getTurn() > 3)
        {
          tile.turn = -1;
        }

        if( tile.turn > game->timeManager->getTurn() /*&& (tile.turn - game->timeManager->getTurn() <= 3)*/)
        {
          game->renderer->drawTexturedQuad( x, y, 1, 1, "sand");
        }

      }
    }

    // Draw the grid
    if( game->options->getNumber("Show Grid") > 0.0f )
    {
      game->renderer->setColor( Color( 0.25f, 0.25f, 0.25f, 0.5f ) );
      for(int x = 0; x < m_Map->GetWidth() + 1; x++)
      {
        game->renderer->drawLine(x, 0, x, m_Map->GetHeight(), 1);
      }
      for(int y = 0; y < m_Map->GetHeight() + 1; y++)
      {
        game->renderer->drawLine(0, y, m_Map->GetWidth(), y, 1);
      }
    }

  }
  
  void DrawPlant::animate( const float& t, AnimData*, IGame* game )
  {

    // should we draw the plant size as a number?
    bool drawSize = game->options->getNumber("Draw Plant Sizes") > 0.0f;
    
    game->renderer->setColor( Color( 1, 1, 1, 1 ) );
    
    // Draw the dirt
    game->renderer->drawAnimQuad( m_Plant->x, m_Plant->y, 1, 1, "plants", 15);
    
    // if the plant did not grow (the plants growth this turn was 0)
    if (m_Plant->growth == 0)
    {
      game->renderer->drawAnimQuad( m_Plant->x, m_Plant->y, 1, 1, "plants", m_Plant->size);
      if( drawSize )
      {
        game->renderer->drawText( m_Plant->x, m_Plant->y, "Roboto", toString(m_Plant->size), 3, IRenderer::Left);
      }
    }
    else
    // else the plant had some sort of size change so we need to fade in and out some plants
    {
      // Draw the previous plant fading out
      game->renderer->setColor( Color( 1, 1, 1, 1-t ) );
      game->renderer->drawAnimQuad( m_Plant->x, m_Plant->y, 1, 1, "plants", m_Plant->size - m_Plant->growth);
      if( drawSize )
      {
        game->renderer->drawText( m_Plant->x, m_Plant->y, "Roboto", toString(m_Plant->size - m_Plant->growth), 3, IRenderer::Left);
      }
      
      // Draw the current plant fading in
      game->renderer->setColor( Color( 1, 1, 1, t ) );
      game->renderer->drawAnimQuad( m_Plant->x, m_Plant->y, 1, 1, "plants", m_Plant->size);
      if( drawSize )
      {
        game->renderer->drawText( m_Plant->x, m_Plant->y, "Roboto", toString(m_Plant->size), 3, IRenderer::Left);
      }
    }
  }
  
  void DrawCreature::animate(const float& t, AnimData*, IGame* game )
  {
    unsigned int index = (unsigned int)(m_Creature->m_moves.size() * t);
    float subT = m_Creature->m_moves.size() * t - index;

    float posX = m_Creature->m_moves[index].from.x + (m_Creature->m_moves[index].to.x - m_Creature->m_moves[index].from.x) * subT;
    float posY = m_Creature->m_moves[index].from.y + (m_Creature->m_moves[index].to.y - m_Creature->m_moves[index].from.y) * subT;

    // todo: maybe the bigger the creature, the longer a path it leaves?
	//(*m_Creature->map)(posY,posX) = Map::Tile("sand",game->timeManager->getTurn() + 3);
    (*m_Creature->map)(floor(posY+0.5f),floor(posX+0.5f)) = Map::Tile(game->timeManager->getTurn() + 3);

    game->renderer->setColor( PlayerColor(m_Creature->owner) );

    // for some reason commenting out these other body parts works on windows...
    

    // todo: maybe change this

    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_body" , m_Creature->energy-1);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_etc" , 0);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_leg" , m_Creature->speed - 1);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_arm" , m_Creature->herbivorism - 1);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_armor" , m_Creature->defense - 1);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_head" , m_Creature->carnivorism - 1);

    // draw the health bar
    if( game->options->getNumber("Show Health Bars") > 0.0f)
    {
      game->renderer->setColor( Color(1, 1, 1, 1) );
      game->renderer->drawTexturedQuad( posX, posY, 1, 0.125f, "healthbar" );
      game->renderer->setColor( PlayerColor(m_Creature->owner) );
      game->renderer->drawTexturedQuad( posX, posY, m_Creature->maxHealth != 0 ? float(m_Creature->health) / float(m_Creature->maxHealth) : 0, 0.125f, "healthbar" );
    }

    // if they has sex
    if( !m_Creature->canBreed )
    {
      game->renderer->setColor( Color(1, 1, 1, 1) );
      // heart size
      float h = 0.25f;

      game->renderer->drawTexturedQuad( posX-h/2.0f, posY-h/2.0f, h, h, "heart" );
      game->renderer->drawTexturedQuad( posX+1-h/2.0f, posY-h/2.0f, h, h, "heart" );
      game->renderer->drawTexturedQuad( posX-h/2.0f, posY+1-h/2.0f, h, h, "heart" );
      game->renderer->drawTexturedQuad( posX+1-h/2.0f, posY+1-h/2.0f, h, h, "heart" );
    }
  }


  void DrawAnimation::animate(const float& t, AnimData*, IGame* game )
  {

    if(m_animation->enable.empty() || game->options->getNumber(m_animation->enable) > 0.0f)
    {
        game->renderer->setColor( Color(1.0f,1.0f,1.0f,1.0f) );
        game->renderer->drawAnimQuad( m_animation->x, m_animation->y, m_animation->dx, m_animation->dy, m_animation->animation , (int)(m_animation->frames * t));
    }

  }
  
  
  void DrawEatAnimation::animate(const float& t, AnimData*, IGame* game)
  {
    float trans = t < 0.1f ? t * 10.0f : t > 0.9f ? 1-(t-0.9f)*10.0f : 1;  
    game->renderer->setColor( Color(1.0f,1.0f,1.0f,trans) );
    game->renderer->drawTexturedQuad( m_EatAnimation->x, m_EatAnimation->y - (1-t)*0.25f - 0.125f, 1, 1, "teeth");
    game->renderer->drawRotatedTexturedQuad( m_EatAnimation->x, m_EatAnimation->y - t*0.25f + 0.375f, 1, 1, 180, "teeth");
  } // DrawEatAnimation
  
  
  void DrawSplashScreen::animate(const float& t, AnimData*, IGame* game )
  {
    // clamp t to 0.8
    float trans = t > 0.8f ? 0.8f : t;

    // Draw the backgroudn fading to white
    game->renderer->setColor( Color(0.8f, 0.8f, 0.8f, trans) );
    game->renderer->drawQuad(0, 0, m_SplashScreen->width, m_SplashScreen->height);
    
    // Draw the "Winner: " text in black and why they won
    game->renderer->setColor( Color(0.0f, 0.0f, 0.0f, trans) );
    game->renderer->drawText( m_SplashScreen->width / 2, m_SplashScreen->height / 2 - 4.5f, "Roboto", "Winner: ", 4.5f, IRenderer::Center);
    game->renderer->drawText( m_SplashScreen->width / 2, m_SplashScreen->height / 2 + 1.33f, "Roboto", m_SplashScreen->reason, 3.5f, IRenderer::Center);

    // Draw the actual winner's name in their color
    game->renderer->setColor( PlayerColor( m_SplashScreen->winnerID, trans ) );
    game->renderer->drawText( m_SplashScreen->width / 2, m_SplashScreen->height / 2 - 2, "Roboto", m_SplashScreen->winner, 7.25f, IRenderer::Center);
  }

  void DrawHUD::animate(const float& t, AnimData*, IGame* game )
  {
    game->renderer->setColor( Color(1.0f, 1.0f, 1.0f, 1.0f) );

    auto alignment = m_HUD->playerID == 0 ? IRenderer::Left : IRenderer::Right;

    string timeString = "Time: ";
    timeString += toString(99999999);
    cout << "timeString: " << timeString << endl;
    string idString = "ID: ";
    idString += toString(m_HUD->playerID);
    int width = std::max((int)game->renderer->textWidth("Roboto",m_HUD->playerName,3.0f),(int)game->renderer->textWidth("Roboto",timeString,3.0f)) + 1;
    int islandPos = m_HUD->playerID * (m_HUD->mapWidth - width);
    timeString = "Time: ";
    timeString += toString(m_HUD->time);

    // draw the island behind the player's HUD
    DrawIsland(islandPos, m_HUD->mapHeight + 1, width, 3, m_HUD->tile, game);

    int textPos = m_HUD->playerID * m_HUD->mapWidth;

    game->renderer->setColor( PlayerColor( m_HUD->playerID) );
    game->renderer->drawText( textPos, m_HUD->mapHeight + 1, "Roboto", m_HUD->playerName, 3.0f, alignment);
    game->renderer->drawText( textPos, m_HUD->mapHeight + 2, "Roboto", idString, 3.0f, alignment);
    game->renderer->drawText( textPos, m_HUD->mapHeight + 3, "Roboto", timeString, 3.0f, alignment);

  }


  void DrawNest::animate(const float& t, AnimData*, IGame* game )
  {
    game->renderer->setColor( Color(1.0f, 1.0f, 1.0f, 1.0f) );
    game->renderer->drawTexturedQuad(m_Nest->x, m_Nest->y, 1, 1, "nest");
  }
}

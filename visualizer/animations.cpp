#include "animations.h"
#include "galapagos.h"
#include "easing_equations.h"
#include "glm/gtx/fast_square_root.hpp"
#include <sstream>
#include <string>

namespace visualizer
{

  static Color PlayerColor(int id, float trans)
  {
    return id == 0 ? Color( 0.8, 0.1, 0.1, trans ) : Color( 0.1, 0.1, 0.8, trans );
  }

  static Color PlayerColor(int id)
  {
    return PlayerColor(id, 1);
  }
   
  void StartAnim::animate( const float& /* t */, AnimData * /* d */, IGame* /*game*/ )
  {
  }

  void DrawMap::animate( const float& t, AnimData *, IGame* game )
  {
    int middleY = m_Map->GetHeight() / 2;
    
    bool bLighting = game->options->getNumber("Enable Lighting") > 0.0f;
    bool bSunEffect = bLighting && (game->options->getNumber("Enable Sun") > 0.0f);
    float color = 1.0f;
    
    if(bLighting)
    {
      color = linearTween(t,m_Map->GetPrevMapColor(),m_Map->GetMapColor() - m_Map->GetPrevMapColor(),1.0);
    }
    
  
    for (int x = 0; x < m_Map->GetWidth(); x++)
    {
      for (int y = 0; y < m_Map->GetHeight(); y++)
      {
        const Map::Tile& tile = (*m_Map)(y,x);
        float r = 0.8f;
        float g = 0.8f;
        
        if(bLighting)
        {
          float d = 1.0f;
          
          if(bSunEffect)
          {
            d = glm::fastInverseSqrt((float)((m_Map->GetxPos() - x)*(m_Map->GetxPos() - x)+(middleY-y)*(middleY-y)))*10.0f;
          }
          
          r = color*d;
          g = color;
        } 
        
        game->renderer->setColor( Color(r, g, 0.2f,1.0f ) );

        // todo: this could be cleaned up a bit, but it seems to work right now
        if(tile.turn == game->timeManager->getTurn())
        {
            game->renderer->drawTexturedQuad( x, y, 1, 1, (tile.startTime < t) ? tile.texture : "grass" );
        }
        else if((tile.turn + 2) == game->timeManager->getTurn())
        {
            game->renderer->drawTexturedQuad( x, y, 1, 1, (tile.startTime > t) ? tile.texture : "grass" );
        }
        else
        {
            game->renderer->drawTexturedQuad( x, y, 1, 1, (tile.turn + 2 > game->timeManager->getTurn()) ? tile.texture : "grass" );
        }

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
    int frame_sp = m_Creature->speed - 1;
    int frame_en = (int)(10.0f*(m_Creature->energyLeft / (float)(m_Creature->maxEnergy + 1)));
    int frame_hb = m_Creature->herbivorism - 1;
    int frame_df = m_Creature->defense - 1;
    int frame_cv = m_Creature->carnivorism - 1;

    unsigned int index = (unsigned int)(m_Creature->m_moves.size() * t);
    float subT = m_Creature->m_moves.size() * t - index;

    float posX = m_Creature->m_moves[index].from.x + (m_Creature->m_moves[index].to.x - m_Creature->m_moves[index].from.x) * subT;
    float posY = m_Creature->m_moves[index].from.y + (m_Creature->m_moves[index].to.y - m_Creature->m_moves[index].from.y) * subT;

    game->renderer->setColor( PlayerColor(m_Creature->owner) );

    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_leg" , frame_sp);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_body" , frame_en);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_etc" , 0);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_arm" , frame_hb);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_armor" , frame_df);
    game->renderer->drawAnimQuad( posX, posY, 1, 1, "creature_head" , frame_cv);

  }


  void DrawAnimation::animate(const float& t, AnimData*, IGame* game )
  {
    game->renderer->setColor( Color(1.0f,1.0f,1.0f,1.0f) );
    game->renderer->drawAnimQuad( m_animation->x, m_animation->y, 1, 1, "death" , (int)(m_animation->frame * t));
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
    float trans = t < 0.2f ? t * 5.0f : 1;

    // Draw the backgroudn fading to white
    game->renderer->setColor( Color(1.0f, 1.0f, 1.0f, trans) );
    game->renderer->drawQuad(0, 0, m_SplashScreen->width, m_SplashScreen->height);
    
    // Draw the "Winner: " text in black and why they won
    game->renderer->setColor( Color(0.0f, 0.0f, 0.0f, trans) );
    game->renderer->drawText( m_SplashScreen->width / 2, m_SplashScreen->height / 2 - 4.5f, "Roboto", "Winner: ", 4.5f, IRenderer::Center);
    game->renderer->drawText( m_SplashScreen->width / 2, m_SplashScreen->height / 2 + 1.33f, "Roboto", m_SplashScreen->reason, 3.5f, IRenderer::Center);

    // Draw the actual winner's name in their color
    game->renderer->setColor( PlayerColor( m_SplashScreen->winnerID, trans ) );
    game->renderer->drawText( m_SplashScreen->width / 2, m_SplashScreen->height / 2 - 2, "Roboto", m_SplashScreen->winner, 7.25f, IRenderer::Center);
  }
}

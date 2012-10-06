#include "animations.h"
#include "galapagos.h"
#include <sstream>
#include <string>

namespace visualizer
{
  // todo: move this to the core?
  template< class T >
  static string toString(const T& in)
  {
    stringstream ss;
    ss << in;
    return ss.str();
  }
   
  void StartAnim::animate( const float& /* t */, AnimData * /* d */, IGame* /*game*/ )
  {
  }

  void DrawMap::animate( const float& t, AnimData * /*d*/, IGame* game )
  {
    // Set the color to red
    game->renderer->setColor( Color( 0, 1, 0, 1 ) );
    
    // Draw a 2x2 rectangle at (1,1), with the top left corner of the screen being the origin 
    for (int x = 0; x < m_Map->width; x++)
    {
      for (int y = 0; y < m_Map->height; y++)
      {
        game->renderer->setColor( Color( (float)x/m_Map->width, (float)y/m_Map->height, 0, 1 ) );
        game->renderer->drawTexturedQuad( x, y, 1, 1, "grass" );
      }
    }
    
  }
  
  void DrawPlant::animate( const float& t, AnimData* /*d*/, IGame* game )
  {
    game->renderer->setColor( Color( 1, 1, 1, 1 ) );
    game->renderer->drawTexturedQuad( m_Plant->x, m_Plant->y, 1, 1, "leaf" );
    game->renderer->drawText( m_Plant->x, m_Plant->y, "Roboto", toString(m_Plant->size), 3, IRenderer::Left);
  }
  
  void DrawCreature::animate(const float& t, AnimData* d, IGame* game )
  {
  	game->renderer->setColor( Color( 1, 1, 1, 1 ) );
  	
  	// todo: change texture
    game->renderer->drawTexturedQuad( m_Creature->x, m_Creature->y, 1, 1, "leaf" );
    
    //game->renderer->drawText( m_Plant->x, m_Plant->y, "Roboto", toString(m_Plant->size), 3, IRenderer::Left);
  }

}

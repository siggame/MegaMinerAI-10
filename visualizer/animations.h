#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "galapagosAnimatable.h"

namespace visualizer
{

    struct StartAnim: public Anim
    {
      public:
        void animate( const float& t, AnimData *d, IGame* game );

    };
  
    class DrawMap: public Anim
    {
        public:
            DrawMap( Map* map ) { m_Map = map; }
            void animate( const float& t, AnimData* d, IGame* game );

            float controlDuration() const
            { return 0; }

            float totalDuration() const
            { return 0; }
        
        private:
            Map *m_Map;

    }; // DrawBackground
    
     class DrawPlant: public Anim
    {
        public:
            DrawPlant( Plant* plant ) { m_Plant = plant; }
            void animate( const float& t, AnimData* d, IGame* game );

            float controlDuration() const
            { return 0; }

            float totalDuration() const
            { return 0; }
        
        private:
            Plant *m_Plant;

    }; // DrawBackground
}

#endif // ANIMATION_H

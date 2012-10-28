#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "galapagosAnimatable.h"

namespace visualizer
{
    template< class T >
    static string toString(const T& in)
    {
    stringstream ss;
    ss << in;
    return ss.str();
    }

    struct StartAnim: public Anim
    {
      public:
        void animate( const float& t, AnimData *d, IGame* game );

    };
  
    class DrawMap: public Anim
    {
        public:
            DrawMap( Map* map ) : m_Map(map) {}
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
		    DrawPlant( Plant* plant ) : m_Plant(plant) {}
		    void animate( const float& t, AnimData* d, IGame* game );

		    float controlDuration() const
		    { return 0; }

		    float totalDuration() const
		    { return 0; }
		
		private:
		    Plant *m_Plant;

	 }; // DrawPlant
	 
	 class DrawCreature: public Anim
     {
        public:
            DrawCreature( Creature* creature ) : m_Creature(creature) {}
            void animate( const float& t, AnimData* d, IGame* game );

            float controlDuration() const
            { return 0; }

            float totalDuration() const
            { return 0; }
        
        private:
            Creature *m_Creature;

     }; // DrawCreature

     class DrawAnimation : public Anim
     {
     public:
         DrawAnimation( SpriteAnimation* animation ) : m_animation(animation) {}
         void animate( const float& t, AnimData* d, IGame* game );

         float controlDuration() const
         { return 0; }

         float totalDuration() const
         { return 0; }

         // todo: need to implement
         bool IsDone() const { return false; }

     private:
         SpriteAnimation* m_animation;
     };

     class DrawSplashScreen : public Anim
     {
         public:
            DrawSplashScreen( SplashScreen* ss ) : m_SplashScreen(ss) {}
            void animate( const float& t, AnimData* d, IGame* game );

            float controlDuration() const
            { return 0; }

            float totalDuration() const
            { return 0; }
        
        private:
            SplashScreen *m_SplashScreen;
     };
     
     class DrawEatAnimation : public Anim
     {
         public:
            DrawEatAnimation( EatAnimation* e ) : m_EatAnimation(e) {}
            void animate( const float& t, AnimData* d, IGame* game );

            float controlDuration() const
            { return 0; }

            float totalDuration() const
            { return 0; }
        
        private:
            EatAnimation *m_EatAnimation;
     };
    
}

#endif // ANIMATION_H

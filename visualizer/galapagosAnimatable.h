#ifndef GALAPAGOS_ANIMATABLE_H
#define GALAPAGOS_ANIMATABLE_H

#include "irenderer.h"
#include "parser/structures.h"

#include "math.h"

#include <glm/glm.hpp>
#include <string>

namespace visualizer
{
    class Map : public Animatable
    {
    public:

        struct Tile
        {
            explicit Tile(const std::string& tex = "grass",int t = 1000,float st = 0.0f) :
                texture(tex), turn(t), startTime(st) {}

          std::string texture;
          float startTime;
          int turn;           
          // todo: add more
        };
        
        Map(int w, int h, float pc, float mc, float xp) : m_tiles(w*h), width(w), height(h), prevMapColor(pc), mapColor(mc), xPos(xp) 
        {
        }
        
        Map(const Map& other, float mapColor, float xPos) : m_tiles(other.m_tiles), width(other.width),
        height(other.height), prevMapColor(other.mapColor), mapColor(mapColor), xPos(xPos)
        {
          
        }
     
        
        Tile& operator()(unsigned int r, unsigned int c)
        {
          return m_tiles[c + r*width];
        }
        
        const Tile& operator()(unsigned int r, unsigned int c) const
        {
          return m_tiles[c + r*width];
        }
        
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        float GetPrevMapColor() const { return prevMapColor; }
        float GetxPos() const { return xPos; }
        float GetMapColor() const { return mapColor; }

    private:
    
        std::vector<Tile> m_tiles;
        int width;
        int height;
        
        // lighting
        float prevMapColor;
        float mapColor;
        float xPos;
        
        // todo: add more?
    };
    
    struct Plant : public Animatable
    {
       int x;
       int y;
       int size;
       int growth;
       // todo: add more
    };
    
    struct Creature : public Animatable
    {
        struct Moves
        {
            Moves() {}
            Moves(const glm::vec2& t, const glm::vec2& f) : to(t), from(f)
            {
            }

            // todo: maybe rename these
            glm::vec2 to;
            glm::vec2 from;
        };

    	int owner;

        int energyLeft;
        int maxEnergy;
        int carnivorism;
        int herbivorism;
        int speed;
        int defense;

        //useful for visualizer?
        int movementLeft;
        int canEat;
        int canBreed;
        int parentID;

        std::vector<Moves> m_moves;
    	// todo: add more
    };

    struct SpriteAnimation : public Animatable
    {
        int x;
        int y;
        int frame;
    };

    struct SplashScreen : public Animatable
    {
        string winner;
        string message;
        int width;
        int height;
    };
    
    struct EatAnimation : public Animatable
    {
      explicit EatAnimation(int X, int Y) : x(X), y(Y) {}
      
      int x;
      int y;
    };

} // visualizer

#endif // GALAPAGOS_ANIMATABLE_H

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
            texture(tex), startTime(st), turn(t)  {}

          std::string texture;
          float startTime;
          int turn;           
          // todo: add more
        };
        
        Map(int w, int h, int hud, float pc, float mc, float xp) : m_tiles(w*h), width(w), height(h), hudHeight(hud), prevMapColor(pc), mapColor(mc), xPos(xp) 
        {
        }
        
        Map(const Map& other, float mapColor, float xPos) : m_tiles(other.m_tiles), width(other.width),
        height(other.height), hudHeight(other.hudHeight), prevMapColor(other.mapColor), mapColor(mapColor), xPos(xPos)
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
        int GetHUDHeight() const { return hudHeight; }

    private:
    
      std::vector<Tile> m_tiles;
      int width;
      int height;
      int hudHeight;
      
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

      int energy;
      int health;
      int maxHealth;
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
      SpriteAnimation(int posX, int posY, int f) : x(posX), y(posY), frame(f) {}

      int x;
      int y;
      int frame;
    };

    struct SplashScreen : public Animatable
    {
      SplashScreen(string win, string msg, int id, int w, int h) : winner(win), reason(msg), winnerID(id), width(w), height(h) {}

      string winner;
      string reason;
      int winnerID;
      int width;
      int height;
    };
    
    struct EatAnimation : public Animatable
    {
      EatAnimation(int X, int Y) : x(X), y(Y) {}
      
      int x;
      int y;
    };

    struct HUD : public Animatable
    {
      HUD(int mw, int mh, int h, string name, int id, float t) :
          time(t), playerName(name), playerID(id), mapWidth(mw), mapHeight(mh), height(h) {}
      
      float time;
      string playerName;
      int playerID;
      int mapWidth;
      int mapHeight;
      int height;
    };

    struct Nest
    {
      Nest(int mX, int mY, int dX, int dY) : momX(mX), momY(mY), dadX(dX), dadY(dY) {}

      int momX;
      int momY;
      int dadX;
      int dadY;
    };

    struct Lovin
    {
      Lovin(int X, int Y) : x(X), y(Y) {}

      int x;
      int y;
    };

} // visualizer

#endif // GALAPAGOS_ANIMATABLE_H

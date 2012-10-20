#ifndef GALAPAGOS_ANIMATABLE_H
#define GALAPAGOS_ANIMATABLE_H

//#include "galapagosAnimatable.h"
#include "irenderer.h"
#include "parser/structures.h"

#include "math.h"
#include <string>

namespace visualizer
{
    class Map : public Animatable
    {
    public:
    
        // todo: remove friendship
        friend class DrawMap;
    
        struct Tile
        {
          Tile(const std::string& tex = "grass",int t = 1000) : texture(tex), turn(t) {}

          std::string texture;
          int turn;
          
          // todo: add more
        };
        
        Map(int w, int h, float pc, float mc, float xp) : width(w), height(h), prevMapColor(pc), mapColor(mc), xPos(xp)
        {
          m_tiles.resize(w*h);
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
        
        int Width() const { return width; }
        int Height() const { return height; }
        
        // lighting
        void SetColor(float c) { mapColor = c; }
        void SetSunPos(float x) { xPos = x; }
        void SetPrevColor(float c) { prevMapColor = c; }
        
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
       // todo: add more
    };
    
    struct Creature : public Animatable
    {
    	int x;
    	int y;
    	int owner;
    	// todo: add more
    };

} // visualizer

#endif // GALAPAGOS_ANIMATABLE_H

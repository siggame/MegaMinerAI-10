#ifndef GALAPAGOS_ANIMATABLE_H
#define GALAPAGOS_ANIMATABLE_H

//#include "galapagosAnimatable.h"
#include "irenderer.h"
#include "parser/structures.h"

#include "math.h"

namespace visualizer
{
    struct Map: public Animatable
    {
        int width;
        int height;
        float prevColor;
        float color;
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
    	// todo: add more
    };

} // visualizer

#endif // GALAPAGOS_ANIMATABLE_H

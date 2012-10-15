#ifndef GALAPAGOS_H
#define GALAPAGOS_H

#include <QObject>
#include <QThread>
#include "igame.h"
#include "animsequence.h"
#include <map>
#include <string>
#include <list>

// The Codegen's Parser
#include "parser/parser.h"
#include "parser/structures.h"

using namespace std;

namespace visualizer
{
    struct Rect
    {
        int left;
        int top;
        int right;
        int bottom;
    };

    class Galapagos: public QThread, public AnimSequence, public IGame
    {
        Q_OBJECT;
        Q_INTERFACES( visualizer::IGame );
        public: 
            Galapagos();
            ~Galapagos();

            PluginInfo getPluginInfo();
            void loadGamelog( std::string gamelog );

            void run();
            void setup();
            void destroy();

            void preDraw();
            void postDraw();

            void addCurrentBoard();
    
            map<string, int> programs;
            
            list<int> getSelectedUnits();
        private:
            parser::Game *m_game;  // The Game Object from parser/structures.h that is generated by the Codegen
            bool m_suicide;
            list<int> m_selectedUnitIDs;
            
            void GetSelectedRect(Rect& out) const;
    }; 

} // visualizer

#endif // GALAPAGOS_H

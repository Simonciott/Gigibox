#include <SFML/Graphics.hpp>

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <functional>
//#include <stack>

#include "GigiConstants.hpp"
#include "GigiBuffers.hpp"
#include "GigiTexture.hpp"
#include "GigiSprite.hpp"

#include "GigiAssembly.hpp"

using std::map;
using std::string;
using std::vector;

using std::cout;
using std::endl;
using std::string;
using std::fstream;
using std::ios;
using std::function;
using std::stoi;
//using std::stack;

using sf::Image;
using sf::Sprite;
using sf::Color;
using sf::Texture;
using sf::Vector2f;
using sf::Vector2u;

int main(int argc, const char* argv[])
{
    const char* programscript = argc < 2 ? "./dvd.gasm" : argv[1];

    // sprite per metterci sopra il buffer sotto forma di texture
    Sprite Gigi_BackBufferSprite(Gigi_BackBufferTexture::texture);

    sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE }), "GigiBox");

    window.setView(sf::View(sf::FloatRect(Vector2f(), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT))));
    window.setFramerateLimit(FPS);

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            Gigi_BackBufferTexture::getBackBuffer()->setPixel({ (unsigned int)j, (unsigned int)i }, Color(j, (i + j) / 2, i));
        }
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            Gigi_BackBufferTexture::getBuffer()->setPixel({ (unsigned int)j, (unsigned int)i }, Color((i + j) / 2, i, j));
            //Gigi_BackBufferTexture::getBackBuffer()->setPixel({ (unsigned int)j, (unsigned int)i }, Color(j, (i + j) / 2, i));
        }
    }

    /* RIP dato immagine manualmente scritto
    uint8_t img_data[] = {0x08, 0x08, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0x01, 0x02,
            0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02,
            0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x02,
            0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
            0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
            0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
            0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00,
            0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00,
            0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x02,
            0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02
    };*/

    Gigi_AssemblyRegisters::AddRegistersToData();

    // scoped code block
    if (1) {
        string line;
        fstream programstream;
        programstream.open(string(programscript));

        while (std::getline(programstream, line)) {
            int com = line.find(";");
            if (com != string::npos) line.erase(com, line.size());

            Gigi_AssemblyInterpreter::programInstructions.push_back(line);
        }
        programstream.close();
    }
    if(Gigi_AssemblyInterpreter::programInstructions.empty())
        cout << "GIGIBOX WARNING:\n\tIN:\tmain()" << "\n\tDESCRIZIONE AVVERTIMENTO (potrebbe non essere accurato):\tNon sono state caricate alcune istruzioni al programma, causato probabilmente da script mancanti o inesistenti\n\tPROCEDURE:\tNon verra' eseguito alcun programma. Riavviare Gigibox se si desidera eseguirne uno\n\n";
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        Gigi_AssemblyRegisters::interrupted = !Gigi_AssemblyInterpreter::running;

        while (!Gigi_AssemblyRegisters::interrupted) {
            //cout << Gigi_AssemblyInterpreter::programInstructions[Gigi_AssemblyRegisters::programCounter] << endl;
            Gigi_AssemblyInterpreter::StepProgram();

            //cout << Gigi_AssemblyRegisters::programCounter << endl << endl;;

            /*for (int i = 0; i < Gigi_AssemblyRegisters::data.size(); i++) {
                cout << *Gigi_AssemblyRegisters::getData(i - REGISTERS_IN_MEMORY) << endl;
            }*/
            //cout << "\n\nfine stampa\n\n";
        }

        Gigi_BackBufferTexture::clearBackBuffer(Color::Black);

        for (int i = 0; i < Gigi_AssemblyRegisters::storedSprites.size(); i++) {
            Gigi_Sprite& spr = Gigi_AssemblyRegisters::storedSprites[i];
            spr.Draw(spr.x, spr.y);
        }

        Gigi_BackBufferTexture::swapBuffers();

        window.draw(Gigi_BackBufferSprite);
        window.display();
    }
}
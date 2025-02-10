#include <SFML/Graphics.hpp>

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <functional>
//#include <stack>

#include "Constants.hpp"
#include "Buffers.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"

#include "GigiAssembly.hpp"
#include "Input.hpp"
#include "Arguments.hpp"

using Gigi::Sprite;
using Gigi::BuffersManager;
using namespace Gigi::Assembly;
using namespace Gigi::Input;
using Gigi::Arguments;

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
using sf::Color;
using sf::Texture;
using sf::Vector2f;
using sf::Vector2u;

int main(int argc, const char* argv[])
{
    for (int i = 1; i < argc; i += 2) {
        Arguments::processArgument(argv, i);
    }

    // sprite per metterci sopra il buffer sotto forma di texture
    sf::Sprite Gigi_BackBufferSprite(BuffersManager::texture);

    sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE }), "GigiBox");

    window.setView(sf::View(sf::FloatRect(Vector2f(), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT))));
    window.setFramerateLimit(FPS);

    /*
    
    codice per generare una schermata con la matematica

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            BuffersManager::getBackBuffer()->setPixel({ (unsigned int)j, (unsigned int)i }, Color(j, (i + j) / 2, i));
        }
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            BuffersManager::getBuffer()->setPixel({ (unsigned int)j, (unsigned int)i }, Color((i + j) / 2, i, j));
            //Gigi_BackBufferTexture::getBackBuffer()->setPixel({ (unsigned int)j, (unsigned int)i }, Color(j, (i + j) / 2, i));
        }
    }*/

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

    Registers::AddRegistersToData();

    // scoped code block
    if (1) {
        string line;
        fstream programstream;
        cout << Arguments::program << "\n";
        programstream.open(Arguments::program);
        if (!programstream.is_open()) throw 0b11010000;

        while (std::getline(programstream, line)) {
            int com = line.find(";");
            if (com != string::npos) line.erase(com, line.size());

            Interpreter::programInstructions.push_back(line);
        }
        programstream.close();
    }
    if(Interpreter::programInstructions.empty())
        cout << "GIGIBOX WARNING:\n\tIN:\tmain()" << "\n\tDESCRIZIONE AVVERTIMENTO (potrebbe non essere accurato):\tNon sono state caricate alcune istruzioni al programma, causato probabilmente da script mancanti o inesistenti\n\tPROCEDURE:\tNon verra' eseguito alcun programma. Riavviare Gigibox se si desidera eseguirne uno\n\n";
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            // NON TESTATO. PER FAVORE TESTARE
            *Registers::getData(-4) = (
                ((int)Key::Up.isEitherDown() << 7) +
                ((int)Key::Left.isEitherDown() << 6) +
                ((int)Key::Down.isEitherDown() << 5) +
                ((int)Key::Right.isEitherDown() << 4) +
                ((int)Key::Zed.isEitherDown() << 3) +
                ((int)Key::Ex.isEitherDown() << 2) +
                ((int)Key::Cee.isEitherDown() << 1)
            );
        }

        Registers::interrupted = !Interpreter::running;

        while (!Registers::interrupted) {
            //cout << Gigi_AssemblyInterpreter::programInstructions[Gigi_AssemblyRegisters::programCounter] << endl;
            Interpreter::StepProgram();

            //cout << Gigi_AssemblyRegisters::programCounter << endl << endl;;

            /*for (int i = 0; i < Gigi_AssemblyRegisters::data.size(); i++) {
                cout << *Gigi_AssemblyRegisters::getData(i - REGISTERS_IN_MEMORY) << endl;
            }*/
            //cout << "\n\nfine stampa\n\n";
        }

        BuffersManager::clearBackBuffer(Color::Black);

        for (int i = 0; i < Registers::storedSprites.size(); i++) {
            Sprite& spr = Registers::storedSprites[i];
            spr.Draw(spr.x, spr.y);
        }

        BuffersManager::swapBuffers();

        window.draw(Gigi_BackBufferSprite);
        window.display();
    }
}
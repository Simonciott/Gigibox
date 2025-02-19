#include <SFML/Graphics.hpp>

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <functional>
//#include <stack>

#include "System/CommonFunctions.hpp"

#include "Game/Constants.hpp"
#include "Game/Buffers.hpp"
#include "Game/Texture.hpp"
#include "Game/Sprite.hpp"

#include "Assembly/Assembly.hpp"
#include "Game/Input.hpp"

#include "System/Arguments.hpp"
#include "System/Logger.hpp"

using namespace Gigi;
using namespace Gigi::Assembly;

using sf::Image;
using sf::Color;
using sf::Texture;
using sf::Vector2f, sf::Vector2u;

int main(int argc, const char* argv[])
{
    for (int i = 1; i < argc; i += 2) {
        Arguments::processArgument(argv, i);
    }

    // sprite per metterci sopra il buffer sotto forma di texture
    sf::Sprite Gigi_BackBufferSprite(BuffersManager::texture);

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

    try {
        string line;
        fstream programstream;
        programstream.open(Arguments::program);
        if (!programstream.is_open()) throw CODE_ERROR_FILE;

        while (std::getline(programstream, line)) {
            int com = line.find(";");
            if (com != string::npos) line.erase(com, line.size());

            Interpreter::programInstructions.push_back(line);
        }
        programstream.close();
    }
    catch (int er) {
        Logger::logError("main()", "logger.error.fileopen", "logger.warning.assembly.noprogramrun", er);
    }
    if (Interpreter::programInstructions.empty())
        Logger::logWarning("main()", "logger.warning.assembly.missingscript", "logger.warning.assembly.noprogramrun");

    // crea finestra
    sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE }), "GigiBox");

    window.setView(sf::View(sf::FloatRect(Vector2f(), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT))));
    window.setFramerateLimit(FPS);
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            /*
                l'input è tenuto in un numero con i primi bit significanti se l'input a cui è assegnato sta venendo premuto
                bit 1: su o w
                bit 2: sinistra o a
                bit 3: giù o s
                bit 4: destra o d
                bit 5: z oppure i
                bit 6: x oppure o
                bit 7: c oppure p
            */
            *Registers::getData(-4) = (
                ((int)Input::Key::Up.isEitherDown()) +
                ((int)Input::Key::Left.isEitherDown() << 1) +
                ((int)Input::Key::Down.isEitherDown() << 2) +
                ((int)Input::Key::Right.isEitherDown() << 3) +
                ((int)Input::Key::Zed.isEitherDown() << 4) +
                ((int)Input::Key::Ex.isEitherDown() << 5) +
                ((int)Input::Key::Cee.isEitherDown() << 6)
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
            spr.Draw();
        }

        BuffersManager::swapBuffers();

        cout << *Registers::getData(-4) << NEWL;

        window.draw(Gigi_BackBufferSprite);
        window.display();
    }
}
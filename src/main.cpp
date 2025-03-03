#include <string>
#include <iostream>
#include <cstring>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "System/System.hpp"

#include "Game/Constants.hpp"
#include "Game/Texture.hpp"
#include "Game/Sprite.hpp"
#include "Game/Input.hpp"

#include "Assembly/Assembly.hpp"

using namespace Gigi;
using namespace Gigi::Assembly;

using sf::Color;
using sf::Texture;
using sf::Vector2f, sf::Vector2u;

int main(int argc, const char* argv[])
{

    for (int i = 1; i < argc; i += 2) {
        Arguments::processArgument(argv, i);
    }
    Arguments::directory = getPathDirectoryPath(Arguments::program);

    // sprite per metterci sopra il buffer sotto forma di texture
    sf::Sprite Gigi_BackBufferSprite(BuffersManager::texture);

    Registers::AddRegistersToData();

    try {
        string line;
        fstream programstream;
        programstream.open(Arguments::program);
        if (!programstream.is_open()) throw CODE_ERROR_FILE;

        while (std::getline(programstream, line)) {
            while (line[0] == SPACE || line[0] == TAB) {
                line.erase(0, 1);
            }

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
            Interpreter::StepProgram();
        }

        BuffersManager::clearBackBuffer(Color::Black);

        for (int i = 0; i < Registers::storedSprites.size(); i++) {
            Assembly::Sprite& spr = Registers::storedSprites[i];
            spr.Draw(Registers::storedImages);
        }

        BuffersManager::swapBuffers();

        cout << *Registers::getData(-4) << NEWL;

        window.draw(Gigi_BackBufferSprite);
        window.display();
    }
}
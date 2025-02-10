#include "Input.hpp"

using namespace Gigi::Input;

Key::Key(Scan mk, Scan ak) {
	mainKey = mk;
	alternativeKey = ak;
}

bool Key::isEitherDown() {
	return sf::Keyboard::isKeyPressed(mainKey) || sf::Keyboard::isKeyPressed(alternativeKey);
}
Key Key::Up(Scan::W, Scan::Up);
Key Key::Left(Scan::A, Scan::Left);
Key Key::Down(Scan::S, Scan::Down);
Key Key::Right(Scan::D, Scan::Right);

Key Key::Zed(Scan::I, Scan::Z);
Key Key::Ex(Scan::O, Scan::X);
Key Key::Cee(Scan::P, Scan::C);
#include "Input.hpp"

using namespace Gigi::Input;

Key::Key(Scan mk, Scan ak) {
	mainKey = mk;
	alternativeKey = ak;
}

bool Key::isEitherDown() {
	return sf::Keyboard::isKeyPressed(mainKey) || sf::Keyboard::isKeyPressed(alternativeKey);
}
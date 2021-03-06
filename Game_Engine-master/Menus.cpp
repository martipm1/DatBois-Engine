#include "Menus.h"

// ------------------------------------------------------------
Menu::Menu(MenuTypes _type) : type(_type)
{}

// ------------------------------------------------------------
Menu::~Menu()
{}

// ------------------------------------------------------------
void Menu::SwitchActive()
{
	active = !active;
}

// ------------------------------------------------------------
bool Menu::GetActive() const
{
	return active;
}

// ------------------------------------------------------------
void Menu::Render()
{}

// ------------------------------------------------------------
MenuTypes Menu::GetType() const
{
	return type;
}

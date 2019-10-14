#pragma once
#include "Attack.h"
//#include <SFML/Graphics.hpp>

class Enemy
{
public:
	std::string m_name;
	std::vector<Attack> m_attacks;
	unsigned int m_currrentAttack;
	//sf::Texture m_texture;
};


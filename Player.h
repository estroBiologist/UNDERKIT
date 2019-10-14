#pragma once
#include <SFML/Graphics.hpp>

//Easiest fucking class in the world
class Player: public sf::Sprite, public sf::Texture
{
public:
	std::string		m_name = "Chara";
	unsigned int	m_health = 20;
	unsigned int	m_maxHealth = 20;
	unsigned int	m_atk = 1;
	unsigned int	m_def = 1;
	unsigned int	m_lv = 1;
};


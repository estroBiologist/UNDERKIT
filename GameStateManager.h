#pragma once
#include "GameState.h"
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>


class NonCopyable {
public:
	
	NonCopyable()	= default;
	~NonCopyable()	= default;

	//Move constructors
	NonCopyable				(NonCopyable&& other)	= default;
	NonCopyable& operator=	(NonCopyable&& other)	= default;

	//Copy constructors
	NonCopyable		(const NonCopyable& other)	= delete;
	void operator=	(const NonCopyable& other)	= delete;
};




class GameStateManager: public NonCopyable
{
private:
	std::unique_ptr<GameState> currentLoadedState;
public:
	void changeState(std::unique_ptr<GameState> state);

	void update(sf::Time delta) const { if (currentLoadedState) currentLoadedState->update(delta);}
	void draw(sf::RenderWindow& window) const { if (currentLoadedState) currentLoadedState->draw(window); }
	void event(sf::Event event, sf::RenderWindow& window) { if (currentLoadedState) currentLoadedState->event(event, window); }
};


#pragma once
namespace sf {
	class Time;
	class RenderWindow;
	class Event;
}

class GameStateManager;

class GameState
{
protected:
	GameStateManager* m_stateManager;

public:
	virtual ~GameState() = default;

	struct GameSettings {
		float maxFramerate = -1.f;
		
	};
	virtual void start() = 0;
	virtual void update(sf::Time delta) = 0;
	virtual void draw(sf::RenderWindow &window) = 0;
	virtual void event(sf::Event event, sf::RenderWindow& window) = 0;
	
	GameState(GameStateManager* manager) : m_stateManager(manager) {}
};

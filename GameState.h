#pragma once
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
	virtual void update() = 0;

	GameState(GameStateManager* manager) : m_stateManager(manager) {}
};

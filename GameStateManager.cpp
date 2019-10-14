#include "GameStateManager.h"

void GameStateManager::changeState(std::unique_ptr<GameState> state)
{
	currentLoadedState = std::move(state);
	currentLoadedState->start();
}

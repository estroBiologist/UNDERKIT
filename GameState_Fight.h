#pragma once
#include "GameState.h"

class GameStateFight final : public GameState
{
public:
	void start() override;
	void update() override;
};


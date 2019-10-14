#pragma once
#include "GameState.h"
#include <memory>


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
public:
	std::unique_ptr<GameState> currentLoadedState;
	void changeState(std::unique_ptr<GameState> state);
};


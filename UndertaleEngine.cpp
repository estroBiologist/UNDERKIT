// UndertaleEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "UTText.h"
#include "GlobalResources.h"
#include "GameStateManager.h"
#include "GameStateFight.h"


#ifdef UT_EDITOR
#include <imgui/imgui.h>
#include <imgui/imgui-sfml-master/imgui-SFML.h>
#endif

void update(sf::Time delta);
void draw(sf::RenderWindow& window);

unsigned int debug_FPS;


//Declared in GlobalResources.h
sf::Font dtm_mono;
sf::Font dtm_sans;
sf::Font eightbit_wonder;
sf::Font mars_need_cunnilingus;

sf::Clock globalTimer;

bool editing = false;

GameStateManager manager;

int main() {
	
	dtm_sans.loadFromFile("assets/menu/fonts/DTM-Sans.otf");
	dtm_mono.loadFromFile("assets/menu/fonts/DTM-Mono.otf");
	eightbit_wonder.loadFromFile("assets/menu/fonts/8-BIT WONDER.TTF");
	mars_need_cunnilingus.loadFromFile("assets/menu/fonts/Mars_Needs_Cunnilingus.ttf");
	
	std::srand(time(nullptr));
	
	using namespace sf;
	RenderWindow window(VideoMode(640,480), "UNDERKIT", Style::Close);
	std::cout << "UNDERKIT Battle Engine - by Chloe K. @trans_disaster\nPlease see LICENSE.txt for license information";
	
#ifdef UT_EDITOR
	ImGui::SFML::Init(window);
#endif




	manager.changeState(std::make_unique<GameStateFight>(GameStateFight(&manager)));
	
	Clock deltaClock;

	unsigned long frameCounter = 0;

	Clock frameCounterClock;
	
	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event)) {
			
			if (event.type == Event::Closed)
				window.close();
			
			if (event.type == Event::KeyPressed) {
				switch (event.key.code) {
				case Keyboard::E:
					editing = !editing;
					break;
				default:
					break;
				}
			}

			manager.event(event, window);
		}

		if (frameCounterClock.getElapsedTime().asSeconds() >= .25f) {
			debug_FPS = frameCounter / frameCounterClock.getElapsedTime().asSeconds();
			frameCounter = 0;
			frameCounterClock.restart();
		}
		
		Time deltaTime(deltaClock.restart());
		update(deltaTime);
		draw(window);
		frameCounter++;
	}
}





void update(sf::Time delta) {
	
	manager.update(delta);
}

void draw(sf::RenderWindow& window) {
#ifdef _DEBUG

	manager.draw(window);
	UTText debugText(dtm_sans, "FPS: " + std::to_string(debug_FPS));
	window.draw(debugText);
#endif
}

#include "GameStateFight.h"
#include "GlobalResources.h"
#include "UTText.h"

bool GameStateFight::isSelectState(BattleState state) {
	return
		state == BattleState::Menu_Fight_Select ||
		state == BattleState::Menu_Act_Select ||
		state == BattleState::Menu_Item_Select ||
		state == BattleState::Menu_Mercy_Select ||
		state == BattleState::Menu_Act_Options;
}

bool GameStateFight::isConfirmAction(sf::Keyboard::Key key) {
	return key == sf::Keyboard::Z || key == sf::Keyboard::Enter;
}

bool GameStateFight::isCancelAction(sf::Keyboard::Key key) {
	return key == sf::Keyboard::X || key == sf::Keyboard::LControl;
}

void GameStateFight::attack_end() {
	//Select a new random flavour text
	state = BattleState::Menu_Base;
}

void GameStateFight::attack_begin() {

	if (!current_attack) {
		//Find first enemy with non-empty attack vector
		for (auto& enemy : enemies) {
			if (enemy.m_attacks.empty()) continue;
			current_attack = &enemy.m_attacks[std::rand() % enemy.m_attacks.size()];
			break;
		}
		//If no attack loaded, just end the turn and go back to battle
		if (!current_attack) attack_end();
	}
}
#ifdef UT_EDITOR

void GameStateFight::imdraw() {
	ImGui::SFML::Render();	
}

#else

void GameStateFight::imdraw() {
}

#endif

GameStateFight::GameStateFight(GameStateManager* manager): GameState(manager) {
}

void GameStateFight::start() {
	player.loadFromFile("assets/player/heart.png");
	player.setTexture(player);

	btn_fight.setPosition(32, 432);
	btn_act.setPosition(185, 432);
	btn_item.setPosition(345, 432);
	btn_mercy.setPosition(500, 432);

	btns_texture.loadFromFile("assets/menu/buttons.png");

	btn_fight.setTexture(btns_texture);
	btn_act.setTexture(btns_texture);
	btn_item.setTexture(btns_texture);
	btn_mercy.setTexture(btns_texture);
}

void GameStateFight::update(sf::Time delta) {
	static double gameTime = 0.0;
	static constexpr double timeStep = 16.666666f;
	using namespace sf;

	double millis = static_cast<double>(delta.asMicroseconds()) / 1000.0;

	double deltaFactor = millis / timeStep;
	gameTime += millis;

	if (state == BattleState::Menu_Base) {
		bullet_box.setSize({564.f, 130.f}); //Dialogue size constant
		bullet_box.setOrigin({319.f, 320.f}); //Dialogue origin constant
		bullet_box.setBackgroundColor(Color::Black);
		bullet_box.setBorderColor(Color::White);
		switch (current_menu_choice) {
		case 0:
			player.setPosition(40, 446);
			break;
		case 1:
			player.setPosition(195, 446);
			break;
		case 2:
			player.setPosition(353, 446);
			break;
		case 3:
			player.setPosition(507, 446);
			break;
		default:
			break;
		}
	}
	if (isSelectState(state)) {
		//Set player position based on menu index
		Vector2f playerPos;
		
		if (menu_index.x % 2 == 0)
			playerPos.x = 64.f;
		else
			playerPos.x = 324.f; 

		playerPos.y = 278.f + 32.f * menu_index.y;
		
		player.setPosition(playerPos);
		
	}
	

	
	//Fixed update for bullet_box bc of lerp-based movement
	while (gameTime > timeStep) {
		bullet_box.update();
		gameTime -= timeStep;
	}
	BulletBox::attacking = true;

	if (state == BattleState::Attack_AttackPhase) {

		Vector2f playerVel;

		float spd = 2.0f * static_cast<float>(deltaFactor);
		
		if (Keyboard::isKeyPressed(Keyboard::Left))
			playerVel.x -= spd;
		if (Keyboard::isKeyPressed(Keyboard::Right))
			playerVel.x += spd;
		if (Keyboard::isKeyPressed(Keyboard::Up))
			playerVel.y -= spd;
		if (Keyboard::isKeyPressed(Keyboard::Down))
			playerVel.y += spd;

		player.move(playerVel);


		auto playerBounds = player.getGlobalBounds();
		bullet_box.resolveCollision(playerBounds);
		player.setPosition(playerBounds.left, playerBounds.top);

		if (current_attack) {
			attackTimer -= delta.asSeconds();
			if (attackTimer <= 0.f) attack_end();
		} else attack_end();
	}
}

void GameStateFight::event(sf::Event event, sf::RenderWindow& window) {
	using namespace sf;

	
	if (state == BattleState::Menu_Base) {
		if (event.key.code == Keyboard::Left)
			current_menu_choice--;
		if (event.key.code == Keyboard::Right)
			current_menu_choice++;
		if (current_menu_choice < 0) current_menu_choice = 3;
		if (current_menu_choice > 3) current_menu_choice = 0;

		if (isConfirmAction(event.key.code)) {
			menu_index = {0,0};
			switch (current_menu_choice) {
			case 0:
				state = BattleState::Menu_Fight_Select;
				return;
			case 1:
				state = BattleState::Menu_Act_Select;
				return;
			case 2:
				state = BattleState::Menu_Item_Select;
				return;
			case 3:
				state = BattleState::Menu_Mercy_Select;
				return;
			default:
				throw std::exception("Invalid menu option.");
			}
		}
	}

	//Selecting (fight, act, item)
	if (isSelectState(state)) {
		//Left-right selection is only possible in item selection & act options
		if (state == BattleState::Menu_Item_Select || state == BattleState::Menu_Act_Options) {
			if (event.key.code == Keyboard::Left && menu_index.x > 0)
				menu_index.x--;
			if (event.key.code == Keyboard::Right)
				menu_index.x++;
		}

		if (event.key.code == Keyboard::Up && menu_index.y > 0)
			menu_index.y--;
		if (event.key.code == Keyboard::Down && menu_index.y < 2)
			menu_index.y++;
	}
	
	if (isCancelAction(event.key.code)) {
		switch (state) {
		case BattleState::Menu_Act_Select:
		case BattleState::Menu_Fight_Select:
		case BattleState::Menu_Item_Select:
		case BattleState::Menu_Mercy_Select:
			state = BattleState::Menu_Base;
			break;
		case BattleState::Menu_Act_Options:
			state = BattleState::Menu_Act_Select;
			break;
		default:
			break;
		}
	}
	if (event.type == Event::MouseButtonPressed)
		if (event.mouseButton.button == Mouse::Left)
			bullet_box.setOrigin(Vector2f(Mouse::getPosition(window)));
}

void GameStateFight::draw(sf::RenderWindow& window) {
	using namespace sf;	
	window.clear();

	
	window.draw(bullet_box);

	static UTText flavourText(dtm_mono, "* [No flavour text loaded.]");

	static bool flavourTextInitialized = false;
	if (!flavourTextInitialized) {
		flavourTextInitialized = true;
		flavourText.addEffect(new UTText::TypeWriterEffect(0.04f));
		flavourText.addEffect(new UTText::JitterEffect(.1f, 1.f));
		//flavourText.addEffect(new UTText::SwirlEffect(1.f, 3.f));
	}

	std::string text;

	switch (state) {
	case BattleState::Menu_Fight_Select:
	case BattleState::Menu_Act_Select:
	case BattleState::Menu_Mercy_Select:
		for (auto& enemy: enemies) {
			text += "  * " + enemy.m_name + "\n";
		}
		flavourText.setString(text);
	default:
		break;
	}
	
	flavourText.setPosition(52,270);
	flavourText.setCharacterSize(27);

	if (!bullet_box.transitionOver())
		flavourText.resetEffects();
	
	//Don't draw the flavour text during attacks
	switch (state) {
	case BattleState::Attack_AttackPhase:
	case BattleState::Attack_PreDialogue:
	case BattleState::Attack_PostDialogue:
		break;
	default:
		window.draw(flavourText);
	}

	
	//Set texture rects...
	btn_fight.setTextureRect(	IntRect(0,	0,110,42));
	btn_act.setTextureRect(		IntRect(110,0,110,42));
	btn_item.setTextureRect(	IntRect(220,0,110,42));
	btn_mercy.setTextureRect(	IntRect(330,0,110,42));

	//...and change one of them again if it's the active button.
	if (state != BattleState::Attack_AttackPhase)
	switch(current_menu_choice) {
	case 0:
		btn_fight.setTextureRect(IntRect(0,42,110,42));
		break;
	case 1:
		btn_act.setTextureRect(IntRect(110,42,110,42));
		break;
	case 2:
		btn_item.setTextureRect(IntRect(220,42,110,42));
		break;
	case 3:
		btn_mercy.setTextureRect(IntRect(330,42,110,42));
		break;
	default:
		throw std::exception("Invalid menu state.");
	}
	
	RectangleShape hpBar({25.f,21.f});
	RectangleShape hpBarBack({25.f,21.f});

	hpBar.setPosition(275, 400);
	hpBarBack.setPosition(275, 400);
	hpBar.setSize(Vector2f(std::ceilf(25.f * static_cast<float>(player.m_health) / static_cast<float>(player.m_maxHealth)),21.f));

	hpBar.setFillColor(Color(255,255,0));
	hpBarBack.setFillColor(Color(255,0,0));

	window.draw(hpBarBack);
	window.draw(hpBar);

	
	static Texture hp_txt_texture;
	static bool hp_txt_initialized = false;
	if (!hp_txt_initialized) {
		hp_txt_initialized = true;
		hp_txt_texture.loadFromFile("assets/menu/hp_txt.png");
	}
	static Sprite hp_txt(hp_txt_texture);
	hp_txt.setPosition(244.f, 405.f);
	window.draw(hp_txt);

	window.draw(btn_fight);
	window.draw(btn_act);
	window.draw(btn_item);
	window.draw(btn_mercy);


	//Draw text
	UTText playerName(mars_need_cunnilingus, player.m_name + "   LV " +std::to_string(player.m_lv));
	playerName.setCharacterSize(24);
	playerName.setPosition(30.f, 394.f);
	window.draw(playerName);

	//playerName.setPosition(132.f, 394.f);
	//window.draw(playerName);
	
	playerName.setString(std::to_string(player.m_health) + " / " + std::to_string(player.m_maxHealth));
	playerName.setPosition(314.f, 394.f);
	window.draw(playerName);




	window.draw(player);


	

	imdraw();
	
	window.display();
}

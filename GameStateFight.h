#pragma once
#include "GameState.h"
#include "Player.h"
#include "BulletBox.h"
#include "Attack.h"
#include "Enemy.h"

class GameStateFight final : public GameState
{
private:
	Attack* current_attack = nullptr;

	enum class BattleState {
		Menu_Attacking,
		Menu_Base,
		Menu_Fight_Select,
		Menu_Fight_Move,
		Menu_Fight_Hit,
		Menu_Act_Select,
		Menu_Act_Options,
		Menu_Act_Response,
		Menu_Item_Select,
		Menu_Item_Response,
		Menu_Mercy_Select,
		Menu_Mercy_Response,
		Attack_PreDialogue,
		Attack_AttackPhase,
		Attack_PostDialogue
	};
	BattleState state = BattleState::Attack_AttackPhase;

	static inline bool isSelectState(BattleState state);

	static inline bool isConfirmAction(sf::Keyboard::Key key);

	static inline bool isCancelAction(sf::Keyboard::Key key);


	sf::Sprite btn_fight;
	sf::Sprite btn_act;
	sf::Sprite btn_item;
	sf::Sprite btn_mercy;
	sf::Texture btns_texture;

	BulletBox bullet_box;
	Player player;

	void attack_end();

	void attack_begin();


	void imdraw();

	std::vector<Enemy> enemies;
	int current_menu_choice = 0;
	sf::Vector2i menu_index;
	float attackTimer;
	std::vector<std::string> flavour_text;
	
public:
	explicit GameStateFight(GameStateManager* manager);

	void start() override;
	void update(sf::Time delta) override;
	void event(sf::Event event, sf::RenderWindow& window) override;
	void draw(sf::RenderWindow& window) override;
};


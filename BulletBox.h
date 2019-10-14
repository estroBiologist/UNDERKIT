#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

template<typename T>
T lerp(T a, T b, float f) {
	return (a * f) + (b * (1.f - f));
}

template<typename T>
T short_angle_dist(T a0,T a1) {
    auto max = 3.14159f *2;
    auto da = std::fmod(a1 - a0, max);
    return std::fmod(2*da, max )- da;
}
template<typename T>
T angle_lerp(T a0,T a1,float t) {
    return a0 + short_angle_dist(a0,a1) * (1.0f - t);
}

class BulletBox : public sf::Drawable {
private:
	BulletBox* goal = nullptr;
	static constexpr float threshold = 0.3f;

public:
	void update();

	void resolveCollision(sf::FloatRect& boundingBox) const;

	void set(const BulletBox& goal);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	sf::Vector2f origin = {320.f, 240.f};
	sf::Vector2f size = {140.f, 140.f};
	
	static bool attacking;
	float angle = 0.f;
	float borderSize = 5.f;
	sf::Color borderColor = sf::Color::White;
	sf::Color backgroundColor = sf::Color::Black;

	enum TransitionMethod {
		Classic,
		Modern,
		Instant
	};
	TransitionMethod transitionMethod = Modern;

	bool transitionOver() const;
	void setOrigin(sf::Vector2f origin);
	void setSize(sf::Vector2f size);
	void setBorderColor(sf::Color color);
	void setBackgroundColor(sf::Color color);
	
	
};

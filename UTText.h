#pragma once
#include <SFML/Graphics.hpp>

class UTText: public sf::Drawable, public sf::Transformable
{
private:
	const sf::Font* m_font;
	sf::String m_text;
	float m_baselineOffset;
	unsigned int m_size;

public:
	~UTText();
	void resetEffects();

	class TextEffect {
	public:
		virtual ~TextEffect() = default;
	private:
		friend class UTText;
		virtual void apply(sf::Vector2f& position, int characterNum, sf::Color& color) = 0;
		virtual void reset() = 0;
	};





	
	class TypeWriterEffect : public TextEffect {
		int currentChar = 0;
		sf::Clock clock;
		float interval;
		virtual void reset() {
			clock.restart();
			currentChar = 0;
		}
		virtual void apply(sf::Vector2f& position, int characterNum, sf::Color& color) {
			while (clock.getElapsedTime().asSeconds() > interval) {
				currentChar++;
				clock.restart();
			}
			if (characterNum >= currentChar)
				color = sf::Color::Transparent;
			else color = sf::Color::White;
		}
	public:
		TypeWriterEffect(float interval_secs) : interval(interval_secs) {}
	};



	
	
	class JitterEffect : public TextEffect {
		double chance;
		float intensity;
		virtual void reset() {
		}
		virtual void apply(sf::Vector2f& position, int characterNum, sf::Color& color) {
			double randchance = static_cast<float>(std::rand() % 10000) / 100.f;
			if (randchance <= chance) {

				float xoffset = std::fmod(std::rand(), 3) - 1 * intensity;
				float yoffset = std::fmod(std::rand(), 3) - 1 * intensity;
				position += {xoffset, yoffset};
			}
		}
	public:
		
		JitterEffect(double chance, float intensity) : chance(chance), intensity(intensity) {}
	};



	
	class SwirlEffect : public TextEffect {
		double speed;
		sf::Clock clock;

		float radius;
		virtual void reset() {
			clock.restart();
		}
		virtual void apply(sf::Vector2f& position, int characterNum, sf::Color& color) {
			float rot = speed * 5 * clock.getElapsedTime().asSeconds() + static_cast<float>(characterNum) / 3.f;
			
			float xoffset = std::sin(rot)* radius;
			float yoffset = std::cos(rot) * radius;
			position += {xoffset, yoffset};
		}










		
	public:
		
		SwirlEffect(float speed, float radius) : speed(speed), radius(radius) {}
	};


	
private:
	std::vector<TextEffect*> m_effects;
public:
	void addEffect(TextEffect*effect);
	
	UTText();
	UTText(const sf::Font& font, const sf::String& string);
	void setFont(const sf::Font& font);
	void setString(const sf::String& string);
	void setCharacterSize(unsigned int size);
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


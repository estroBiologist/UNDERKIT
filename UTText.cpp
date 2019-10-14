#include "UTText.h"

UTText::~UTText() {
	for (auto effect : m_effects)
		delete effect;
}

void UTText::resetEffects() { for (auto effect : m_effects) effect->reset(); }

void UTText::addEffect(TextEffect* effect) {
	m_effects.push_back(effect);
}

UTText::UTText() {
}

UTText::UTText(const sf::Font& font, const sf::String& string) : m_font(&font), m_text(string) {
	
}

void UTText::setFont(const sf::Font& font) {
	m_font = &font;
}

void UTText::setString(const sf::String& string) {
	m_text = string;
	resetEffects();
}

void UTText::setCharacterSize(unsigned size) {
	m_size = size;
}

void UTText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	float advance = 0;
	unsigned int line = 0;
	for (unsigned int i = 0; i < m_text.getSize(); i++) {
		sf::Sprite spr;

		auto character = m_text[i];
		if (character == '\n') {
			advance = 0;
			line++;
			if (m_text[i+1] == '\r')
				i++;
			continue;
		}
		
		const sf::Glyph& glyph = m_font->getGlyph(character, m_size, false);
		spr.setTexture(m_font->getTexture(m_size));
		spr.setTextureRect(glyph.textureRect);

		//Calculate character position
		sf::Vector2f pos(getPosition());
		pos.x += advance;
		pos.y += (line + 1) * m_size + glyph.bounds.top;
		

		sf::Color sprColor = spr.getColor();

		for (auto effect: m_effects)
			effect->apply(pos, i, sprColor);

		spr.setColor(sprColor);
		spr.setPosition(pos);
		target.draw(spr);
		advance += glyph.advance;
	}
}

#include "BulletBox.h"

bool BulletBox::attacking = false;

void BulletBox::update() {
	static float lerpSpeed = 0.8f;
	bool goalReached = true;
	if (goal) {
		if (std::abs(goal->angle - this->angle) > threshold) {
			angle = angle_lerp(angle, goal->angle, lerpSpeed);
			goalReached = false;
		}
		if (std::abs(goal->origin.x - this->origin.x) > threshold) {
			origin.x = lerp(origin.x, goal->origin.x, lerpSpeed);
			goalReached = false;
		}
		if (std::abs(goal->origin.y - this->origin.y) > threshold) {
			origin.y = lerp(origin.y, goal->origin.y, lerpSpeed);
			goalReached = false;
		}
		if (std::abs(goal->size.x - this->size.x) > threshold) {
			size.x = lerp(size.x, goal->size.x, lerpSpeed);
			goalReached = false;
		}
		if (std::abs(goal->size.y - this->size.y) > threshold) {
			size.y = lerp(size.y, goal->size.y, lerpSpeed);
			goalReached = false;
		}
		if (goalReached) {
			delete goal;
			goal = nullptr;
		}
	}
}

void BulletBox::resolveCollision(sf::FloatRect& boundingBox) const {
	auto thisBox = sf::FloatRect(
	origin - size / 2.f, size);

	if (boundingBox.left < thisBox.left)
		boundingBox.left = thisBox.left;
	if (boundingBox.left + boundingBox.width > thisBox.left + thisBox.width)
		boundingBox.left = thisBox.left + thisBox.width - boundingBox.width;
	if (boundingBox.top < thisBox.top)
		boundingBox.top = thisBox.top;
	if (boundingBox.top + boundingBox.height > thisBox.top + thisBox.height)
		boundingBox.top = thisBox.top + thisBox.height - boundingBox.height;
}

void BulletBox::set(const BulletBox& goal) {
	delete this->goal;
	this->goal = new BulletBox(goal);
}

void BulletBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	static sf::RectangleShape box;
	
	box.setSize(size);
	box.setFillColor(backgroundColor);
	box.setOutlineColor(borderColor);
	box.setOutlineThickness(borderSize);
	box.setOrigin(box.getSize() / 2.f);
	box.setPosition(origin);
	target.draw(box, states);
}

bool BulletBox::transitionOver() const {
	if (goal) {
		if (std::abs(goal->angle - this->angle) > threshold) {
			return false;
		}
		if (std::abs(goal->origin.x - this->origin.x) > threshold) {
			return false;
		}
		if (std::abs(goal->origin.y - this->origin.y) > threshold) {
			return false;
		}
		if (std::abs(goal->size.x - this->size.x) > threshold) {
			return false;
		}
		if (std::abs(goal->size.y - this->size.y) > threshold) {
			return false;
		}
		return true;
	}
}

void BulletBox::setOrigin(sf::Vector2f origin) {
	if (!goal)
		goal = new BulletBox(*this);
	goal->origin = origin;
}



void BulletBox::setSize(sf::Vector2f size) {
	if (!goal)
		goal = new BulletBox(*this);
	goal->size = size;
}

void BulletBox::setBorderColor(sf::Color color) {
	if (!goal)
		goal = new BulletBox(*this);
	goal->borderColor = color;
}

void BulletBox::setBackgroundColor(sf::Color color) {
	if (!goal)
		goal = new BulletBox(*this);
	goal->backgroundColor = color;
}

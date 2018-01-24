#include "text.h"

namespace ui {

	Text::Text(std::string _text, const sf::Font& _font, sf::Color color, int charsize)
		: text(_text, _font, charsize) {
		text.setFillColor(color);
		updateSize();
		disabled = true;
		background_color = sf::Color(0x0);
	}
	void Text::setText(std::string _text){
		text.setString(_text);
		updateSize();
	}
	std::string Text::getText(){
		return text.getString().toAnsiString();
	}
	void Text::clearText(){
		text.setString("");
		updateSize();
	}
	void Text::setCharacterSize(unsigned int size){
		text.setCharacterSize(size);
		updateSize();
	}
	unsigned int Text::getCharacterSize() const {
		return text.getCharacterSize();
	}
	void Text::setTextColor(sf::Color color){
		text.setFillColor(color);
	}
	const sf::Color& Text::getTextColor() const {
		return text.getFillColor();
	}
	void Text::setBackGroundColor(sf::Color color){
		background_color = color;
	}
	const sf::Color& Text::getBackGroundColor() const {
		return background_color;
	}
	void Text::render(sf::RenderWindow& rw){
		sf::RectangleShape rect(size);
		rect.setFillColor(background_color);
		rw.draw(rect);
		rw.draw(text);
	}
	void Text::updateSize(){
		sf::FloatRect bounds = text.getGlobalBounds();
		if (text.getString().isEmpty()){
			size.x = (float)text.getCharacterSize();
			size.y = (float)text.getCharacterSize();
		} else {
			size.x = bounds.width + bounds.left;
			size.y = bounds.height + bounds.top;
		}
	}

}
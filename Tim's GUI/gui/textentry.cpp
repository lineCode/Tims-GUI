#include "textentry.h"
#include "context.h"
#include "gui.h"

namespace ui {

	TextEntry::TextEntry(const sf::Font& font, int charsize)
		: Text("", font, sf::Color(0xFF), charsize) {

	}
	TextEntry::TextEntry(std::string str, const sf::Font& font, int charsize, sf::Color _text_color, sf::Color _bg_color)
		: Text(str, font, _text_color, charsize) {
		setBackGroundColor(_bg_color);
	}
	void TextEntry::beginTyping(){
		grabFocus();
		getContext().setTextEntry(this);
	}
	void TextEntry::endTyping(){
		if (getContext().getTextEntry() == this){
			getContext().setTextEntry(nullptr);
		}
	}
	void TextEntry::moveTo(vec2 pos){
		for (int i = 0; i < text.getString().getSize(); i++){
			vec2 charpos = text.findCharacterPos(i);
			if (pos.x < charpos.x){
				cursor_index = i - 1;
				updateSize();
				return;
			}
		}
		cursor_index = text.getString().getSize();
		updateSize();
	}
	void TextEntry::onReturn(std::string entered_text){

	}
	void TextEntry::onType(std::string full_text){

	}
	void TextEntry::render(sf::RenderWindow& renderwindow){
		sf::RectangleShape rect(size);
		rect.setFillColor(background_color);
		renderwindow.draw(rect);
		renderwindow.draw(text);
		if (getContext().getTextEntry() == this){
			positionCursor();
			sf::RectangleShape rect2(vec2(cursor_width, (float)text.getCharacterSize()));
			rect2.setFillColor(sf::Color(
				text.getFillColor().r,
				text.getFillColor().g,
				text.getFillColor().b,
				(uint8_t)(128 * (0.5 + 0.5 * sin(getProgramTime() * 3.141592654 * 2.0)))));
			rect2.setPosition(vec2(cursor_pos, 0));
			renderwindow.draw(rect2);
		}
	}
	void TextEntry::onLeftClick(int clicks){
		beginTyping();
		moveTo(localMousePos());
	}
	void TextEntry::onFocus(){
		beginTyping();
	}
	void TextEntry::write(char ch){
		if (ch != '\n' && ch != '\r'){
			std::string oldstring = text.getString();
			text.setString(oldstring.substr(0, cursor_index) + ch + oldstring.substr(cursor_index, oldstring.size() - 1));
			cursor_index += 1;
			updateSize();
			onType(text.getString());
		}
	}
	void TextEntry::onBackspace(){
		if (!text.getString().isEmpty() && cursor_index > 0){
			std::string newstring = text.getString();
			newstring.erase(newstring.begin() + cursor_index - 1);
			cursor_index -= 1;
			text.setString(newstring);
			updateSize();
		}
		onType(text.getString());
	}
	void TextEntry::onDelete(){
		if (!text.getString().isEmpty() && cursor_index < text.getString().getSize()){
			std::string newstring = text.getString();
			newstring.erase(cursor_index, 1);
			text.setString(newstring);
			updateSize();
		}
		onType(text.getString());
	}
	void TextEntry::onLeft(){
		if (cursor_index > 0){
			cursor_index -= 1;
			updateSize();
		}
	}
	void TextEntry::onRight(){
		if (cursor_index < text.getString().getSize()){
			cursor_index += 1;
			updateSize();
		}
	}
	void TextEntry::onHome(){
		cursor_index = 0;
		updateSize();
	}
	void TextEntry::onEnd(){
		cursor_index = (unsigned)text.getString().getSize();
		updateSize();
	}
	void TextEntry::positionCursor(){
		cursor_index = std::min(cursor_index, text.getString().getSize());
		cursor_pos = text.findCharacterPos(cursor_index).x;
		if (cursor_index == text.getString().getSize()){
			cursor_width = text.getCharacterSize() * 0.5f;
		} else {
			cursor_width = text.findCharacterPos(cursor_index + 1).x - cursor_pos;
		}
	}

}
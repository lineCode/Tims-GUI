#pragma once

#include "GUI/Element.hpp"

namespace ui {

	struct Text : InlineElement {

		Text(std::string _text, const sf::Font& _font, sf::Color color = sf::Color(0xFF), unsigned charsize = 15u, TextStyle style = TextStyle::Regular);
		Text(std::wstring _text, const sf::Font& _font, sf::Color color = sf::Color(0xFF), unsigned charsize = 15u, TextStyle style = TextStyle::Regular);

		void setText(std::string _text);
		void setText(std::wstring _text);
		std::wstring getText();
		void clearText();

		void setCharacterSize(unsigned int size);
		unsigned int getCharacterSize() const;

		void setTextColor(sf::Color color);
		const sf::Color& getTextColor() const;

		void setStyle(TextStyle style);
		TextStyle getStyle() const;

		void render(sf::RenderWindow& renderwin) override;

	protected:

		void updateSize();

		sf::Text text;
	};

}
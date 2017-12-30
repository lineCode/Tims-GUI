#pragma once
#include "gui.h"
#include "stringhelpers.h"

namespace ui {
	namespace helpers {

		// TODO:
		// slider

		struct CallbackButton : Window {
			CallbackButton(const std::string& label, const sf::Font& font, const std::function<void()>& _onClick) : onClick(_onClick) {
				auto text = new Text(label, font);
				addChildWindow(text, vec2(0, 0));
				size = text->size;
			}

			void onLeftClick(int clicks) override {
				if (onClick){
					onClick();
				}
			}

			void render(sf::RenderWindow& rw) override {
				sf::RectangleShape rect;
				rect.setSize(size);
				rect.setFillColor(sf::Color(0xCCCCCCFF));
				rect.setOutlineColor(sf::Color(0xFF));
				rect.setOutlineThickness(1);
				rw.draw(rect);
				renderChildWindows(rw);
			}

			private:
			std::function<void()> onClick;
		};

		struct ToggleButton : Window {
			ToggleButton(bool& _target, float _size = 20.0f) : target(_target) {
				size = vec2(_size, _size);
			}

			void onLeftClick(int clicks) override {
				target = !target;
			}

			void render(sf::RenderWindow& rw) override {
				sf::CircleShape circle;
				circle.setRadius(size.x / 2);
				circle.setFillColor(sf::Color(0xFFFFFFFF));
				rw.draw(circle);
				if (target){
					circle.setFillColor(sf::Color(0xFF));
					circle.setRadius(size.x * 0.45f);
					circle.setPosition(vec2(size.x * 0.05f, size.x * 0.05f));
					rw.draw(circle);
				}
			}

			private:
			bool& target;
		};

		struct PullDownMenu : Window {
			/*
			on-change callback?
			*/
		};

		struct Slider : Window {
			/*
			vertical/horizontal?
			generic/custom rendering?
			on-change callback?
			*/
		};

		/*
		better name?
		validation function?
		on-change callback?
		on-return callback?
		*/
		struct TextEntryHelper : TextEntry {
			TextEntryHelper(const std::string& str, const sf::Font& font,
						const std::function<void(const std::string&)>& _on_return,
						const std::function<bool(const std::string&)>& _validate = {},
						const std::function<void(const std::string&)>& _on_change = {})
				: TextEntry(str, font), on_return(_on_return), validate(_validate), on_change(_on_change) {

				setBackGroundColor(sf::Color(0xFFFFFFFF));
				setTextColor(sf::Color(0xFF));
			}

			void onReturn(const std::string& entered_text) override {
				if (on_return && (!validate || validate(entered_text))){
					on_return(entered_text);
				}
			}

			void onType(const std::string& full_text) override {
				if (validate){
					if (validate(full_text)){
						setBackGroundColor(sf::Color(0xFFFFFFFF));
					} else {
						setBackGroundColor(sf::Color(0xFF8080FF));
					}
				}
				if (on_change){
					on_change(full_text);
				}
			}

			private:
			std::function<void(const std::string&)> on_return;
			std::function<bool(const std::string&)> validate;
			std::function<void(const std::string&)> on_change;
		};

		struct NumberTextEntry : TextEntry {
			NumberTextEntry(float default, float _min, float _max, const std::function<void(float)>& _callback, const sf::Font& font)
				: TextEntry(toString(default), font), value(default), min(_min), max(_max), callback(_callback) {
				setBackGroundColor(sf::Color(0xFFFFFFFF));
				setTextColor(sf::Color(0xFF));
			}

			void onReturn(const std::string& entered_text) override {
				if (validate(entered_text) && callback){
					float val = stringToFloat(entered_text);
					callback(val);
				}
			}

			void onType(const std::string& full_text) override {
				if (validate(full_text)){
					setBackGroundColor(sf::Color(0xFFFFFFFF));
				} else {
					setBackGroundColor(sf::Color(0xFF8080FF));
				}
			}

			private:
			float value;
			const float min;
			const float max;
			std::function<void(float)> callback;

			bool validate(const std::string& text){
				float val = stringToFloat(text);
				return !std::isnan(val);
			}
		};
	
		
	}
}
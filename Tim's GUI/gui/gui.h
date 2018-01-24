#pragma once

#include "window.h"
#include "text.h"
#include "textentry.h"
#include "context.h"

// TODO: reconsider references to improve move semantics and rvo

namespace ui {

	Window* root();

	void addKeyboardCommand(sf::Keyboard::Key trigger_key, std::function<void()> handler);
	void addKeyboardCommand(sf::Keyboard::Key trigger_key, std::vector<sf::Keyboard::Key> required_keys, std::function<void()> handler);
	void setQuitHandler(std::function<bool()> handler);

	long double getProgramTime();

	vec2 getScreenSize();

	vec2 getMousePos();

	Context& getContext();

	void init(unsigned width = 500, unsigned height = 400, std::string title = "Behold", int target_fps = 30);

	void quit(bool force = false);

	void run();
}
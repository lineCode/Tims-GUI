#include "guiwindow.h"
#include "guicontext.h"
#include <set>

namespace ui {

	Window::~Window(){
		if (parent){
			parent->releaseChildWindow(this);
			if (getContext().getCurrentWindow() == this){
				getContext().focusTo(parent);
			}
		}
		getContext().clearTransitions(this);
		if (getContext().getDraggingWindow() == this){
			getContext().setDraggingWindow(nullptr);
		}
		clear();
	}
	void Window::close(){
		delete this;
	}
	bool Window::hit(vec2 testpos){
		return ((testpos.x >= 0.0f) && (testpos.x < size.x) && (testpos.y >= 0.0f) && (testpos.y < size.y));
	}
	vec2 Window::localMousePos(){
		vec2 pos = (vec2)sf::Mouse::getPosition(getContext().getRenderWindow());
		Window *window = this;
		while (window){
			pos -= window->pos;
			window = window->parent;
		}
		return(pos);
	}
	vec2 Window::absPos(){
		vec2 pos = vec2();
		Window *window = this;
		while (window != nullptr){
			pos += window->pos;
			window = window->parent;
		}
		return(pos);
	}
	void Window::onLeftClick(int clicks){

	}
	void Window::onLeftRelease(){

	}
	void Window::onRightClick(int clicks){

	}
	void Window::onRightRelease(){

	}
	void Window::onScroll(double delta_x, double delta_y){

	}
	void Window::startDrag(){
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)){
			if (getContext().getDraggingWindow() != this){
				getContext().focusTo(this);
				getContext().setDraggingWindow(this, (vec2)sf::Mouse::getPosition(getContext().getRenderWindow()) - pos);
			}
		}
	}
	void Window::onDrag(){

	}
	void Window::onHover(){

	}
	void Window::onHoverWithWindow(Window *drag_window){

	}
	bool Window::onDropWindow(Window *window){
		return false;
	}
	void Window::onFocus(){

	}
	bool Window::isFocused(){
		return getContext().getCurrentWindow() == this;
	}
	void Window::onLoseFocus(){

	}
	void Window::focusToNextWindow(){
		if (parent && isFocused()){
			Window* next = nullptr;
			for (Window* window : parent->childwindows){
				if (window->pos.y > pos.y){
					if (!next || window->pos.y < next->pos.y){
						next = window;
					}
				} else if (window->pos.y == pos.y){
					if (window->pos.x > pos.x){
						if (!next || window->pos.x < next->pos.x){
							next = window;
						}
					}
				}
			}
			if (next == nullptr){
				next = parent->childwindows.front();
				for (Window* window : parent->childwindows){
					if (window->pos.y < next->pos.y){
						next = window;
					} else if (window->pos.y == next->pos.y){
						if (window->pos.x < next->pos.x){
							next = window;
						}
					}
				}
			}
			getContext().focusTo(next);
		}
	}
	void Window::focusToPreviousWindow(){
		if (parent && isFocused()){
			Window* prev = nullptr;
			for (Window* window : parent->childwindows){
				if (window->pos.y < pos.y){
					if (!prev || window->pos.y > prev->pos.y){
						prev = window;
					}
				} else if (window->pos.y == pos.y){
					if (window->pos.x < pos.x){
						if (!prev || window->pos.x > prev->pos.x){
							prev = window;
						}
					}
				}
			}
			if (prev == nullptr){
				prev = parent->childwindows.front();
				for (Window* window : parent->childwindows){
					if (window->pos.y > prev->pos.y){
						prev = window;
					} else if (window->pos.y == prev->pos.y){
						if (window->pos.x > prev->pos.x){
							prev = window;
						}
					}
				}
			}
			getContext().focusTo(prev);
		}
	}
	void Window::grabFocus(){
		getContext().focusTo(this);
	}
	void Window::onKeyDown(sf::Keyboard::Key key){

	}
	void Window::onKeyUp(sf::Keyboard::Key key){

	}
	bool Window::keyDown(sf::Keyboard::Key key){
		if (getContext().getCurrentWindow() == this){
			return(sf::Keyboard::isKeyPressed(key));
		} else {
			return(false);
		}
	}
	Window::Alignment::Alignment(Type _type, Window* _relative_to, float _margin){
		type = _type;
		relative_to = _relative_to;
		margin = _margin;
	}
	Window::XAlignment::XAlignment(Type type, Window* relative_to, float margin) : Alignment(type, relative_to, margin) {

	}
	Window::YAlignment::YAlignment(Type type, Window* relative_to, float margin) : Alignment(type, relative_to, margin) {

	}
	void Window::setXAlign(XAlignment xalignment){
		xalign = xalignment;
		if (parent){
			parent->children_aligned = false;
		}
	}
	void Window::setYAlign(YAlignment yalignment){
		yalign = yalignment;
		if (parent){
			parent->children_aligned = false;
		}
	}
	void Window::align(){
		if (xalign.type != Alignment::None){
			if (xalign.relative_to){
				float relx = xalign.relative_to == parent ? 0.0f : xalign.relative_to->pos.x;
				switch (xalign.type){
					case Alignment::Before:
						pos.x = relx - size.x - xalign.margin;
						break;
					case Alignment::After:
						pos.x = relx + xalign.relative_to->size.x + xalign.margin;
						break;
					case Alignment::Center:
						pos.x = relx + xalign.relative_to->size.x * 0.5f - size.x * 0.5f;
						break;
					case Alignment::InsideMin:
						pos.x = relx + xalign.margin;
						break;
					case Alignment::InsideMax:
						pos.x = relx + xalign.relative_to->size.x - size.x - xalign.margin;
						break;
				}
			}
		}

		if (yalign.type != Alignment::None){
			if (yalign.relative_to){
				float rely = yalign.relative_to == parent ? 0.0f : yalign.relative_to->pos.y;
				switch (yalign.type){
					case Alignment::Before:
						pos.y = rely - size.y - yalign.margin;
						break;
					case Alignment::After:
						pos.y = rely + yalign.relative_to->size.y + yalign.margin;
						break;
					case Alignment::Center:
						pos.y = rely + yalign.relative_to->size.y * 0.5f - size.y * 0.5f;
						break;
					case Alignment::InsideMin:
						pos.y = rely + yalign.margin;
						break;
					case Alignment::InsideMax:
						pos.y = rely + yalign.relative_to->size.y - size.y - yalign.margin;
						break;
				}
			}
		}
	}
	void Window::alignChildren(){
		std::set<Window*> remaining;

		for (Window* w : childwindows){
			remaining.insert(w);
		}

		while (remaining.size() > 0){
			bool found = false;
			for (auto it = remaining.begin(); !found && it != remaining.end(); it++){
				Window* xwin = (*it)->xalign.relative_to;
				Window* ywin = (*it)->yalign.relative_to;
				if ((!xwin || (remaining.find(xwin) == remaining.end())) && (!ywin || (remaining.find(ywin) == remaining.end()))){
					(*it)->align();
					remaining.erase(it);
					found = true;
					break;
				}
			}
			if (!found){
				throw std::runtime_error("The relative alignments are cyclical");
			}
		}
	}

	Window::XAlignment Window::noAlignX(){
		return XAlignment(Alignment::None);
	}
	Window::XAlignment Window::leftOf(Window* window, float margin){
		return XAlignment(Alignment::Before, window, margin);
	}
	Window::XAlignment Window::rightOf(Window* window, float margin){
		return XAlignment(Alignment::After, window, margin);
	}
	Window::XAlignment Window::middleOfX(Window* window){
		return XAlignment(Alignment::Center, window, 0.0f);
	}
	Window::XAlignment Window::insideLeft(Window* window, float margin){
		return XAlignment(Alignment::InsideMin, window, margin);
	}
	Window::XAlignment Window::insideRight(Window* window, float margin){
		return XAlignment(Alignment::InsideMax, window, margin);
	}
	Window::YAlignment Window::noAlignY(){
		return YAlignment(Alignment::None);
	}
	Window::YAlignment Window::above(Window* window, float margin){
		return YAlignment(Alignment::Before, window, margin);
	}
	Window::YAlignment Window::below(Window* window, float margin){
		return YAlignment(Alignment::After, window, margin);
	}
	Window::YAlignment Window::middleOfY(Window* window){
		return YAlignment(Alignment::Center, window, 0.0f);
	}
	Window::YAlignment Window::insideTop(Window* window, float margin){
		return YAlignment(Alignment::InsideMin, window, margin);
	}
	Window::YAlignment Window::insideBottom(Window* window, float margin){
		return YAlignment(Alignment::InsideMax, window, margin);
	}
	void Window::addChildWindow(Window *window){
		if (window->parent != nullptr){
			throw;
		}
		window->parent = this;
		childwindows.insert(childwindows.begin(), window);
	}
	void Window::addChildWindow(Window *window, vec2 pos){
		window->pos = pos;
		addChildWindow(window);
	}
	void Window::addChildWindow(Window *window, XAlignment xalignment){
		addChildWindow(window);
		window->setXAlign(xalignment);
	}
	void Window::addChildWindow(Window *window, YAlignment yalignment){
		addChildWindow(window);
		window->setYAlign(yalignment);
	}
	void Window::addChildWindow(Window *window, XAlignment xalignment, YAlignment yalignment){
		addChildWindow(window);
		window->setXAlign(xalignment);
		window->setYAlign(yalignment);
	}
	void Window::addChildWindow(Window* window, float xpos, YAlignment yalignment){
		addChildWindow(window);
		window->pos.x = xpos;
		window->setYAlign(yalignment);
	}
	void Window::addChildWindow(Window* window, XAlignment xalignment, float ypos){
		addChildWindow(window);
		window->setXAlign(xalignment);
		window->pos.y = ypos;
	}
	void Window::releaseChildWindow(Window* window){
		for (int i = 0; i < childwindows.size(); i++){
			if (childwindows[i] == window){
				childwindows[i]->parent = nullptr;
				childwindows.erase(childwindows.begin() + i);
				return;
			}
		}
		throw;
	}
	void Window::bringToFront(){
		if (!parent){
			return;
		}
		for (int i = 0; i < parent->childwindows.size(); i++){
			if (parent->childwindows[i] == this){
				parent->childwindows.erase(parent->childwindows.begin() + i);
				parent->childwindows.insert(parent->childwindows.begin(), this);
				return;
			}
		}
		throw;
	}
	void Window::clear(){
		while (childwindows.size() > 0){
			childwindows[0]->close();
		}
	}
	Window* Window::findWindowAt(vec2 _pos){
		if (!visible || disabled){
			return nullptr;
		}

		if (clipping && ((_pos.x < 0.0f) || (_pos.x > size.x) || (_pos.y < 0.0) || (_pos.y > size.y))){
			return false;
		}

		Window* window = nullptr;
		for (int i = 0; i < childwindows.size() && window == nullptr; i++){
			window = childwindows[i]->findWindowAt(_pos - childwindows[i]->pos);
			if (window && (window == getContext().getDraggingWindow())){
				window = nullptr;
			}
		}
		if (window){
			return window;
		}

		if (this->hit(_pos)){
			return this;
		}

		return nullptr;
	}
	void Window::render(sf::RenderWindow& renderwindow){
		sf::RectangleShape rectshape;
		rectshape.setSize(size);
		rectshape.setFillColor(sf::Color((((uint32_t)std::hash<Window*>{}(this)) & 0xFFFFFF00) | 0x80));
		rectshape.setOutlineColor(sf::Color(0xFF));
		rectshape.setOutlineThickness(1);
		renderwindow.draw(rectshape);
		renderChildWindows(renderwindow);
	}
	void Window::renderChildWindows(sf::RenderWindow& renderwindow){
		if (!children_aligned){
			alignChildren();
			children_aligned = true;
		}
		for (int i = (int)childwindows.size() - 1; i >= 0; i -= 1){
			if (childwindows[i]->visible){
				if (childwindows[i]->clipping){
					getContext().translateView(childwindows[i]->pos);

					sf::FloatRect rect = getContext().getClipRect();
					vec2 pos = getContext().getViewOffset();
					getContext().intersectClipRect(sf::FloatRect(-pos, childwindows[i]->size));
					getContext().updateView();
					childwindows[i]->render(renderwindow);
					getContext().setClipRect(rect);
					getContext().translateView(-childwindows[i]->pos);
					getContext().updateView();
				} else {
					getContext().translateView(childwindows[i]->pos);
					getContext().updateView();
					childwindows[i]->render(renderwindow);
					getContext().translateView(-childwindows[i]->pos);
					getContext().updateView();
				}
			}
		}
	}
	void Window::startTransition(double duration, const std::function<void(double)> transitionFn, const std::function<void()>& onComplete){
		getContext().addTransition(Transition(this, duration, transitionFn, onComplete));
	}
	const std::vector<Window*>& Window::getChildWindows() const {
		return childwindows;
	}
	Window* Window::getParent() const {
		return parent;
	}

}
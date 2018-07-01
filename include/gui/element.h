#pragma once

#include "gui/roundrectangle.h"

#include <SFML\Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>

typedef sf::Vector2f vec2;


namespace ui {
	
	using Key = sf::Keyboard::Key;

	// How an element is laid out and positioned relative to its parent and siblings
	enum class DisplayStyle {

		// positioned relative to parent according only to setPos()
		Free,

		// positioned on the same line as adjacent Inline elements
		Inline,

		// positioned on a unique line, and taking up the full width available
		Block,

		// positioned left of all inline elements
		FloatLeft,

		// positioned right of all inline elements
		FloatRight
	};

	// How an element's inline children are aligned horizontally
	enum class AlignStyle {
		// from the left edge
		Left,

		// from the right edge
		Right,

		// centered between the left and right edges
		Center,

		// spaced to fill the entire line
		Justify
	};

	enum class TextStyle : uint32_t {
		Regular = sf::Text::Text::Style::Regular,
		Bold = sf::Text::Text::Style::Bold,
		Italic = sf::Text::Text::Style::Italic,
		Underlined = sf::Text::Text::Style::Underlined,
		StrikeThrough = sf::Text::Text::Style::StrikeThrough,
	};

	struct Element : std::enable_shared_from_this<Element> {


		// default constructor
		Element(DisplayStyle _display_style);

		// virtual destructor for safe polymorphic destruction
		virtual ~Element();

		// clears and removes the element from its parent
		void close();

		// returns true if the element has been closed
		bool isClosed() const;

		// called when the element is closed; to be used for releasing resources reliably
		virtual void onClose();

		template<typename ElementType>
		std::shared_ptr<ElementType> getThisAs(){
			static_assert(std::is_base_of<Element, ElementType>::value, "ElementType must derive from ui::Element");
			return std::dynamic_pointer_cast<ElementType, Element>(shared_from_this());
		}

		// prevent the element from receiving input
		void disable();

		// allow the element the receive input
		void enable();

		// returns true if the element can receive input
		bool isEnabled() const;

		// set the visibility of the element
		void setVisible(bool is_visible);

		// returns true if the element is visible
		bool isVisible() const;

		// when true, limits rendering and input to within the bounding rectangle
		void setClipping(bool _clipping);

		// get the position (top-left corner of the element)
		vec2 getPos() const;

		// set the position (top-left corner of the element)
		void setPos(vec2 _pos);

		// get the width and height of the element
		vec2 getSize() const;

		// set the size. Choosing force = true will set both the min and max size
		void setSize(vec2 _size, bool force = false);

		// set the minimum and maximum size
		void setMinSize(vec2 _min_size);
		void setMinWidth(float width);
		void setMinHeight(float height);
		void setMaxSize(vec2 _max_size);
		void setMaxWidth(float width);
		void setMaxHeight(float height);

		// set the display style
		void setDisplayStyle(DisplayStyle style);

		// get the display style
		DisplayStyle getDisplayStyle() const;

		// set the horizontal alignment style
		void setAlignStyle(AlignStyle style);

		// get the horizontal alignment style
		AlignStyle getAlignStyle() const;

		// set the padding; spacing between content and border
		void setPadding(float _padding);

		// get the padding; spacing between content and border
		float getPadding() const;

		// set the margin; spacing between other self and other elements
		void setMargin(float _margin);

		// get the margin; spacing between other self and other elements
		float getMargin() const;

		// get the background color
		sf::Color getBackgroundColor() const;

		// set the background color
		void setBackgroundColor(sf::Color color);

		// get the border color
		sf::Color getBorderColor() const;

		// set the border color
		void setBorderColor(sf::Color color);

		// get the border radius
		float getBorderRadius() const;

		// set the border radius
		void setBorderRadius(float radius);

		// true if a test point (in local space) intercepts the element
		virtual bool hit(vec2 testpos) const;

		// the mouse's position relative to the element
		vec2 localMousePos() const;

		// the element's position relative to the root element
		vec2 rootPos() const;

		// called when the element is clicked on with the left mouse button
		// if false is returned, call will propagate to the parent
		// if true is returned, onLeftRelease will be invoked when the button is released
		virtual bool onLeftClick(int clicks);

		// called when the left mouse button is released
		virtual void onLeftRelease();

		// called when the element is clicked on with the right mouse button
		// if false is returned, call will propagate to the parent
		// if true is returned, onRightRelease will be invoked when the button is released
		virtual bool onRightClick(int clicks);

		// called when the right mouse button is released
		virtual void onRightRelease();

		// true if the left mouse button is down and the element is in focus
		bool leftMouseDown() const;

		// true if the right mouse button is down and the element is in focus
		bool rightMouseDown() const;

		// called when the mouse is scrolled and the element is in focus
		// if false is returned, call will propagate to the parent
		virtual bool onScroll(float delta_x, float delta_y);

		// begins the element being dragged by the mouse
		void startDrag();
		
		// called when the element is being dragged
		virtual void onDrag();

		// stops the mouse dragging the element
		void stopDrag();

		// true if the element is currently being dragged by the mouse
		bool dragging() const;

		// called when the mouse moves onto the element and its children
		virtual void onMouseOver();

		// called when the mouse moves off of the element and its children
		virtual void onMouseOut();

		// true if the mouse is currently over top of the element
		bool hovering() const;

		// called when the mouse is over the element
		// if false is returned, call will propagate to the parent
		virtual bool onHover();

		// called when the mouse is over the element with another element being dragged
		// if false is returned, call will propagate to the parent
		virtual bool onHoverWith(std::shared_ptr<Element> element);

		// drop the element (via the point local_pos, in local coordinates) onto the element below it
		void drop(vec2 local_pos);

		// called when a dragged element is released over the element
		// shall return false if the parent's method is to be invoked
		// if false is returned, call will propagate to the parent
		virtual bool onDrop(std::shared_ptr<Element> element);

		// called when the element gains focus
		virtual void onFocus();

		// true if the element is in focus
		bool inFocus() const;

		// called when the element loses focus
		virtual void onLoseFocus();

		// brings the element into focus
		void grabFocus();

		// called when a key is pressed and the element is in focus
		// if false is returned, call will propagate to the parent
		// if true is returned, onKeyUp will be invoked when the key is released
		virtual bool onKeyDown(Key key);

		// called when the key is released and the element last handled this key being pressed
		virtual void onKeyUp(Key key);

		// true if 'key' is currently being pressed and the element is in focus
		bool keyDown(Key key) const;

		// write a sequence of text
		void write(const std::string& text, sf::Font& font, sf::Color color = sf::Color(0xFF), unsigned charsize = 15, TextStyle style = TextStyle::Regular);

		// write a sequence of text
		void write(const std::wstring& text, sf::Font& font, sf::Color color = sf::Color(0xFF), unsigned charsize = 15, TextStyle style = TextStyle::Regular);

		// write a line break
		void writeLineBreak(unsigned charsize = 15u);

		// write a tab
		void writeTab(float width = 50.0f);

		// add a new child element
		template<typename ElementType, typename... ArgsT>
		std::shared_ptr<ElementType> add(ArgsT&&... args){
			static_assert(std::is_base_of<Element, ElementType>::value, "ElementType must derive from Element");
			// This may look strange, but the child creates the first shared_ptr to itself
			// (so that shared_from_this is valid in the constructor) and this is how that is dealt with.
			auto rawchild = new ElementType(std::forward<ArgsT>(args)...);
			std::shared_ptr<ElementType> child = rawchild->getThisAs<ElementType>();
			adopt(child);
			return child;
		}

		// adopt an existing child element
		void adopt(std::shared_ptr<Element> child);

		// remove and destroy a child element
		void remove(std::shared_ptr<Element> element);

		// release a child element, possibly to add to another element
		std::shared_ptr<Element> release(std::shared_ptr<Element> element);

		// get all children
		const std::vector<std::shared_ptr<Element>>& getChildren() const;

		// get the parent element
		std::weak_ptr<Element> getParent() const;

		// layout the element before the given sibling
		void layoutBefore(const std::shared_ptr<Element>& sibling);

		// layout the element after the given sibling
		void layoutAfter(const std::shared_ptr<Element>& sibling);

		// render the element in front of its siblings, regardless of layout
		void bringToFront();

		// destroy all children
		void clear();

		// find the element at the given local coordinates, optionally excluding a given element and all its children
		std::shared_ptr<Element> findElementAt(vec2 _pos, std::shared_ptr<Element> exclude = nullptr);

		// render the element
		virtual void render(sf::RenderWindow& renderwindow);

	private:

		std::shared_ptr<Element> shared_this;

		DisplayStyle display_style;
		AlignStyle align_style;

		bool disabled;
		bool visible;
		bool clipping;

		vec2 pos;
		vec2 size;
		vec2 min_size;
		vec2 max_size;
		vec2 old_total_size;

		float layout_index;
		float padding;
		float margin;

		RoundedRectangle display_rect;

		void makeDirty();
		bool isDirty() const;
		void makeClean();

		bool dirty;

		using LayoutIndex = float;

		// returns true if a change is needed
		bool update(float width_avail);

		// position and arrange children. Returns the actual size used
		vec2 Element::arrangeChildren(float width_avail);

		// render the element's children, translating and clipping as needed
		void renderChildren(sf::RenderWindow& renderwindow);

		LayoutIndex getNextLayoutIndex() const;
		void organizeLayoutIndices();

		struct WhiteSpace {

			enum Type {
				None,
				LineBreak,
				Tab
			};

			WhiteSpace(Type _type, LayoutIndex _layout_index, unsigned _charsize = 15u);

			Type type;
			LayoutIndex layout_index;
			unsigned charsize;
		};

		std::weak_ptr<Element> parent;
		std::vector<std::shared_ptr<Element>> children;
		std::vector<WhiteSpace> white_spaces;

		friend struct Context;
		friend void run();
		friend Element& root();
		friend struct LayoutData;
	};

	struct FreeElement : Element {
		FreeElement();
	};

	struct InlineElement : Element {
		InlineElement();
	};

	struct BlockElement : Element {
		BlockElement();
	};

	struct LeftFloatingElement : Element {
		LeftFloatingElement();
	};

	struct RightFloatingElement : Element {
		RightFloatingElement();
	};

} // namespace ui
#ifndef HANDLERLIST_HPP
#define HANDLERLIST_HPP

#include "hookhandler.hpp"

/* The MouseHandlerList does not own its handlers;
 * destroying them is the caller's responsibility.
 */
class MouseHandlerList {

	static size_t const d_capacity = 4;
	size_t d_count;
	MouseHookHandler *d_handlers[d_capacity];

	public:

		MouseHandlerList();
		void add(MouseHookHandler *handler);

		bool onMouseDown(MouseDownEvent const &event);
		bool onMouseUp(MouseUpEvent const &event);
		bool onMouseMove(MouseMoveEvent const &event);
		bool onMouseWheel(MouseWheelEvent const &event);

};

#endif
/*******************************************************************************
 * This file is part of "Patrick's Programming Library Toolkit", ppltk.
 * Web: https://github.com/pfedick/ppltk
 *
 *******************************************************************************
 * Copyright (c) 2024, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/


#include "ppltk.h"


namespace ppltk {

using namespace ppl7;
using namespace ppl7::grafix;


//#define DEBUGEVENTS 1

static WindowManager* wm=NULL;

WindowManager* GetWindowManager()
{
	if (!wm) throw NoWindowManagerException();
	return wm;
}

const WidgetStyle& GetWidgetStyle()
{
	if (!wm) throw NoWindowManagerException();
	return wm->getWidgetStyle();
}

WindowManager::WindowManager()
{
	if (wm != NULL) throw DuplicateWindowManagerException();
	wm=this;
	LastMouseDown=NULL;
	LastMouseEnter=NULL;
	LastMouseFocus=NULL;
	clickCount=0;
	doubleClickIntervall=150;
	KeyboardFocus=NULL;
	grabMouseWidget=NULL;
	GameControllerFocus=NULL;

	ppl7::Resource* resources=GetPPLTKResource();
	ppl7::grafix::Grafix* gfx=ppl7::grafix::GetGrafix();
	if (gfx) {
		gfx->loadFont(resources->getMemory(4), "Default");
		gfx->loadFont(resources->getMemory(5), "Default Mono");
	}
	//ButtonSymbols.load(resources->getMemory(3),15,15,ImageList::DIFFUSE);
	Toolbar.load(resources->getMemory(1), 16, 16, ImageList::ALPHABLT);

	updateButtonSymbols();
}

WindowManager::~WindowManager()
{
	if (wm == this) wm=NULL;
}

const WidgetStyle& WindowManager::getWidgetStyle() const
{
	return Style;
}

void WindowManager::setWidgetStyle(const WidgetStyle& style)
{
	Style=style;
	updateButtonSymbols();
}

void WindowManager::updateButtonSymbols()
{
	Resource* res=GetPPLTKResource();
	ppl7::grafix::ImageList tmp;
	tmp.load(res->getMemory(3), 15, 15, ImageList::DIFFUSE);
	ppl7::grafix::Size s=tmp.size();
	ButtonSymbols.create(s.width, s.height);
	ButtonSymbols.bltDiffuse(tmp, 0, 0, Style.buttonFontColor);
	ButtonSymbols.setIconSize(15, 15);
}

void WindowManager::grabMouse(Widget* w)
{
	grabMouseWidget=w;
}

void WindowManager::releaseMouse(Widget* w)
{
	if (grabMouseWidget == w) grabMouseWidget=NULL;
}

Widget* WindowManager::findMouseWidget(Widget* window, Point& p)
{
	if (!window) return NULL;
	/*
	printf ("Iterate: %s:%s, %d:%d, Point: %d:%d\n", (const char*)window->widgetType(),
			(const char*)window->name(), window->x(), window->y(),
			p.x, p.y);
	*/
	if (grabMouseWidget) {
		Point wp=grabMouseWidget->absolutePosition();
		p-=wp;
		return grabMouseWidget;
	}
	std::list<Widget*>::const_reverse_iterator it;
	Point p2;
	if (window->childs.size() > 0) {
		for (it=window->childs.rbegin();it != window->childs.rend();++it) {
			Widget* w=*it;
			if (p.x >= w->p.x
				&& p.y >= w->p.y
				&& p.x < w->p.x + w->s.width
				&& p.y < w->p.y + w->s.height) {
				if (w->isEnabled()) {
					// Passendes Widget gefunden, Koordinaten des Events auf das Widget umrechnen
					p2.x=p.x - w->p.x - w->myClientOffset.x1;
					p2.y=p.y - w->p.y - w->myClientOffset.y1;
					p=p2;
					return findMouseWidget(w, p);	// Iterieren
				}
			}
			if (w->isModal()) return window;
		}
	}
	//printf ("MATCH\n");
	return window;
}

void WindowManager::unregisterWidget(Widget* widget)
{
	if (LastMouseDown == widget) LastMouseDown=NULL;
	if (LastMouseEnter == widget) LastMouseEnter=NULL;
	if (KeyboardFocus == widget) KeyboardFocus=NULL;
	if (LastMouseFocus == widget) LastMouseFocus=NULL;
	if (GameControllerFocus == widget) GameControllerFocus=NULL;

}

void WindowManager::deferedDeleteWidgets(Widget* widget)
{
	bool match=true;
	while (match) {
		match=false;
		std::list<Widget*>::iterator it;
		for (it=widget->childs.begin();it != widget->childs.end();++it) {
			Widget* child=(*it);
			deferedDeleteWidgets(child);
			if (child->deleteRequested) {
				delete (child);
				//widget->needsRedraw();
				it=widget->childs.end();
				match=true;
			}
		}
	}
}

void WindowManager::dispatchMouseEvent(Window* window, MouseEvent& event)
{
	Widget* w;
	//printf("WindowManager::dispatchMouseEvent\n");
	//deferedDeleteWidgets(window);
	switch (event.type()) {
		case Event::MouseEnter:
			window->mouseState=event;
			event.setWidget(window);
			LastMouseEnter=window;
			window->mouseEnterEvent(&event);
			break;
		case Event::MouseLeave:
			window->mouseState=event;
			if (LastMouseEnter) {
				event.setWidget(LastMouseEnter);
				LastMouseEnter->mouseLeaveEvent(&event);
				LastMouseEnter=NULL;
			}
			event.setWidget(window);
			window->mouseLeaveEvent(&event);
			break;
		case Event::MouseMove:
			window->mouseState=event;
			//printf ("window->mouseState.p.x=%i\n",window->mouseState.p.x);
			w=findMouseWidget(window, event.p);
			if (w) {
				if (w != LastMouseEnter) {
					if (LastMouseEnter) {
						event.setWidget(LastMouseEnter);
						LastMouseEnter->mouseLeaveEvent(&event);
					}
					event.setWidget(w);
					w->mouseEnterEvent(&event);
					LastMouseEnter=w;
				}
				event.setWidget(w);
				w->mouseMoveEvent(&event);
			} else if (LastMouseEnter) {
				event.setWidget(LastMouseEnter);
				LastMouseEnter->mouseLeaveEvent(&event);
				LastMouseEnter=NULL;
			}
			break;
		case Event::MouseDown:
#ifdef DEBUGEVENTS
			ppl7::PrintDebugTime("WindowManager::dispatchMouseEvent, MouseDown\n");
#endif

			window->mouseState=event;
			w=findMouseWidget(window, event.p);
			if (w) {
				clickCount=event.clicks;
				if (w != LastMouseDown) {
					if (LastMouseDown) {
						LastMouseDown->needsRedraw();
					}
				}
				LastMouseDown=w;
				setMouseFocus(w);
				setKeyboardFocus(w);
				w->needsRedraw();

				event.setWidget(w);
				w->mouseDownEvent(&event);

				if (event.clicks == 2) {
					LastMouseDown->mouseDblClickEvent(&event);
				}
			}
			break;

		case Event::MouseUp:
#ifdef DEBUGEVENTS
			ppl7::PrintDebugTime("WindowManager::dispatchMouseEvent, MouseUp, clicks: %d\n", event.clicks);
#endif
			window->mouseState=event;
			w=findMouseWidget(window, event.p);
			if (w) {
				event.setWidget(w);
				w->mouseUpEvent(&event);
				if (LastMouseDown == w) {
					clickCount=event.clicks;
					clickEvent=event;
					if (event.clicks == 1) startClickEvent(static_cast<Window*>(window));
					else if (event.clicks == 2) {
						//ppl7::PrintDebug("Doubleclick\n");
						//LastMouseDown->mouseDblClickEvent(&event);
					}
					//LastMouseDown->mouseClickEvent((MouseEvent*)&event);
				} else {
					clickCount=0;
					LastMouseDown=NULL;
				}
			}
			break;
		case Event::MouseWheel:
			window->mouseState=event;
			w=findMouseWidget(window, event.p);
			if (w) {
				event.setWidget(w);
				w->mouseWheelEvent(&event);
			}
			break;


		default:
#ifdef DEBUGEVENTS
			PrintDebugTime("WindowManager::dispatchMouseEvent(%tu, %s)  ==> Unhandled Event\n", (std::ptrdiff_t)window, event.name().toChar());
#endif
			break;
	}
	deferedDeleteWidgets(window);
}

void WindowManager::setMouseFocus(Widget* w)
{
	if (w != LastMouseFocus) {
		if (LastMouseFocus) {
			//printf("Widget lost Mouse focus: %s:%s\n", (const char*)LastMouseFocus->widgetType(), (const char*)LastMouseFocus->name());
			FocusEvent event(Event::FocusOut, LastMouseFocus, w);
			LastMouseFocus->lostFocusEvent(&event);
			if (!event.accepted()) return;
		}
		//printf("Widget got Mouse focus: %s:%s\n", (const char*)w->widgetType(), (const char*)w->name());
		FocusEvent event(Event::FocusIn, LastMouseFocus, w);
		w->gotFocusEvent(&event);
	}
	LastMouseFocus=w;
}

void WindowManager::setDoubleClickIntervall(int ms)
{
	doubleClickIntervall=ms;
}

int WindowManager::getDoubleClickIntervall() const
{
	return doubleClickIntervall;
}



void WindowManager::dispatchClickEvent(Window* window)
{
	if (!window) return;
	if (!LastMouseDown) return;
//#ifdef DEBUGEVENTS
	//ppl7::PrintDebugTime("WindowManager::dispatchClickEvent, clickCount=%d\n", clickCount);
//#endif
	if (clickCount == 1) LastMouseDown->mouseClickEvent(&clickEvent);
	//else if (clickCount > 1) LastMouseDown->mouseDblClickEvent(&clickEvent);
	clickCount=0;
	//LastMouseDown=NULL;
	deferedDeleteWidgets(window);
}

void WindowManager::setKeyboardFocus(Widget* w)
{
	if (w == KeyboardFocus) return;
	if (KeyboardFocus) {
		FocusEvent e(Event::FocusOut, KeyboardFocus, w);
		KeyboardFocus->lostFocusEvent(&e);
		KeyboardFocus->needsRedraw();
	}
	KeyboardFocus=w;
	FocusEvent e(Event::FocusIn, KeyboardFocus, w);
	KeyboardFocus->gotFocusEvent(&e);
	KeyboardFocus->needsRedraw();
}

Widget* WindowManager::getKeyboardFocus() const
{
	return KeyboardFocus;
}

void WindowManager::setGameControllerFocus(Widget* w)
{
	if (w == GameControllerFocus) return;
	if (GameControllerFocus) {
		FocusEvent e(Event::FocusOut, GameControllerFocus, w);
		GameControllerFocus->lostFocusEvent(&e);
	}
	GameControllerFocus=w;
	FocusEvent e(Event::FocusIn, GameControllerFocus, w);
	GameControllerFocus->gotFocusEvent(&e);
}

Widget* WindowManager::getGameControllerFocus() const
{
	return GameControllerFocus;
}


}	// EOF namespace ppltk

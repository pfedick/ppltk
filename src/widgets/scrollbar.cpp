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




Scrollbar::Scrollbar(int x, int y, int width, int height) // @suppress("Class members should be properly initialized")
	: ppltk::Widget()
{
	this->create(x, y, width, height);
	setClientOffset(0, 0, 0, 0);
	//ppl7::grafix::Grafix *gfx=ppl7::grafix::GetGrafix();
	ppltk::WindowManager* wm=ppltk::GetWindowManager();
	up_button=new ppltk::Button(0, 0, width, 23);
	down_button=new ppltk::Button(0, height - 23, width, 23);
	up_button->setIcon(wm->ButtonSymbols.getDrawable(3));
	down_button->setIcon(wm->ButtonSymbols.getDrawable(4));
	up_button->setEventHandler(this);
	down_button->setEventHandler(this);
	addChild(up_button);
	addChild(down_button);
	size=0;
	pos=0;
	visibleItems=0;
	drag_started=false;
}

Scrollbar::~Scrollbar()
{
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

ppl7::String Scrollbar::widgetType() const
{
	return ppl7::String("Scrollbar");
}

void Scrollbar::setSize(int size)
{
	if (size != this->size && size >= 0) {
		this->size=size;
		if (pos > size) pos=size;
		needsRedraw();
	}
}

void Scrollbar::setPosition(int position)
{
	if (position != pos && position < size && position >= 0) {
		pos=position;
		needsRedraw();
	}
}

void Scrollbar::setVisibleItems(int items)
{
	visibleItems=items;
	needsRedraw();
}

int Scrollbar::position() const
{
	return pos;
}

void Scrollbar::paint(ppl7::grafix::Drawable& draw)
{
	const ppltk::WidgetStyle& style=ppltk::GetWidgetStyle();
	ppl7::grafix::Color light=style.buttonBackgroundColor * 1.8f;
	ppl7::grafix::Color shadow=style.buttonBackgroundColor * 0.4f;
	ppl7::grafix::Color shade1=style.buttonBackgroundColor * 1.05f;
	ppl7::grafix::Color shade2=style.buttonBackgroundColor * 0.85f;
	ppl7::grafix::Drawable indicator=draw.getDrawable(0, 23, draw.width(), draw.height() - 24);
	int w=indicator.width() - 1;
	//int h=indicator.height()-1;
	ppl7::grafix::Rect r1=indicator.rect();

	if (visibleItems > 0 && visibleItems < size) {
		float pxi=(float)indicator.height() / (float)size;
		int visible=pxi*visibleItems;
		if (visible<25) visible=25;
		int unvisible=indicator.height()-visible;
		r1.y1=pos*unvisible/(size-visibleItems);
		r1.y2=r1.y1+visible;
		/*
		ppl7::PrintDebug("height: %d, pxi=%0.3f, size=%d, visibleItems=%d, visible=%d, unvisible=%d, y1=%d, y2=%d\n",
			indicator.height(),pxi,size,visibleItems, visible, unvisible,r1.y1,r1.y2);
		*/
		if (r1.y2 >= indicator.height()-1) r1.y2=indicator.height() - 2;
		
		
	}
	slider_pos=r1;
	slider_pos.y1+=23;
	slider_pos.y2+=23;

	ppl7::grafix::Color scrollarea=style.windowBackgroundColor * 1.2f;
	indicator.cls(scrollarea);
	indicator.colorGradient(r1, shade1, shade2, 1);
	indicator.line(0, r1.y1, w, r1.y1, light);
	indicator.line(0, r1.y1, 0, r1.y2, light);
	indicator.line(0, r1.y2, w, r1.y2, shadow);
	indicator.line(w, r1.y1, w, r1.y2, shadow);

	//ppltk::Widget::paint(draw);
	//draw.fillRect(0,y1,draw.width(),y2,style.frameBorderColorLight);
}

void Scrollbar::mouseDownEvent(ppltk::MouseEvent* event)
{
	//printf("Scrollbar::mouseDownEvent\n");
	if (event->widget() == up_button) {
		if (pos>0) {
			int d=1;
			if (event->button==MouseState::Right) {
				d=visibleItems-1;
				if (d<1)d=1;
			}
			pos-=d;
			if (pos<0) pos=0;
			//ppl7::PrintDebug("pos=%d\n",pos);
			needsRedraw();
			ppltk::Event ev(ppltk::Event::ValueChanged);
			ev.setWidget(this);
			valueChangedEvent(&ev, pos);
		}
		return;

	} else if (event->widget() == down_button) {
		if (pos < size - visibleItems) {
			int d=1;
			if (event->button==MouseState::Right) {
				d=visibleItems-1;
				if (d<1)d=1;
			}
			pos+=d;
			if (pos>=size-visibleItems) pos=size-visibleItems;
			//ppl7::PrintDebug("pos=%d\n",pos);
			needsRedraw();
			ppltk::Event ev(ppltk::Event::ValueChanged);
			ev.setWidget(this);
			valueChangedEvent(&ev, pos);
		}
		return;
	}
	if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
		//printf("HorizontalSlider::mouseDownEvent: %d, %d\n", event->p.x, event->p.y);
		if (event->p.inside(slider_pos)) {
			drag_started=true;
			drag_offset=event->p.y - slider_pos.y1;
			drag_start_pos=event->p;
			ppltk::GetWindowManager()->grabMouse(this);
		} else if (event->p.y < slider_pos.y1 && pos>0) {
			int d=visibleItems-1;
			if (d<1) d=1;
			pos-=d;
			if (pos<0) pos=0;
			needsRedraw();
			ppltk::Event ev(ppltk::Event::ValueChanged);
			ev.setWidget(this);
			valueChangedEvent(&ev, pos);
		} else if (event->p.y > slider_pos.y2 && pos < size - visibleItems) {
			int d=visibleItems-1;
			if (d<1) d=1;
			pos+=d;
			if (pos>=size) pos=size-visibleItems;
			needsRedraw();
			ppltk::Event ev(ppltk::Event::ValueChanged);
			ev.setWidget(this);
			valueChangedEvent(&ev, pos);
		}
	}
}

void Scrollbar::lostFocusEvent(ppltk::FocusEvent* event)
{
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

void Scrollbar::mouseUpEvent(ppltk::MouseEvent* event)
{
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

void Scrollbar::mouseMoveEvent(ppltk::MouseEvent* event)
{
	if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
		if (drag_started) {
			int draw_range=height() - 46;
			int64_t v=(event->p.y- drag_offset) * size / draw_range;
			if (v < 0) v=0;
			if (v >= size - visibleItems) v=size - visibleItems;
			pos=v;
			needsRedraw();
			ppltk::Event ev(ppltk::Event::ValueChanged);
			ev.setWidget(this);
			valueChangedEvent(&ev, pos);

		}
	} else if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

void Scrollbar::mouseWheelEvent(ppltk::MouseEvent* event)
{
	int d=1;
	if (event->keyModifier&KeyEvent::KEYMOD_LEFTALT) { //TODO
		d=visibleItems-1;
		if (d<1) d=1;
	}

	if (event->wheel.y < 0 && pos < size - 1) {
		pos+=d;
		if (pos>=size-visibleItems) pos=size-visibleItems;
		needsRedraw();
		ppltk::Event ev(ppltk::Event::ValueChanged);
		ev.setWidget(this);
		valueChangedEvent(&ev, pos);
	} else if (event->wheel.y > 0 && pos > 0) {
		pos-=d;
		if (pos<0) pos=0;
		needsRedraw();
		ppltk::Event ev(ppltk::Event::ValueChanged);
		ev.setWidget(this);
		valueChangedEvent(&ev, pos);

	}
}

void Scrollbar::resizeEvent(ResizeEvent* event)
{
	//ppl7::PrintDebug("Scrollbar::resizeEvent\n");
	Size s=clientSize();
	down_button->setY(s.height - 23);
}


} //EOF namespace

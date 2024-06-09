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

RadioButton::RadioButton()
	: ppltk::Label()
{
	ischecked=false;
}

RadioButton::RadioButton(int x, int y, int width, int height, const ppl7::String& text, bool checked) // @suppress("Class members should be properly initialized")
	: ppltk::Label(x, y, width, height, text)
{
	ischecked=checked;
}

RadioButton::~RadioButton()
{

}

ppl7::String RadioButton::widgetType() const
{
	return ppl7::String("RadioButton");
}

bool RadioButton::checked() const
{
	return ischecked;
}

void RadioButton::setChecked(bool checked)
{
	bool laststate=ischecked;
	ischecked=checked;
	needsRedraw();
	parentMustRedraw();
	// uncheck all other RadioButtons in Parent-Widget
	if (checked == true && this->getParent()) {
		Widget* parent=this->getParent();
		std::list<Widget*>::iterator it;
		for (it=parent->childsBegin(); it != parent->childsEnd();++it) {
			if (typeid(**it) == typeid(RadioButton) && *it != this) {
				((RadioButton*)(*it))->setChecked(false);
			}
		}
	}
	ppltk::Event ev(ppltk::Event::Toggled);
	ev.setWidget(this);
	if (checked != laststate) {
		toggledEvent(&ev, checked);
	}
}


void RadioButton::paint(ppl7::grafix::Drawable& draw)
{
	const ppltk::WidgetStyle& style=ppltk::GetWidgetStyle();
	ppl7::grafix::Drawable d=draw.getDrawable(16, 0, draw.width() - 16, draw.height());
	Label::paint(d);
	int y1=draw.height() / 2;
	draw.circle(9, y1, 7, style.frameBorderColorLight);
	draw.circle(9, y1, 6, style.frameBorderColorLight);
	if (ischecked) draw.floodFill(9, y1, this->color(), style.frameBorderColorLight);
}

void RadioButton::mouseDownEvent(ppltk::MouseEvent* event)
{
	setChecked(true);
}



} //EOF namespace

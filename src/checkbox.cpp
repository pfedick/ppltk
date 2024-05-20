/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *
 *******************************************************************************
 * Copyright (c) 2022, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "ppltk.h"


namespace ppltk {

using namespace ppl7;
using namespace ppl7::grafix;

CheckBox::CheckBox()
	: ppltk::Label()
{
	ischecked=false;
}

CheckBox::CheckBox(int x, int y, int width, int height, const ppl7::String& text, bool checked) // @suppress("Class members should be properly initialized")
	: ppltk::Label(x, y, width, height, text)
{
	ischecked=checked;
}

CheckBox::~CheckBox()
{

}

ppl7::String CheckBox::widgetType() const
{
	return ppl7::String("CheckBox");
}

bool CheckBox::checked() const
{
	return ischecked;
}

void CheckBox::setChecked(bool checked)
{
	ischecked=checked;
	needsRedraw();
	parentMustRedraw();
}


void CheckBox::paint(ppl7::grafix::Drawable& draw)
{
	int s=draw.height() * 3 / 5;
	int sh=s / 2;

	const ppltk::WidgetStyle& style=ppltk::GetWidgetStyle();
	ppl7::grafix::Drawable d=draw.getDrawable(s + (s / 3), 0, draw.width() - (s + s / 3), draw.height());
	Label::paint(d);
	int y1=draw.height() / 2 - sh;
	int y2=draw.height() / 2 + sh;
	draw.drawRect(2, y1, 2 + s, y2, style.frameBorderColorLight);
	draw.drawRect(3, y1 + 1, 1 + s, y2 - 1, style.frameBorderColorLight);
	if (ischecked) draw.fillRect(5, y1 + 3, 2 + s - 3, y2 - 3, this->color());
}

void CheckBox::mouseDownEvent(ppltk::MouseEvent* event)
{
	ischecked=!ischecked;
	needsRedraw();
	ppltk::Event ev(ppltk::Event::Toggled);
	ev.setWidget(this);
	toggledEvent(&ev, ischecked);
}


} //EOF namespace

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


Label::Label(const String& text, BorderStyle style)
	:Frame(style)
{
	const WidgetStyle& widgetstyle = GetWidgetStyle();
	myColor = widgetstyle.labelFontColor;
	myFont = widgetstyle.labelFont;
	setTransparent(true);
	myText = text;

}

Label::Label(int x, int y, int width, int height, const String& text, BorderStyle style)
	:Frame(x, y, width, height)
{
	const WidgetStyle& wstyle = GetWidgetStyle();
	setBorderStyle(style);
	myColor = wstyle.labelFontColor;
	myFont = wstyle.labelFont;
	setTransparent(true);
	myText = text;
}

Label::~Label()
{

}

const String& Label::text() const
{
	return myText;
}

void Label::setText(const String& text)
{
	if (text == myText) return;
	myText = text;
	needsRedraw();
	geometryChanged();
}

const Color& Label::color() const
{
	return myColor;
}

void Label::setColor(const Color& c)
{
	if (c == myColor) return;
	myColor = c;
	needsRedraw();
}

const Drawable& Label::icon() const
{
	return myIcon;
}

void Label::setIcon(const Drawable& icon)
{
	myIcon = icon;
	needsRedraw();
	geometryChanged();
}

const Font& Label::font() const
{
	return myFont;
}

void Label::setFont(const Font& font)
{
	myFont = font;
	needsRedraw();
	geometryChanged();
}


/*
Size Label::contentSize() const
{
	Size s;
	s=myFont.measure(myText);
	if (myIcon.isEmpty() == false) {
		s.width+=4 + myIcon.width();
		int h=2 + myIcon.height();
		if (s.height < h) s.height=h;
	}
	return s;
}
*/

ppl7::grafix::Size Label::sizeHint() const
{
	ppl7::grafix::Size s(6, 6);
	ppl7::grafix::Size is = myIcon.size();
	ppl7::grafix::Size ts = myFont.measure(myText);
	s.width += is.width;
	s.width += ts.width;
	int h = 0;
	if (is.width > 0)
	{
		s.width += 8 + is.width;
		h = 2 + is.height;
	}
	if (ts.width > 0)
	{
		s.width += 4 + ts.width;
		if (h < 2 + ts.height)
			h = 2 + ts.height;
	}
	s.height += h;

	ppl7::grafix::Size min = minSize();
	if (s.width < min.width) s.width = min.width;
	if (s.height < min.height) s.height = min.height;

	ppl7::grafix::Size max = maxSize();
	if (max.width > 0 && s.width > max.width) s.width = max.width;
	if (max.height > 0 && s.height > max.height) s.height = max.height;
	return s;
}

ppl7::grafix::Size Label::minimumSizeHint() const
{
	return sizeHint();
}



String Label::widgetType() const
{
	return "Label";
}


void Label::paint(Drawable& draw)
{
	Frame::paint(draw);
	Drawable d = clientDrawable(draw);
	//printf ("Text: %s, width: %i, height: %i\n",(const char*)myText, d.width(), d.height());
	int x = 0;
	if (myIcon.isEmpty() == false) {
		if (myText.isEmpty()) {
			d.bltAlpha(myIcon, (d.width()) / 2 - myIcon.width() / 2, (d.height()) / 2 - myIcon.height() / 2);
			return;

		}
		else {
			d.bltAlpha(myIcon, x, (d.height()) / 2 - myIcon.height() / 2);
			x += 4 + myIcon.width();
		}
	}
	myFont.setColor(myColor);
	myFont.setOrientation(Font::TOP);
	Size s = myFont.measure(myText);
	d.print(myFont, x, (d.height() - s.height) >> 1, myText);
}


}	// EOF namespace ppltk

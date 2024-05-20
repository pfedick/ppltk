/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
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

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "ppltk.h"


namespace ppltk {

using namespace ppl7;
using namespace ppl7::grafix;


TextEdit::TextEdit()
	:Frame()
{
	const WidgetStyle& style=GetWidgetStyle();
	setBorderStyle(Inset);
	//myColor=style.inputFontColor;
	//myBackgroundColor=style.inputBackgroundColor;
	//myFont=style.inputFont;
	setBackgroundColor(style.inputBackgroundColor);
	setTransparent(false);

}

TextEdit::TextEdit(int x, int y, int width, int height)
	:Frame(x, y, width, height)
{
	const WidgetStyle& style=GetWidgetStyle();
	setBorderStyle(Inset);
	//myColor=style.inputFontColor;
	//myFont=style.inputFont;
	setBackgroundColor(style.inputBackgroundColor);
	setTransparent(false);
}

String TextEdit::widgetType() const
{
	return String("TextEdit");
}

TextEdit::~TextEdit()
{
	/*
	if (timerId) GetWindowManager()->removeTimer(timerId);
	timerId=0;
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
	*/
}

const ppl7::WideString& TextEdit::text() const
{
	return myText;
}

void TextEdit::setText(const String& text)
{
	WideString new_text=text;
	setText(new_text);
}

void TextEdit::setText(const WideString& text)
{
	if (text == myText) return;
	myText=text;
	needsRedraw();
	geometryChanged();
}

const ppl7::grafix::Color& TextEdit::color() const
{
	return myColor;
}

void TextEdit::setColor(const Color& c)
{
	myColor=c;
	needsRedraw();
}


const Font& TextEdit::font() const
{
	return myFont;
}

void TextEdit::setFont(const Font& font)
{
	myFont=font;
	needsRedraw();
	geometryChanged();
}




void TextEdit::paint(Drawable& draw)
{
	//const WidgetStyle& style=GetWidgetStyle();
	Frame::paint(draw);
	Drawable d=clientDrawable(draw);

}


}	// EOF namespace ppltk

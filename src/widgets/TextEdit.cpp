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
	validator=NULL;
	const WidgetStyle& style=GetWidgetStyle();
	setBorderStyle(Inset);
	myColor=style.inputFontColor;
	myBackgroundColor=style.inputBackgroundColor;
	myFont=style.inputFont;
	setBackgroundColor(style.inputBackgroundColor);
	setTransparent(false);
	cursorpos=0;
	startpos=0;
	cursorx=0;
	cursory=0;
	cursorwidth=2;
	blinker=false;
	overwrite=false;
	timerId=0;
	drag_started=false;

}

TextEdit::TextEdit(int x, int y, int width, int height, const String& text)
	:Frame(x, y, width, height)
{
	validator=NULL;
	const WidgetStyle& style=GetWidgetStyle();
	setBorderStyle(Inset);
	myColor=style.inputFontColor;
	myFont=style.inputFont;
	setBackgroundColor(style.inputBackgroundColor);
	setTransparent(false);
	myText=text;
	if (validator) {
		if (validator->validateText(text) == true) validatedText=text;
	}
	cursorpos=0;
	startpos=0;
	cursorx=0;
	cursorwidth=2;
	blinker=false;
	overwrite=false;
	timerId=0;
	drag_started=false;
}

String TextEdit::widgetType() const
{
	return String("TextEdit");
}

TextEdit::~TextEdit()
{
	if (timerId) GetWindowManager()->removeTimer(timerId);
	timerId=0;
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

const ppl7::WideString& TextEdit::text() const
{
	if (validator) return validatedText;
	else return myText;
}

void TextEdit::validateAndSendEvent(const WideString& text)
{
	if (validator) {
		if (validator->validateText(text) == false) return;
	}
	validatedText=text;
	Event ev(Event::Type::TextChanged);
	ev.setWidget(this);
	textChangedEvent(&ev, validatedText);
}

void TextEdit::setText(const String& text)
{
	WideString new_text=text;
	if (new_text == myText) return;
	if (validator != NULL && validator->validateText(new_text) == false) return;
	myText=new_text;
	startpos=0;
	if (cursorpos > myText.size()) cursorpos=myText.size();
	calcCursorPosition();
	selection.clear();
	needsRedraw();
	geometryChanged();
	validateAndSendEvent(myText);
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

const Color& TextEdit::backgroundColor() const
{
	return myBackgroundColor;
}
void TextEdit::setBackgroundColor(const Color& c)
{
	myBackgroundColor=c;
}


ppl7::grafix::Size TextEdit::sizeHint() const
{
	// TODO
	return ppl7::grafix::Size::invalid();
}

ppl7::grafix::Size TextEdit::minimumSizeHint() const
{
	// TODO
	return ppl7::grafix::Size::invalid();
}


void TextEdit::paint(Drawable& draw)
{
	const WidgetStyle& style=GetWidgetStyle();
	if (myText != validatedText && validator != NULL) Frame::setBackgroundColor(style.inputInvalidBackgroundColor);
	else Frame::setBackgroundColor(myBackgroundColor);
	Frame::paint(draw);
	Drawable d=clientDrawable(draw);
	if (selection.x1 != selection.x2) d.fillRect(selection.x1, 0, selection.x2, d.height(), style.inputSelectedBackgroundColor);
	//printf ("Text: %s, width: %i, height: %i\n",(const char*)myText, d.width(), d.height());
	int x=0;
	myFont.setColor(myColor);
	myFont.setOrientation(Font::TOP);
	Size s=myFont.measure(myText);
	d.print(myFont, x, (d.height() - s.height) >> 1, myText);
	//d.invert(Rect(cursorx,0,cursorx+cursorwidth,d.height()),myColor,backgroundColor());
	if (blinker) d.fillRect(cursorx, 0, cursorx + cursorwidth, d.height(), myColor);
}

void TextEdit::mouseDownEvent(MouseEvent* event)
{
	//printf ("TextEdit::mouseDownEvent\n");
	GetWindowManager()->setKeyboardFocus(this);
	cursorpos=calcPosition(event->p.x);
	selection.clear();
	blinker=true;
	if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
		drag_started=true;
		drag_start_position=cursorpos;
		selection.begin((int)cursorpos);
		ppltk::GetWindowManager()->grabMouse(this);
	} else if (event->buttonMask & ppltk::MouseEvent::MouseButton::Middle) {
		WideString clipboard=String(GetWindowManager()->getClipboardText());
		WideString new_text=myText.left(cursorpos) + clipboard + myText.mid(cursorpos);
		myText=new_text;
		cursorpos+=clipboard.size();

	}
	calcCursorPosition();
	needsRedraw();

}


void TextEdit::mouseMoveEvent(ppltk::MouseEvent* event)
{
	if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
		cursorpos=calcPosition(event->p.x);
		if ((int)cursorpos < drag_start_position) {
			selection.start=cursorpos;
			selection.end=drag_start_position;
		} else if ((int)cursorpos > drag_start_position) {
			selection.start=drag_start_position;
			selection.end=cursorpos;
		}
		calcCursorPosition();
		calcSelectionPosition();
		needsRedraw();
	} else if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

void TextEdit::mouseUpEvent(ppltk::MouseEvent* event)
{
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

void TextEdit::gotFocusEvent(FocusEvent* event)
{
	//printf("TextEdit::gotFocusEvent\n");
	blinker=true;
	if (!timerId) timerId=GetWindowManager()->startTimer(this, 500);
	needsRedraw();
}

void TextEdit::lostFocusEvent(FocusEvent* event)
{
	//printf ("TextEdit::lostFocusEvent\n");
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
	blinker=false;
	if (timerId) GetWindowManager()->removeTimer(timerId);
	timerId=0;
	needsRedraw();
}

void TextEdit::textInputEvent(TextInputEvent* event)
{
	if (validator) {
		if (validator->validateInput(event->text) == false) return;
	}

	WideString left, right;
	deleteSelection();
	left=myText.left(cursorpos);
	right=myText.mid(cursorpos);
	left+=event->text + right;
	myText=left;
	cursorpos++;
	calcCursorPosition();
	selection.clear();
	validateAndSendEvent(myText);
	if (validator) {
		if (validator->validateText(myText) == false) return;
	}
}

void TextEdit::deleteSelection()
{
	if (selection.exists()) {
		WideString new_text=myText.left(selection.start) + myText.mid(selection.end);
		myText=new_text;
		cursorpos=selection.start;
		calcCursorPosition();
		selection.clear();
	}
}

void TextEdit::keyDownEvent(KeyEvent* event)
{
	//printf("TextEdit::keyDownEvent(keycode=%i, repeat=%i, modifier: %i)\n", event->key, event->repeat, event->modifier);
	if (isEnabled()) {
		int key_modifier=event->modifier & KeyEvent::KEYMOD_MODIFIER;
		if (key_modifier == KeyEvent::KEYMOD_NONE) {
			if (event->key == KeyEvent::KEY_LEFT && cursorpos > 0) {
				cursorpos--;
				calcCursorPosition();
			} else if (event->key == KeyEvent::KEY_RIGHT && cursorpos < myText.size()) {
				cursorpos++;
				calcCursorPosition();
			} else if (event->key == KeyEvent::KEY_HOME && cursorpos > 0) {
				cursorpos=0;
				calcCursorPosition();
			} else if (event->key == KeyEvent::KEY_END && cursorpos < myText.size()) {
				cursorpos=myText.size();
				calcCursorPosition();
			} else if (event->key == KeyEvent::KEY_BACKSPACE && cursorpos > 0) {
				if (selection.exists()) {
					deleteSelection();
				} else {
					WideString new_text=myText.left(cursorpos - 1) + myText.mid(cursorpos);
					myText=new_text;
					cursorpos--;
				}
				calcCursorPosition();
				validateAndSendEvent(myText);
			} else if (event->key == KeyEvent::KEY_DELETE) {
				if (selection.exists()) {
					deleteSelection();
				} else {
					WideString new_text=myText.left(cursorpos) + myText.mid(cursorpos + 1);
					myText=new_text;
				}
				calcCursorPosition();
				validateAndSendEvent(myText);
			}
			selection.clear();
			calcSelectionPosition();

		} else if (key_modifier == KeyEvent::KEYMOD_LEFTCTRL) {
			//printf("key=%d\n", event->key);
			if (event->key == KeyEvent::KEY_c) {
				ppl7::String text;
				if (selection.exists()) {
					ppl7::String t=myText.mid(selection.start, selection.end - selection.start);
					text=String(t);
					//printf("selection=%s\n", (const char*)text);
					GetWindowManager()->setClipboardText(text);
				}
			} else if (event->key == KeyEvent::KEY_v) {
				deleteSelection();
				WideString clipboard=String(GetWindowManager()->getClipboardText());
				WideString new_text=myText.left(cursorpos) + clipboard + myText.mid(cursorpos);
				myText=new_text;
				cursorpos+=clipboard.size();
				calcCursorPosition();

			}

		} else if (key_modifier == KeyEvent::KEYMOD_LEFTSHIFT || key_modifier == KeyEvent::KEYMOD_RIGHTSHIFT) {
			if (event->key == KeyEvent::KEY_LEFT && cursorpos > 0) {
				if (!selection.exists()) selection.begin(cursorpos);
				cursorpos--;
				if (selection.start > (int)cursorpos) selection.start=cursorpos;
				else selection.end=cursorpos;
				calcCursorPosition();
				calcSelectionPosition();
			} else if (event->key == KeyEvent::KEY_RIGHT && cursorpos < myText.size()) {
				if (!selection.exists()) selection.begin(cursorpos);
				cursorpos++;
				if (selection.end < (int)cursorpos) selection.end=cursorpos;
				else selection.start=cursorpos;
				calcCursorPosition();
				calcSelectionPosition();
			} else if (event->key == KeyEvent::KEY_HOME && cursorpos > 0) {
				if (!selection.exists()) selection.begin(cursorpos);
				if ((int)cursorpos == selection.end) selection.end=selection.start;
				cursorpos=0;
				selection.start=cursorpos;
				calcCursorPosition();
				calcSelectionPosition();
			} else if (event->key == KeyEvent::KEY_END && cursorpos < myText.size()) {
				if (!selection.exists()) selection.begin(cursorpos);
				if ((int)cursorpos == selection.start) selection.start=selection.end;
				cursorpos=myText.size();
				selection.end=cursorpos;
				calcCursorPosition();
				calcSelectionPosition();
			}
		}

	}
	Frame::keyDownEvent(event);
}

void TextEdit::keyUpEvent(KeyEvent* event)
{
	//printf ("TextEdit::keyUpEvent(keycode=%i, repeat=%i, modifier: %i)\n",event->key, event->repeat, event->modifier);
	Frame::keyUpEvent(event);
}

void TextEdit::timerEvent(Event* event)
{
	blinker=!blinker;
	needsRedraw();
	//if (GetWindowManager()->getKeyboardFocus()==this) GetWindowManager()->startTimer(this,500);
	//else blinker=false;

}

void TextEdit::mouseDblClickEvent(MouseEvent* event)
{
	selection.clear();
	if (myText.notEmpty()) {
		selection.start=0;
		selection.end=(int)myText.size();
		calcSelectionPosition();
		needsRedraw();
	}
}


void TextEdit::calcCursorPosition()
{
	WideString text=myText, left, right;
	Size s1;
	if ((ssize_t)cursorpos < 0) cursorpos=0;
	if (cursorpos > text.size()) cursorpos=text.size();
	if (cursorpos == 0) {
		cursorx=0;
		startpos=0;
	} else {
		left=text.left(cursorpos);
		right=text.mid(cursorpos);
		s1=myFont.measure(left);
		cursorx=s1.width;
	}
	needsRedraw();
}

int TextEdit::calcPosition(int x)
{
	WideString text;
	size_t c=0;
	Size s1;
	while (c < myText.size()) {
		text=myText.left(c + 1);
		s1=myFont.measure(text);
		if (x < s1.width) break;
		c++;
	}

	return c;
}

int TextEdit::getDrawStartPositionOfChar(size_t pos)
{
	WideString text=myText.left(pos);
	Size s1=myFont.measure(text);
	return s1.width;
}

void TextEdit::calcSelectionPosition()
{
	if (selection.exists()) {
		selection.x1=getDrawStartPositionOfChar(selection.start);
		selection.x2=getDrawStartPositionOfChar(selection.end);
	} else {
		selection.x1=0;
		selection.x2=0;
	}
}


TextEdit::Selection::Selection()
{
	x1=x2=0;
	start=-1;
	end=-1;
}

bool TextEdit::Selection::exists() const
{
	if (start > -1 && end > -1) return true;
	return false;
}

void TextEdit::Selection::clear()
{
	x1=x2=0;
	start=-1;
	end=-1;
}

void TextEdit::Selection::begin(int position)
{
	start=end=position;
}


}	// EOF namespace ppltk

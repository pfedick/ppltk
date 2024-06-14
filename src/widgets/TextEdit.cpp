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
	myFont=style.inputFont;
	setBackgroundColor(style.inputBackgroundColor);
	setTransparent(false);
	cursorpos=0;
	cursorx=0;
	cursory=0;
	line_height=0;
	total_lines=0;
	current_line=0;
	cache_line_width=0;
	visible_lines=0;
	cursorwidth=2;
	blinker=false;
	overwrite=false;
	timerId=0;
	drag_started=false;
	cache_is_valid=false;
	vertical_scrollbar=NULL;
	first_cursor_up_down_x=-1;
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
	cursorx=0;
	total_lines=0;
	current_line=0;
	visible_lines=0;
	line_height=0;
	cache_line_width=0;
	cursorwidth=2;
	blinker=false;
	overwrite=false;
	timerId=0;
	drag_started=false;
	cache_is_valid=false;
	vertical_scrollbar=NULL;
	first_cursor_up_down_x=-1;
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
	if (vertical_scrollbar) {
		this->removeChild(vertical_scrollbar);
		delete vertical_scrollbar;
	}
}

void TextEdit::enableScrollbar(bool enable)
{
	if (enable == true && vertical_scrollbar == NULL) {
		Size s=clientSize();
		vertical_scrollbar=new Scrollbar(s.width - 19, 0, 19, s.height);
		vertical_scrollbar->setEventHandler(this);
		vertical_scrollbar->setSize(total_lines);
		vertical_scrollbar->setVisibleItems(visible_lines);
		vertical_scrollbar->setPosition(current_line);
		addChild(vertical_scrollbar);

	} else if (enable == false && vertical_scrollbar != NULL) {
		this->removeChild(vertical_scrollbar);
		delete vertical_scrollbar;
		vertical_scrollbar=NULL;
	}
}

void TextEdit::updateScrollbar()
{
	if (!vertical_scrollbar) return;
	vertical_scrollbar->setSize(total_lines);
	vertical_scrollbar->setVisibleItems(visible_lines);
}

void TextEdit::moveScrollbarToCursor()
{
	if (!vertical_scrollbar) return;
	int sline=vertical_scrollbar->position();
	if (current_line < sline)  vertical_scrollbar->setPosition(current_line);
	else if (current_line > sline + visible_lines) vertical_scrollbar->setPosition(current_line - visible_lines);
}

void TextEdit::invalidateCache()
{
	cache_is_valid=false;
	position_cache.clear();
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
	if (cursorpos > myText.size()) cursorpos=myText.size();
	calcCursorPosition();
	selection.clear();
	needsRedraw();
	invalidateCache();
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
	invalidateCache();
	needsRedraw();
	geometryChanged();
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


static ppl7::WideString GetWord(const ppl7::WideString& string, size_t& p)
{
	ppl7::WideString word;
	while (p < string.size()) {
		wchar_t c=string[p];
		switch (c) {
			case '\n':
			case ' ':
				if (word.size() > 0) return word;
				word.append(c);
				p++;
				return word;
			case '-':
				word.append(c);
				p++;
				return word;
			default:
				word.append(c);
				p++;
		}
	}
	return word;
}

void TextEdit::addToCache(const ppl7::WideString& word, int x, int y, int line)
{
	size_t p=0;
	ppl7::WideString letter;
	CacheItem item;
	while (p < word.size()) {
		letter.set(word[p]);
		item.letter=word[p];
		item.size=myFont.measure(letter);
		item.line=line;
		item.p.x=x;
		item.p.y=y;
		position_cache.insert(std::pair<size_t, TextEdit::CacheItem>(position_cache.size(), item));
		x+=item.size.width;
		p++;
	}
}

void TextEdit::rebuildCache(int width)
{
	position_cache.clear();
	int line=0;
	int x=0;
	int y=0;
	ppl7::grafix::Size s=myFont.measure(L" ");
	line_height=s.height;
	//ppl7::PrintDebug("TextEdit::rebuildCache\n");
	size_t p=0;
	while (p < myText.size()) {
		ppl7::WideString word=GetWord(myText, p);
		if (word.size()) {
			if (word[0] == '\n') {
				addToCache(word, x, y, line);
				x=0;
				y+=line_height;
				line++;
			} else if (word[0] == ' ') {
				s=myFont.measure(word);
				addToCache(word, x, y, line);
				x+=s.width;
			} else {
				s=myFont.measure(word);
				if (x + s.width >= width) {
					x=0;
					y+=line_height;
					line++;
				}
				addToCache(word, x, y, line);
				x+=s.width;
			}
		}
	}
	cache_line_width=width;
	total_lines=line;
	cache_is_valid=true;
	calcCursorPosition();
}


void TextEdit::paintSelection(Drawable& draw)
{
	int start_y=0;
	if (vertical_scrollbar) start_y=vertical_scrollbar->position() * line_height;
	//int end_line=start_line + visible_lines + 1;


	const WidgetStyle& style=GetWidgetStyle();
	for (int i=selection.start;i <= selection.end;i++) {
		std::map<size_t, CacheItem>::const_iterator it=position_cache.find(i);
		if (it != position_cache.end()) {
			draw.fillRect(it->second.p.x, it->second.p.y - start_y, it->second.p.x + it->second.size.width, it->second.p.y - start_y + it->second.size.height,
				style.inputSelectedBackgroundColor);
		}
	}

}

void TextEdit::paint(Drawable& draw)
{
	const WidgetStyle& style=GetWidgetStyle();
	ppl7::grafix::Color saveBackgroundColor=backgroundColor();
	if (myText != validatedText && validator != NULL) Frame::setBackgroundColor(style.inputInvalidBackgroundColor);
	Frame::paint(draw);
	Frame::setBackgroundColor(saveBackgroundColor);

	Drawable d=clientDrawable(draw);
	int w=d.width();
	if (vertical_scrollbar) w-=vertical_scrollbar->width();
	if (w != cache_line_width) invalidateCache();

	if (!cache_is_valid) rebuildCache(w);

	visible_lines=d.height() / line_height;
	if (visible_lines * line_height < d.height()) visible_lines--;
	//ppl7::PrintDebug("total-lines: %d, visible-lines: %d\n", total_lines, visible_lines);

	if (visible_lines < total_lines) {
		if (!vertical_scrollbar) {
			enableScrollbar(true);
			w=d.width() - vertical_scrollbar->width();
			rebuildCache(w);
		}
		updateScrollbar();

	} else enableScrollbar(false);

	if (selection.exists()) paintSelection(d);
	myFont.setColor(myColor);
	myFont.setOrientation(Font::TOP);
	WideString letter;
	std::map<size_t, CacheItem>::const_iterator it;
	int start_line=0;
	if (vertical_scrollbar) start_line=vertical_scrollbar->position();
	int end_line=start_line + visible_lines + 1;
	int start_y=start_line * line_height;
	for (it=position_cache.begin();it != position_cache.end();++it) {
		if (it->second.letter != '\n' && it->second.line >= start_line && it->second.line < end_line) {
			letter.set(it->second.letter);
			d.print(myFont, it->second.p.x, it->second.p.y - start_y, letter);
		}
	}
	//if (vertical_scrollbar) ppl7::PrintDebug("line: %d, scrollbar-pos: %d\n", current_line, vertical_scrollbar->position());
	if (blinker) d.fillRect(cursorx, cursory - start_line * line_height, cursorx + cursorwidth, cursory + line_height - start_line * line_height, myColor);
}

void TextEdit::mouseDownEvent(MouseEvent* event)
{
	if (vertical_scrollbar != NULL && event->p.x >= vertical_scrollbar->x()) return;
	first_cursor_up_down_x=-1;
	//printf ("TextEdit::mouseDownEvent\n");
	GetWindowManager()->setKeyboardFocus(this);
	ppl7::grafix::Point p=event->p;
	if (vertical_scrollbar) p.y+=vertical_scrollbar->position() * line_height;

	cursorpos=calcPosition(p);
	selection.clear();
	blinker=true;
	if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
		drag_started=true;
		drag_start_position=cursorpos;
		//selection.begin((int)cursorpos);
		ppltk::GetWindowManager()->grabMouse(this);
	} else if (event->buttonMask & ppltk::MouseEvent::MouseButton::Middle) {
		WideString clipboard=String(GetWindowManager()->getClipboardText());
		WideString new_text=myText.left(cursorpos) + clipboard + myText.mid(cursorpos);
		myText=new_text;
		cursorpos+=clipboard.size();
		invalidateCache();

	}
	calcCursorPosition();
	moveScrollbarToCursor();
	needsRedraw();
}


void TextEdit::mouseMoveEvent(ppltk::MouseEvent* event)
{
	if (vertical_scrollbar != NULL && event->p.x >= vertical_scrollbar->x()) return;
	first_cursor_up_down_x=-1;
	if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
		ppl7::grafix::Point p=event->p;
		if (vertical_scrollbar) p.y+=vertical_scrollbar->position() * line_height;

		cursorpos=calcPosition(p);
		if ((int)cursorpos < drag_start_position) {
			selection.start=cursorpos;
			selection.end=drag_start_position - 1;
		} else if ((int)cursorpos > drag_start_position) {
			selection.start=drag_start_position;
			selection.end=cursorpos - 1;
		}
		calcCursorPosition();
		moveScrollbarToCursor();
		needsRedraw();
	} else if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

void TextEdit::mouseUpEvent(ppltk::MouseEvent* event)
{
	if (vertical_scrollbar != NULL && event->p.x >= vertical_scrollbar->x()) return;
	first_cursor_up_down_x=-1;
	if (drag_started) {
		drag_started=false;
		ppltk::GetWindowManager()->releaseMouse(this);
	}
}

void TextEdit::mouseWheelEvent(ppltk::MouseEvent* event)
{
	if (vertical_scrollbar) vertical_scrollbar->mouseWheelEvent(event);
}


void TextEdit::gotFocusEvent(FocusEvent* event)
{
	//ppl7::PrintDebug("TextEdit::gotFocusEvent\n");
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
	//ppl7::PrintDebug("TextEdit::textInputEvent\n");
	if (validator) {
		if (validator->validateInput(event->text) == false) return;
	}

	WideString left, right;
	deleteSelection();
	left=myText.left(cursorpos);
	right=myText.mid(cursorpos);
	myText=left + event->text + right;
	invalidateCache();
	cursorpos++;
	calcCursorPosition();
	moveScrollbarToCursor();
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
		moveScrollbarToCursor();
		selection.clear();
	}
}


void TextEdit::keyDownEvent(KeyEvent* event)
{
	//printf("TextEdit::keyDownEvent(keycode=%i, repeat=%i, modifier: %i)\n", event->key, event->repeat, event->modifier);
	if (isEnabled()) {
		blinker=true;
		int last_cursorpos=cursorpos;
		int key_modifier=event->modifier & KeyEvent::KEYMOD_MODIFIER;
		bool selectmode=(key_modifier & (KeyEvent::KEYMOD_LEFTSHIFT | KeyEvent::KEYMOD_RIGHTSHIFT));
		//ppl7::PrintDebug("%d\n",selectmode);

		if (event->key != KeyEvent::KEY_UP && event->key != KeyEvent::KEY_DOWN) first_cursor_up_down_x=-1;
		if (event->key == KeyEvent::KEY_ENTER || event->key == KeyEvent::KEY_RETURN) {
			TextInputEvent ev;
			ev.text.set(L"\n");
			textInputEvent(&ev);
		} else if (event->key == KeyEvent::KEY_LEFT && cursorpos > 0) {
			cursorpos--;
			if (selectmode) selection.update_left(cursorpos);
			calcCursorPosition();
		} else if (event->key == KeyEvent::KEY_RIGHT && cursorpos < myText.size()) {
			cursorpos++;
			if (selectmode) selection.update_right(cursorpos);
			calcCursorPosition();
		} else if (event->key == KeyEvent::KEY_UP) {
			ppl7::grafix::Point p=getDrawStartPositionOfChar(cursorpos);
			if (cursorpos >= myText.size()) p=getDrawStartPositionOfChar(cursorpos - 1);
			if (first_cursor_up_down_x < 0)first_cursor_up_down_x=p.x;
			p.y-=line_height;
			if (p.y < 0) p.y=0;
			//if (selectmode && !selection.exists()) selection.begin(cursorpos);
			cursorpos=calcPosition(ppl7::grafix::Point(first_cursor_up_down_x, p.y));
			if (selectmode) selection.go(last_cursorpos - 1, cursorpos);
			calcCursorPosition();
		} else if (event->key == KeyEvent::KEY_DOWN) {
			ppl7::grafix::Point p=getDrawStartPositionOfChar(cursorpos);
			if (cursorpos < myText.size()) {
				if (first_cursor_up_down_x < 0)first_cursor_up_down_x=p.x;
				p.y+=line_height;
				cursorpos=calcPosition(ppl7::grafix::Point(first_cursor_up_down_x, p.y));
				if (cursorpos > myText.size()) cursorpos=myText.size();
				if (selectmode) selection.go(last_cursorpos, cursorpos);
				calcCursorPosition();
			}

		} else if (event->key == KeyEvent::KEY_HOME && !(key_modifier & KeyEvent::KEYMOD_LEFTCTRL)) {
			ppl7::grafix::Point p=getDrawStartPositionOfChar(cursorpos);
			p.x=0;
			//if (selectmode && selection.exists()) selection.begin(cursorpos);
			cursorpos=calcPosition(p);
			if (selectmode) selection.go(last_cursorpos - 1, cursorpos);
			calcCursorPosition();
		} else if (event->key == KeyEvent::KEY_END && !(key_modifier & KeyEvent::KEYMOD_LEFTCTRL)) {
			ppl7::grafix::Point p=getDrawStartPositionOfChar(cursorpos);
			p.x=cache_line_width + 100;
			cursorpos=calcPosition(p);
			if (selectmode) selection.go(last_cursorpos, cursorpos);
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
			invalidateCache();
			validateAndSendEvent(myText);
			blinker=true;
		} else if (event->key == KeyEvent::KEY_DELETE) {
			if (selection.exists()) {
				deleteSelection();
			} else {
				WideString new_text=myText.left(cursorpos) + myText.mid(cursorpos + 1);
				myText=new_text;
			}
			calcCursorPosition();
			invalidateCache();
			validateAndSendEvent(myText);
			blinker=true;
		}

		if (key_modifier & KeyEvent::KEYMOD_LEFTCTRL) {
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
				invalidateCache();
				calcCursorPosition();
			} else if (event->key == KeyEvent::KEY_a) {
				//ppl7::PrintDebug("select all\n");
				selection.start=0;
				selection.end=(int)myText.size() - 1;
				needsRedraw();
				selectmode=true;

			} else if (event->key == KeyEvent::KEY_HOME && cursorpos > 0) {
				cursorpos=0;
				if (selectmode) selection.go(last_cursorpos - 1, cursorpos);
				calcCursorPosition();
			} else if (event->key == KeyEvent::KEY_END && cursorpos < myText.size()) {
				cursorpos=myText.size();
				if (selectmode) selection.go(last_cursorpos, cursorpos);
				calcCursorPosition();
			}
		}
		if (!selectmode) {
			if (event->key != KeyEvent::KEY_LEFTCTRL && event->key != KeyEvent::KEY_RIGHTCTRL) {
				//ppl7::PrintDebug("selection.clear()\n");
				selection.clear();
			}
		}
		moveScrollbarToCursor();
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
	//ppl7::PrintDebug("TextEdit::timerEvent\n");
	blinker=!blinker;
	needsRedraw();
	//if (GetWindowManager()->getKeyboardFocus()==this) GetWindowManager()->startTimer(this,500);
	//else blinker=false;

}

void TextEdit::mouseDblClickEvent(MouseEvent* event)
{
	if (vertical_scrollbar != NULL && event->p.x >= vertical_scrollbar->x()) return;
	GetWindowManager()->setKeyboardFocus(this);
	selection.clear();
	if (myText.notEmpty()) {
		ppl7::grafix::Point p=event->p;
		if (vertical_scrollbar) p.y+=vertical_scrollbar->position() * line_height;

		cursorpos=calcPosition(p);
		selection.begin(cursorpos);
		for (int i=cursorpos;i >= 0;i--) {
			wchar_t c=myText[i];
			if (c == '\n' || c == ' ' || c == '.' || c == ',') break;
			selection.start=i;
		}
		for (int i=cursorpos;i < (int)myText.size();i++) {
			wchar_t c=myText[i];
			if (c == '\n' || c == ' ' || c == '.' || c == ',') break;
			selection.end=i;

		}
		needsRedraw();
		moveScrollbarToCursor();
	}
}


void TextEdit::calcCursorPosition()
{
	WideString text=myText, left;
	Size s1;
	if ((int)cursorpos < 0) cursorpos=0;
	if (cursorpos > text.size()) cursorpos=text.size();
	cursorx=0;
	cursory=0;
	current_line=0;
	if (cursorpos > 0 && cursorpos >= position_cache.size()) {
		std::map<size_t, CacheItem>::const_iterator it=position_cache.find(cursorpos - 1);
		if (it != position_cache.end()) {
			cursorx=it->second.p.x + it->second.size.width;
			cursory=it->second.p.y;
			current_line=it->second.line;

		}
	} else if (cursorpos > 0) {
		std::map<size_t, CacheItem>::const_iterator it=position_cache.find(cursorpos);
		if (it != position_cache.end()) {
			cursorx=it->second.p.x;
			cursory=it->second.p.y;
			current_line=it->second.line;
		}
	}
	needsRedraw();
}

int TextEdit::calcPosition(const ppl7::grafix::Point& p)
{
	int l=p.y / line_height;
	std::map<size_t, CacheItem>::const_iterator it;
	int r=0, mx_y=0;
	//ppl7::PrintDebug("x=%d\n", p.x);
	for (it=position_cache.begin();it != position_cache.end();++it) {
		mx_y=it->second.p.y;
		if (it->second.line == l) {
			r=it->first;
			if (it->second.p.x <= p.x && (it->second.p.x + it->second.size.width) > p.x) return it->first;
			if (r == ((int)position_cache.size() - 1)) return (int)position_cache.size();
		}
	}
	if (p.y >= mx_y + line_height) r=position_cache.size() + 1;
	return r;
}

ppl7::grafix::Point TextEdit::getDrawStartPositionOfChar(size_t pos)
{
	std::map<size_t, CacheItem>::const_iterator it=position_cache.find(pos);
	if (it != position_cache.end()) {
		return it->second.p;
	}
	return ppl7::grafix::Point(0, 0);
}

TextEdit::Selection::Selection()
{
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
	start=-1;
	end=-1;
}

void TextEdit::Selection::begin(int position)
{
	start=end=position;
}

void TextEdit::Selection::update_right(int position)
{
	if (start == -1 || end == -1) start=end=position - 1;
	else if (position == start + 1 && start == end) clear();
	else if (position <= end) start=position;
	else end=position - 1;
	//ppl7::PrintDebug("r: position=%d, start=%d, end=%d\n", position,start,end);

}

void TextEdit::Selection::update_left(int position)
{
	if (start == -1 || end == -1) start=end=position;
	else if (position == start && position == end) clear();
	else if (position >= start) end=position - 1;
	else start=position;
	//ppl7::PrintDebug("l: position=%d, start=%d, end=%d\n", position,start,end);
}


void TextEdit::Selection::go(int start, int end)
{
	if (start > end) {
		for (int i=start;i >= end;i--) update_left(i);
	} else if (start < end) {
		for (int i=start;i <= end;i++) update_right(i);
	}
}

void TextEdit::resizeEvent(ResizeEvent* event)
{
	//ppl7::PrintDebug("TextEdit::resizeEvent\n");
	if (vertical_scrollbar) {
		Size s=clientSize();
		vertical_scrollbar->setPos(s.width - 19, 0);
		vertical_scrollbar->setHeight(s.height);
	}
}

void TextEdit::valueChangedEvent(ppltk::Event* event, int value)
{
	if (vertical_scrollbar != NULL && event->widget() == vertical_scrollbar) {
		//ppl7::PrintDebug("TextEdit::valueChangedEvent\n");
		needsRedraw();
		return;
	}
	Frame::valueChangedEvent(event, value);
}

}	// EOF namespace ppltk

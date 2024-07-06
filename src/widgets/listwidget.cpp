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


ListWidget::ListWidget(int x, int y, int width, int height)
	: ppltk::Frame(x, y, width, height)
{
	setClientOffset(2, 2, 2, 2);
	scrollbar=NULL;
	myCurrentIndex=0;
	scrollbar=new Scrollbar(width - 29, 0, 29, height - 4);
	scrollbar->setEventHandler(this);
	this->addChild(scrollbar);
	mouseOverIndex=-1;
	visibleItems=height / 30;
	scrollbar->setVisible(false);
	scrollbar->setVisibleItems(visibleItems);
	sortingEnabled=false;

	const ppltk::WidgetStyle& style=ppltk::GetWidgetStyle();
	setBackgroundColor(style.listBackgroundColor);
	//printf ("visibleItems=%d\n",visibleItems);
}

ppl7::String ListWidget::currentText() const
{
	return myCurrentText;
}

ppl7::String ListWidget::currentIdentifier() const
{
	return myCurrentIdentifier;
}

void ListWidget::setCurrentText(const ppl7::String& text)
{
	if (text != myCurrentText) {
		std::list<ListWidgetItem>::iterator it;
		for (it=items.begin();it != items.end();++it) {
			if ((*it).text == text) {
				myCurrentText=(*it).text;
				myCurrentIdentifier=(*it).identifier;
				myCurrentIndex=(*it).index;
				size_t start=scrollbar->position();
				if (start + visibleItems <= myCurrentIndex) {
					scrollbar->setPosition(myCurrentIndex);
				}
				needsRedraw();
				return;
			}
		}
	}
}

void ListWidget::setCurrentIndex(size_t index)
{
	if (index >= items.size()) return;
	std::list<ListWidgetItem>::iterator it;
	for (it=items.begin();it != items.end();++it) {
		if ((*it).index == index) {
			myCurrentIndex=index;
			size_t start=scrollbar->position();
			if (start + visibleItems <= index) {
				scrollbar->setPosition(index);
			}
			myCurrentText=(*it).text;
			myCurrentIdentifier=(*it).identifier;
			needsRedraw();
			return;
		}
	}
}

size_t ListWidget::currentIndex() const
{
	return myCurrentIndex;
}

void ListWidget::add(const ppl7::String& text, const ppl7::String& identifier)
{
	ListWidgetItem item;
	item.text=text;
	item.identifier=identifier;
	item.index=items.size();
	items.push_back(item);
	if (items.size() == 1) setCurrentIndex(0);
	scrollbar->setSize((int)items.size());
	if (items.size() > visibleItems) scrollbar->setVisible(true);
	else scrollbar->setVisible(false);
	if (sortingEnabled) updateSortedItems();
	needsRedraw();
}

void ListWidget::remove(size_t index)
{
	std::list<ListWidgetItem>::iterator it;
	for (it=items.begin();it != items.end();++it) {
		if ((*it).index == index) {
			items.erase(it);
			break;
		}
	}
	scrollbar->setSize((int)items.size());
	if (items.size() > visibleItems) scrollbar->setVisible(true);
	else scrollbar->setVisible(false);
	if (sortingEnabled) updateSortedItems();
	needsRedraw();
}


void ListWidget::remove(const ppl7::String& identifier)
{
	std::list<ListWidgetItem>::iterator it;
	for (it=items.begin();it != items.end();++it) {
		if ((*it).identifier == identifier) {
			items.erase(it);
			break;
		}
	}
	scrollbar->setSize((int)items.size());
	if (items.size() > visibleItems) scrollbar->setVisible(true);
	else scrollbar->setVisible(false);
	if (sortingEnabled) updateSortedItems();
	needsRedraw();
}

void ListWidget::clear()
{
	myCurrentText.clear();
	myCurrentIdentifier.clear();
	items.clear();
	sorted_items.clear();
	scrollbar->setPosition(0);
	scrollbar->setSize(0);
	scrollbar->setVisible(false);
	needsRedraw();
}

const std::list<ListWidget::ListWidgetItem>& ListWidget::getItems() const
{
	return items;
}

ppl7::String ListWidget::widgetType() const
{
	return "ListWidget";
}

void ListWidget::paint(ppl7::grafix::Drawable& draw)
{
	ppltk::Frame::paint(draw);
	const ppltk::WidgetStyle& style=ppltk::GetWidgetStyle();
	//int h=height()-1;
	int y=0;
	ppl7::grafix::Font myFont=style.buttonFont;
	ppl7::grafix::Color unevenColor=style.listBackgroundColor * 1.1f;
	ppl7::grafix::Color selectionColor=style.inputSelectedBackgroundColor;
	ppl7::grafix::Color mouseoverSelectionColor=style.inputSelectedBackgroundColor * 1.3f;
	ppl7::grafix::Color mouseoverColor=style.listBackgroundColor * 1.7f;
	ppl7::grafix::Color dividerColor=style.listBackgroundColor * 0.8f;
	std::list<ListWidgetItem>::iterator it;
	myFont.setColor(style.labelFontColor);
	myFont.setOrientation(ppl7::grafix::Font::TOP);
	int start=scrollbar->position();
	//ppl7::grafix::Drawable client=draw.getDrawable(1, 1, draw.width() - 2, draw.height() - 2);
	ppl7::grafix::Drawable client=clientDrawable(draw);
	int w=width() - 2;
	if (scrollbar->isVisible()) w-=29;
	int c=0;
	for (it=items.begin();it != items.end();++it) {
		if (start <= c) {
			if (c == mouseOverIndex && (*it).index != myCurrentIndex)
				client.fillRect(0, y, w, y + 30, mouseoverColor);
			else if (c == mouseOverIndex && (*it).index == myCurrentIndex)
				client.fillRect(0, y, w, y + 30, mouseoverSelectionColor);
			else if ((*it).index == myCurrentIndex)
				client.fillRect(0, y, w, y + 30, selectionColor);
			else if (c & 1)
				client.fillRect(0, y, w, y + 30, unevenColor);
			else
				client.fillRect(0, y, w, y + 30, style.listBackgroundColor);
			ppl7::grafix::Size s=myFont.measure((*it).text);
			client.print(myFont, 4, y + ((30 - s.height) >> 1), (*it).text);
			client.line(0, y + 29, w, y + 29, dividerColor);
			y+=30;
		}
		c++;
	}
}

void ListWidget::valueChangedEvent(ppltk::Event* event, int value)
{
	if (event->widget() == scrollbar) {
		this->needsRedraw();
	} else {
		EventHandler::valueChangedEvent(event, value);
	}
}

void ListWidget::mouseDownEvent(ppltk::MouseEvent* event)
{
	if (event->p.x < width() - 29 && event->widget() == this) {
		size_t index=scrollbar->position() + event->p.y / 30;
		if (index > items.size()) return;
		setCurrentIndex((size_t)index);
		ppltk::Event ev(ppltk::Event::ValueChanged);
		ev.setWidget(this);
		valueChangedEvent(&ev, myCurrentIndex);
		needsRedraw();
	}
	EventHandler::mouseDownEvent(event);
}

void ListWidget::mouseWheelEvent(ppltk::MouseEvent* event)
{
	scrollbar->mouseWheelEvent(event);
	/*
	if (event->wheel.y < 0) {
		scrollbar->setPosition(scrollbar->position() + 1);
		this->needsRedraw();
	} else if (event->wheel.y > 0) {
		scrollbar->setPosition(scrollbar->position() - 1);
		this->needsRedraw();
	}
	*/
}

void ListWidget::mouseMoveEvent(ppltk::MouseEvent* event)
{
	if (event->p.x < width() - 30 && event->widget() == this) {
		//printf ("x=%d\n",event->p.x);
		mouseOverIndex=scrollbar->position() + event->p.y / 30;
		this->needsRedraw();
	}
}


void ListWidget::lostFocusEvent(ppltk::FocusEvent* event)
{
	Widget* new_widget=event->newWidget();
	if (new_widget) {
		if (new_widget == this || new_widget->isChildOf(this)) {
			printf("ListWidget::lostFocusEvent, but we are still inside of childs of this widget\n");
			event->accept(false);
			return;
		}
	}
	event->accept(true);
	FocusEvent new_event(Event::FocusOut, this, new_widget);
	Frame::lostFocusEvent(&new_event);
}

bool ListWidget::hasIdentifier(const ppl7::String& identifier) const
{
	for (auto it=items.begin();it != items.end();++it) {
		if (it->identifier == identifier) return true;
	}
	return false;
}

bool ListWidget::hasText(const ppl7::String& text) const
{
	for (auto it=items.begin();it != items.end();++it) {
		if (it->text == text) return true;
	}
	return false;
}

void ListWidget::updateSortedItems()
{
	sorted_items.clear();
	for (auto it=items.begin();it != items.end();++it) {
		sorted_items.insert(std::pair<ppl7::String, ListWidgetItem&>(it->text, *it));
	}

}

void ListWidget::setSortingEnabled(bool enable)
{
	sortingEnabled=enable;
	if (sortingEnabled) updateSortedItems();
	needsRedraw();
}

bool ListWidget::isSortingEnabled() const
{
	return sortingEnabled;
}


} //EOF namespace

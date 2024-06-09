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


#include "ppltk.h"


namespace ppltk {

using namespace ppl7;
using namespace ppl7::grafix;


TabWidget::TabItem::TabItem(const String& label, const Drawable& icon, Widget* widget)
{
	enabled=true;
	visible=true;
	this->widget=widget;
	this->label=label;
	this->icon=icon;
}

TabWidget::TabWidget()
{
	//const WidgetStyle& widgetstyle=GetWidgetStyle();
	setTransparent(false);
	setClientOffset(3, 3, 3, 3);
	myIndex=-1;
}

TabWidget::TabWidget(int x, int y, int width, int height)
{
	//const WidgetStyle& wstyle=GetWidgetStyle();
	create(x, y, width, height);
	setTransparent(false);
	setClientOffset(3, 3, 3, 3);
	myIndex=-1;
}

TabWidget::~TabWidget()
{
	clear();
}

void TabWidget::emmitCurrentChanged()
{
	ppltk::Event ev(ppltk::Event::CurrentChanged);
	ev.setWidget(this);
	currentChanged(&ev, myIndex);
}

void TabWidget::clear()
{
	std::vector<TabItem>::iterator it;
	for (it=myTabs.begin();it != myTabs.end();++it) {
		removeChild(it->widget);
		delete it->widget;
	}
	myTabs.clear();
	myIndex=-1;
	needsRedraw();
}


void TabWidget::removeTab(int index)
{
	if (index < (int)myTabs.size()) {
		TabItem& tab=myTabs[index];
		removeChild(tab.widget);
		delete tab.widget;
		myTabs.erase(myTabs.begin() + index);
		if (myIndex > index) {
			myIndex--;
			emmitCurrentChanged();
		} else if (myIndex >= (int)myTabs.size()) {
			myIndex=(int)myTabs.size() - 1;
			emmitCurrentChanged();
		}
		needsRedraw();
	}
}

int TabWidget::indexOf(Widget* widget) const
{
	std::vector<TabItem>::const_iterator it;
	int c=0;
	for (it=myTabs.begin();it != myTabs.end();++it) {
		if (it->widget == widget) return c;
		c++;
	}
	return -1;
}

int TabWidget::addTab(Widget* page, const String& label, const Drawable& icon)
{
	TabItem tab(label, icon, page);
	myTabs.push_back(tab);
	myIndex=(int)myTabs.size() - 1;
	needsRedraw();
	emmitCurrentChanged();
	return myIndex;
}

int TabWidget::insertTab(int index, Widget* page, const String& label, const Drawable& icon)
{
	if (index > (int)myTabs.size()) return addTab(page, label, icon);
	TabItem tab(label, icon, page);
	std::vector<TabItem>::const_iterator it=myTabs.begin() + index;
	myTabs.insert(it, tab);
	myIndex=index;
	emmitCurrentChanged();
	return index;
}

int TabWidget::count() const
{
	return (int)myTabs.size();
}

int TabWidget::currentIndex() const
{
	return myIndex;
}

Widget* TabWidget::currentWidget() const
{
	if (myIndex < 0) return NULL;
	return myTabs[myIndex].widget;
}

const String& TabWidget::tabText(int index) const
{
	if (index < 0 || index >= (int)myTabs.size()) throw ppl7::OutOfBoundsEception("Tab %d does not exist", index);
	return myTabs[index].label;
}

const Drawable& TabWidget::tabIcon(int index) const
{
	if (index < 0 || index >= (int)myTabs.size()) throw ppl7::OutOfBoundsEception("Tab %d does not exist", index);
	return myTabs[index].icon;
}

bool TabWidget::isTabVisible(int index) const
{
	if (index < 0 || index >= (int)myTabs.size()) throw ppl7::OutOfBoundsEception("Tab %d does not exist", index);
	return myTabs[index].visible;
}

bool TabWidget::isTabEnabled(int index) const
{
	if (index < 0 || index >= (int)myTabs.size()) throw ppl7::OutOfBoundsEception("Tab %d does not exist", index);
	return myTabs[index].enabled;
}

void TabWidget::setCurrentIndex(int index)
{
	if (index < 0 || index >= (int)myTabs.size()) return;
	if (myIndex != index) {
		myIndex=index;
		needsRedraw();
		emmitCurrentChanged();
	}
}

void TabWidget::setCurrentWidget(Widget* widget)
{
	setCurrentIndex(indexOf(widget));
}

void TabWidget::setTabText(int index, const String& label)
{
	if (index < 0 || index >= (int)myTabs.size()) return;
	if (myTabs[index].label != label) {
		myTabs[index].label=label;
		needsRedraw();
	}
}

void TabWidget::setTabIcon(int index, const Drawable& icon)
{
	if (index < 0 || index >= (int)myTabs.size()) return;
	myTabs[index].icon=icon;
	needsRedraw();
}

void TabWidget::setTabVisible(int index, bool visible)
{
	if (index < 0 || index >= (int)myTabs.size()) return;
	if (myTabs[index].visible != visible) {
		myTabs[index].visible=visible;
		needsRedraw();
	}
}

void TabWidget::setTabEnabled(int index, bool enabled)
{
	if (index < 0 || index >= (int)myTabs.size()) return;
	if (myTabs[index].enabled != enabled) {
		myTabs[index].enabled=enabled;
		needsRedraw();
	}
}

void TabWidget::paint(Drawable& draw)
{
	Widget::paint(draw);
}

String TabWidget::widgetType() const
{
	return "TabWidget";
}


ppl7::grafix::Size TabWidget::sizeHint() const
{
	return ppl7::grafix::Size::invalid();
}

ppl7::grafix::Size TabWidget::minimumSizeHint() const
{
	return ppl7::grafix::Size::invalid();
}

void TabWidget::mouseDownEvent(MouseEvent* event)
{
	ppl7::PrintDebug("TabWidget::mouseDownEvent\n");
}


}	// EOF namespace ppltk

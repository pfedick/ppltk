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
	const WidgetStyle& style=GetWidgetStyle();
	myFont=style.labelFont;
	setTransparent(true);
	Size s=myFont.measure(L" ");
	tabHeight=s.height + 9;
	setClientOffset(3, tabHeight + 3, 3, 3);
	myIndex=-1;
	current_widget=NULL;
}

TabWidget::TabWidget(int x, int y, int width, int height)
{
	const WidgetStyle& style=GetWidgetStyle();
	myFont=style.labelFont;
	create(x, y, width, height);
	setTransparent(true);
	Size s=myFont.measure(L" ");
	tabHeight=s.height + 9;
	setClientOffset(3, tabHeight + 3, 3, 3);
	myIndex=-1;
	current_widget=NULL;
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

void TabWidget::updateCurrentWidget()
{
	if (current_widget) removeChild(current_widget);
	current_widget=NULL;
	if (myIndex < 0 || myIndex >= (int)myTabs.size()) return;
	current_widget=myTabs[myIndex].widget;
	current_widget->setPos(0, 0);
	current_widget->setSize(clientSize());
	addChild(current_widget);
}

void TabWidget::clear()
{
	if (current_widget) removeChild(current_widget);
	std::vector<TabItem>::iterator it;
	for (it=myTabs.begin();it != myTabs.end();++it) {
		removeChild(it->widget);
		delete it->widget;
	}
	myTabs.clear();
	myIndex=-1;
	current_widget=NULL;
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
			updateCurrentWidget();
			emmitCurrentChanged();
		} else if (myIndex >= (int)myTabs.size()) {
			myIndex=(int)myTabs.size() - 1;
			updateCurrentWidget();
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
	page->setPos(0, 0);
	page->setSize(clientSize());
	myTabs.push_back(tab);
	myIndex=(int)myTabs.size() - 1;
	addChild(page);
	needsRedraw();
	updateCurrentWidget();
	emmitCurrentChanged();
	return myIndex;
}

int TabWidget::insertTab(int index, Widget* page, const String& label, const Drawable& icon)
{
	if (index > (int)myTabs.size()) return addTab(page, label, icon);
	TabItem tab(label, icon, page);
	page->setPos(0, 0);
	page->setSize(clientSize());
	std::vector<TabItem>::const_iterator it=myTabs.begin() + index;
	myTabs.insert(it, tab);
	myIndex=index;
	addChild(page);
	updateCurrentWidget();
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

const Font& TabWidget::font() const
{
	return myFont;
}

void TabWidget::setFont(const Font& font)
{
	myFont=font;
	Size s=myFont.measure(L" ");
	tabHeight=s.height + 9;
	setClientOffset(3, tabHeight + 3, 3, 3);

	needsRedraw();
	updateCurrentWidget();
}


void TabWidget::setCurrentIndex(int index)
{
	if (index < 0 || index >= (int)myTabs.size()) return;
	if (myIndex != index) {
		myIndex=index;
		needsRedraw();
		updateCurrentWidget();
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

int TabWidget::drawTab(Drawable& draw, TabItem& tab, int x, const WidgetStyle& wstyle)
{
	if (!tab.visible) return 0;
	int w=2;
	if (tab.label.notEmpty()) {
		Size s=myFont.measure(tab.label);
		w+=s.width + 8;
	}

	if (tab.icon.isEmpty() == false) {
		w+=4 + tab.icon.width();
	}
	if (w < 20) w=20;

	int y=0;
	Color shade1=wstyle.buttonFontColor;
	Color shade2=wstyle.frameBorderColorShadow * 0.5f;
	if (tab.widget == current_widget) {
		draw.fillRect(x, 0, x + w, tabHeight, wstyle.frameBackgroundColor);
		draw.line(x, 0, x + w, 0, wstyle.frameBorderColorLight);
		draw.line(x, 0, x, tabHeight, wstyle.frameBorderColorLight);
		draw.line(x + w, 0, x + w, tabHeight - 1, wstyle.frameBorderColorShadow);
	} else {
		y=4;
		shade1=wstyle.buttonFontColor * 0.8f;
		draw.fillRect(x, 4, x + w, tabHeight - 1, wstyle.tabNonSelectedBackgroundColor);
		draw.line(x, 4, x + w, 4, wstyle.frameBorderColorLight);
		if (x == 0) draw.line(x, 4, x, tabHeight - 1, wstyle.frameBorderColorLight);
		else draw.line(x, 5, x, tabHeight - 1, shade2);
		draw.line(x + w, 4, x + w, tabHeight - 1, wstyle.frameBorderColorShadow);
		draw.line(x, tabHeight, x + w, tabHeight, wstyle.frameBorderColorLight);
	}

	tab.x1=x;
	tab.x2=x + w;


	if (tab.icon.isEmpty() == false) {
		x+=2;
		if (tab.enabled) draw.bltAlpha(tab.icon, x, y + (tabHeight - y - tab.icon.height()) / 2);
		else draw.bltBlend(tab.icon, 0.5f, x, y + (tabHeight - y - tab.icon.height()) / 2);
		x+=2 + tab.icon.width();
	}
	if (tab.label.notEmpty()) {
		myFont.setOrientation(Font::TOP);

		if (tab.enabled) myFont.setColor(shade1);
		else myFont.setColor(wstyle.tabDisabledFontColor);
		Size s=myFont.measure(tab.label);
		x+=4;
		draw.print(myFont, x, y + ((tabHeight - y - s.height) >> 1), tab.label);
	}



	return w + 1;
}

void TabWidget::paint(Drawable& draw)
{
	//Widget::paint(draw);
	const WidgetStyle& wstyle=GetWidgetStyle();
	/*
	myBackground=wstyle.frameBackgroundColor;
	myBorderColorLight=wstyle.frameBorderColorLight;
	myBorderColorShadow=wstyle.frameBorderColorShadow;
	*/
	Drawable d=draw.getDrawable(0, tabHeight, draw.width(), draw.height());
	int w=d.width() - 1;
	int h=d.height() - 1;
	d.cls(wstyle.frameBackgroundColor);
	d.line(0, 0, 0, h, wstyle.frameBorderColorLight);
	d.line(0, h, w, h, wstyle.frameBorderColorShadow);
	d.line(w, 0, w, h, wstyle.frameBorderColorShadow);
	std::vector<TabItem>::iterator it;
	int x=0;
	for (it=myTabs.begin();it != myTabs.end();++it) {
		x+=drawTab(draw, *it, x, wstyle);

	}
	d.line(x, 0, w, 0, wstyle.frameBorderColorLight);
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
	if (event->p.y < 30) {
		//ppl7::PrintDebug("TabWidget::mouseDownEvent\n");
		std::vector<TabItem>::const_iterator it;
		for (it=myTabs.begin();it != myTabs.end();++it) {
			if (event->p.x >= it->x1 && event->p.x < it->x2 && it->enabled == true && it->visible == true) {
				setCurrentWidget(it->widget);
				return;
			}
		}
	}
}

}	// EOF namespace ppltk

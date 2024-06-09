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
#include "ppltk-layout.h"


namespace ppltk {

using namespace ppl7;
using namespace ppl7::grafix;

Widget::Widget()
{
	parent=NULL;
	myLayout=NULL;
	lockcount=0;
	visible=true;
	enabled=true;
	needsredraw=true;
	transparent=false;
	child_needsredraw=false;
	topMost=false;
	modal=false;
	deleteRequested=false;
	MaxSize.width=65535;
	MaxSize.height=65535;
	MinSize.width=0;
	MinSize.height=0;
	use_own_drawbuffer=false;
	myName.set("unknown");
}

Widget::Widget(int x, int y, int width, int height)
{
	parent=NULL;
	lockcount=0;
	visible=true;
	enabled=true;
	needsredraw=true;
	transparent=false;
	child_needsredraw=false;
	topMost=false;
	modal=false;
	deleteRequested=false;
	MaxSize.width=65535;
	MaxSize.height=65535;
	MinSize.width=0;
	MinSize.height=0;
	use_own_drawbuffer=false;
	myName.set("unknown");
	create(x, y, width, height);
}

Widget::~Widget()
{
	GetWindowManager()->unregisterWidget(this);
	if (parent) parent->removeChild(this);
	if (myLayout) delete myLayout;
	std::list<Widget*>::iterator it;
	while (childs.begin() != childs.end()) {
		Widget* child=*childs.begin();
		delete(child);
	}
	childs.clear();
}

void Widget::setLayout(Layout* layout)
{
	myLayout=layout;
	layout->myParent=this;
	layout->isValid=false;
}

Layout* Widget::layout() const
{
	return myLayout;
}

void Widget::invalidateLayout()
{
	if (myLayout) myLayout->isValid=false;
	needsRedraw();
	if (parent && parent->myLayout) parent->invalidateLayout();
}


void Widget::updateDrawbuffer()
{
	if (!use_own_drawbuffer) return;
	if (drawbuffer.width() == s.width && drawbuffer.height() == s.height) return;
	drawbuffer.create(s.width, s.height);
	needsRedraw();
}

void Widget::setUseOwnDrawbuffer(bool enable)
{
	use_own_drawbuffer=enable;
	updateDrawbuffer();
}

void Widget::destroyChilds()
{
	std::list<Widget*>::iterator it;
	while (childs.begin() != childs.end()) {
		Widget* child=*childs.begin();
		delete(child);
	}
	childs.clear();
}

void Widget::deleteLater()
{
	deleteRequested=true;
}

size_t Widget::numChilds() const
{
	return childs.size();
}

std::list<Widget*>::iterator Widget::childsBegin()
{
	return childs.begin();
}

std::list<Widget*>::iterator Widget::childsEnd()
{
	return childs.end();
}

std::list<Widget*>::const_iterator Widget::childsBegin() const
{
	return childs.begin();
}

std::list<Widget*>::const_iterator Widget::childsEnd() const
{
	return childs.end();
}

Widget* Widget::getParent() const
{
	return parent;
}

Widget* Widget::getTopmostParent() const
{
	if (parent) return parent->getTopmostParent();
	return (Widget*)this;
}

void Widget::addChild(Widget* w)
{
	if (w == NULL) throw NullPointerException();
	if (w == this) return;
	if (w->parent) w->parent->removeChild(this);
	childs.push_back(w);
	w->parent=this;
	childNeedsRedraw();
	geometryChanged();
}

void Widget::removeChild(Widget* w)
{
	if (w == NULL) throw NullPointerException();
	if (w == this) return;
	childs.remove(w);
	w->parent=NULL;
	needsRedraw();
	geometryChanged();
}

void Widget::needsRedraw()
{
	if (needsredraw == true) return;
	needsredraw=true;
	child_needsredraw=true;
	if (parent) {
		if (transparent) parent->needsRedraw();
		else parent->childNeedsRedraw();
	}
}

void Widget::childNeedsRedraw()
{
	if (child_needsredraw) return;
	child_needsredraw=true;
	if (parent) parent->childNeedsRedraw();
}

void Widget::parentMustRedraw()
{
	needsredraw=true;
	if (parent) parent->parentMustRedraw();
}

bool Widget::redrawRequired() const
{
	if (needsredraw) return true;
	if (child_needsredraw) return true;
	return false;
}

void Widget::setTopmost(bool flag)
{
	if (topMost != flag) {
		topMost=flag;
		parentMustRedraw();
	}
}

void Widget::toTop(Widget* w)
{
	if (w == NULL) {
		if (parent) parent->toTop(this);
		return;
	}
	if (w == this) return;
	childs.remove(w);
	w->parent=NULL;
	needsRedraw();
	childs.push_front(w);
	w->parent=this;
	childNeedsRedraw();
}

void Widget::toTop(Widget& w)
{
	toTop(&w);
}

const Point& Widget::pos() const
{
	return p;
}

Point Widget::absolutePosition() const
{
	if (parent) return parent->absolutePosition() + p + myClientOffset.topLeft();
	return p + myClientOffset.topLeft();
}

const Size& Widget::size() const
{
	return s;
}

Rect Widget::rect() const
{
	return Rect(p.x, p.y, s.width, s.height);
}


int Widget::width() const
{
	return s.width;
}

int Widget::height() const
{
	return s.height;
}

const Size Widget::maxSize() const
{
	return MaxSize;
}

const Size Widget::minSize() const
{
	return MinSize;
}

void Widget::setMaxSize(const Size& s)
{
	if (MaxSize != s) {
		MaxSize=s;
		geometryChanged();
	}
}

void Widget::setMaxSize(int width, int height)
{
	if (width != MaxSize.width || height != MaxSize.height) {
		MaxSize.width=width;
		MaxSize.height=height;
		geometryChanged();
	}
}

void Widget::setMinSize(const Size& s)
{
	if (MinSize != s) {
		MinSize=s;
		geometryChanged();
	}
}

void Widget::setMinSize(int width, int height)
{
	if (width != MinSize.width || height != MinSize.height) {
		MinSize.width=width;
		MinSize.height=height;
		geometryChanged();
	}
}

const SizePolicy& Widget::sizePolicy() const
{
	return mySizePolicy;
}

void Widget::setSizePolicy(const SizePolicy& policy)
{
	mySizePolicy=policy;
	geometryChanged();
}

void Widget::setSizePolicy(SizePolicy::Policy horizontal, SizePolicy::Policy vertical)
{
	mySizePolicy.HorizontalPolicy=horizontal;
	mySizePolicy.VerticalPolicy=vertical;
	geometryChanged();
}


ppl7::grafix::Size Widget::sizeHint() const
{
	/*The default implementation of sizeHint() returns an invalid size if there is no layout for this widget, and returns the layout's preferred size otherwise.
	*/

	if (!myLayout) return ppl7::grafix::Size::invalid();
	ppl7::grafix::Size s=myLayout->sizeHint();
	if (s.isValid()) s+=ppl7::grafix::Size(myClientOffset.left() + myClientOffset.right(),
		myClientOffset.top() + myClientOffset.bottom());
	return s;

}

ppl7::grafix::Size Widget::minimumSizeHint() const
{
	/*The default implementation of minimumSizeHint() returns an invalid size if there is no layout for this widget, and returns the layout's minimum size otherwise. Most built-in widgets reimplement minimumSizeHint().
	*/

	if (!myLayout) return ppl7::grafix::Size::invalid();
	ppl7::grafix::Size s=myLayout->minimumSize();
	if (s.isValid()) s+=ppl7::grafix::Size(myClientOffset.left() + myClientOffset.right(),
		myClientOffset.top() + myClientOffset.bottom());
	return s;
}


void Widget::geometryChanged()
{
	Event ev(Event::GeometryChanged);
	ev.setWidget(this);
	geometryChangedEvent(&ev);
	if (ev.isAccepted()) return;
	if (parent) parent->geometryChanged();
}

int Widget::x() const
{
	return p.x;
}

int Widget::y() const
{
	return p.y;
}


void Widget::setTransparent(bool flag)
{
	if (flag != transparent) {
		transparent=flag;
	}
}

bool Widget::isTransparent() const
{
	return transparent;
}

void Widget::setEnabled(bool flag)
{
	if (enabled != flag) {
		enabled=flag;
		needsRedraw();
	}
}

bool Widget::isEnabled() const
{
	return enabled;
}

void Widget::setVisible(bool flag)
{
	if (visible != flag) {
		visible=flag;
		needsRedraw();
		geometryChanged();
	}
}

bool Widget::isVisible() const
{
	return visible;
}

void Widget::setModal(bool flag)
{
	modal=flag;
}

bool Widget::isModal() const
{
	return modal;
}



void Widget::create(int x, int y, int width, int height)
{
	p.x=x;
	p.y=y;
	s.width=width;
	s.height=height;
	updateDrawbuffer();
	parentMustRedraw();
}

void Widget::setX(int x)
{
	p.x=x;
	parentMustRedraw();
}

void Widget::setY(int y)
{
	p.y=y;
	parentMustRedraw();
}

void Widget::setPos(int x, int y)
{
	p.x=x;
	p.y=y;
	parentMustRedraw();
}

void Widget::setPos(const Point& p)
{
	this->p=p;
	parentMustRedraw();
}

void Widget::setWidth(int width)
{
	s.width=width;
	updateDrawbuffer();
	parentMustRedraw();
}

void Widget::setHeight(int height)
{
	s.height=height;
	updateDrawbuffer();
	parentMustRedraw();
}

void Widget::setSize(int width, int height)
{
	s.width=width;
	s.height=height;
	updateDrawbuffer();
	parentMustRedraw();
}

void Widget::setSize(const Size& s)
{
	this->s=s;
	updateDrawbuffer();
	parentMustRedraw();
}

void Widget::setClientOffset(int left, int top, int right, int bottom)
{
	myClientOffset.x1=left;
	myClientOffset.y1=top;
	myClientOffset.x2=right;
	myClientOffset.y2=bottom;
}

Drawable Widget::drawable(const Drawable& parent) const
{
	if (use_own_drawbuffer) return drawbuffer;
	Drawable d;
	d.copy(parent, p, s);
	return d;
}

Drawable Widget::clientDrawable(const Drawable& parent) const
{
	Drawable d;
	Point p1(myClientOffset.x1, myClientOffset.y1);
	Size s1=s;
	s1.width-=(myClientOffset.x1 + myClientOffset.x2);
	s1.height-=(myClientOffset.y1 + myClientOffset.y2);
	if (s1.width < 0) s1.width=0;
	if (s1.height < 0) s1.height=0;
	d.copy(parent, p1, s1);
	return d;
}

Rect Widget::clientRect() const
{
	return Rect(
		myClientOffset.x1,
		myClientOffset.y1,
		s.width - myClientOffset.x2 - myClientOffset.x1,
		s.height - myClientOffset.y2 - myClientOffset.y1
	);
}

Size Widget::clientSize() const
{
	return Size(
		s.width - myClientOffset.x1 - myClientOffset.x2,
		s.height - myClientOffset.y1 - myClientOffset.y2);
}


void Widget::draw(Drawable& d)
{
	if (!visible) return;
	if (myLayout && myLayout->isValid == false) myLayout->recalculate();
	if (needsredraw == false && child_needsredraw == false) return;
	std::list<Widget*>::iterator it;
	Drawable mycd=drawable(d);
	Drawable cd;
	if (needsredraw) {
		paint(mycd);
		child_needsredraw=true;
	}
	if (child_needsredraw) {
		// Jetzt die unten liegenden Childs
		cd=clientDrawable(mycd);
		for (it=childs.begin();it != childs.end();++it) {
			Widget* child=*it;
			if (child->topMost == false) {
				if (needsredraw) child->redraw(cd);
				else child->draw(cd);
			}
		}

		// Dann die TopMost Childs
		for (it=childs.begin();it != childs.end();++it) {
			Widget* child=*it;
			if (child->topMost == true) {
				if (needsredraw) child->redraw(cd);
				else child->draw(cd);
			}
		}
		child_needsredraw=false;
	}
	if (use_own_drawbuffer) {
		//printf("Blt Widget with own drawbuffer\n");
		d.blt(drawbuffer, p.x, p.y);
		if (parent) parent->markWidgetsAboveForRedraw(this);
	}
	needsredraw=false;
}

void Widget::redraw(Drawable& d)
{
	needsRedraw();
	draw(d);
}

void Widget::markWidgetsAboveForRedraw(Widget* widget)
{
	if (parent) parent->markWidgetsAboveForRedraw(this);
	std::list<Widget*>::iterator it;
	Rect wrect=widget->rect();
	bool found=false;
	if (widget->topMost == false) {
		for (it=childs.begin();it != childs.end();++it) {
			Widget* child=*it;
			if (child->topMost == false) {
				if (child == widget) found=true;
				else if (found) {
					// must redraw if in range of widget
					if (wrect.intersects(child->rect())) child->needsRedraw();
				}
			}
		}
	}
	for (it=childs.begin();it != childs.end();++it) {
		Widget* child=*it;
		if (child->topMost == true) {
			if (child == widget) found=true;
			else if (found) {
				// must redraw if in range of widget
				if (wrect.intersects(child->rect())) child->needsRedraw();
			}
		}
	}
}

void Widget::paint(Drawable& draw)
{
	int step;
	Color red(255, 0, 0, 255), white(255, 255, 255, 255);
	int y2=draw.height() - 1;
	int x2=draw.width() - 1;

	for (int x=0;x < draw.width();x++) {
		step=x % 4;
		if (step == 0 || step == 1) {
			draw.putPixel(x, 0, red);
			if (y2 > 0) draw.putPixel(x, y2, red);
		} else {
			draw.putPixel(x, 0, white);
			if (y2 > 0) draw.putPixel(x, y2, white);
		}
	}
	for (int y=0;y < draw.height();y++) {
		step=y % 4;
		if (step == 0 || step == 1) {
			draw.putPixel(0, y, red);
			if (x2 > 0) draw.putPixel(x2, y, red);
		} else {
			draw.putPixel(0, y, white);
			if (x2 > 0) draw.putPixel(x2, y, white);
		}
	}
}

/*
Size Widget::preferedSize() const
{
	Size s, c;
	if (MinSize.width > 0) s.width=MinSize.width;
	if (MinSize.height > 0) s.height=MinSize.height;
	int w=myClientOffset.x1 + myClientOffset.x2;
	int h=myClientOffset.y1 + myClientOffset.y2;
	c=contentSize();
	c.width+=w;
	c.height+=h;

	if (c.width > s.width) s.width=c.width;
	if (c.height > s.height) s.height=c.height;
	if (s.width > MaxSize.width) s.width=MaxSize.width;
	if (s.height > MaxSize.height) s.height=MaxSize.height;
	return s;
}

Size Widget::contentSize() const
{
	return Size();
}
*/


void Widget::setName(const String& name)
{
	myName=name;
}

const String& Widget::name() const
{
	return myName;
}

String Widget::widgetType() const
{
	return String("Widget");
}

bool Widget::isChildOf(Widget* other) const
{
	if (parent == other) return true;
	if (parent) return parent->isChildOf(other);
	return false;
}

void Widget::setFocus()
{
	GetWindowManager()->setKeyboardFocus(this);
}

bool Widget::hasFocus() const
{
	if (GetWindowManager()->getKeyboardFocus() == this) return true;
	return false;
}

}	// EOF namespace ppltk

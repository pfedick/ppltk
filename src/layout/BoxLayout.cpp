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


BoxLayout::Item::Item(Widget* w)
    : type(BoxLayout::ItemType::Widget), widget(w)
{

}

BoxLayout::Item::Item(Layout* l)
    : type(BoxLayout::ItemType::Layout), layout(l)
{

}

BoxLayout::BoxLayout(Direction dir, Widget* parent)
    : Layout(parent)
{
    myDirection = dir;
}

BoxLayout::~BoxLayout()
{

}


BoxLayout::Direction BoxLayout::direction() const
{
    return myDirection;
}

void BoxLayout::setSpacing(int spacing)
{
    mySpacing = spacing;
}

int BoxLayout::spacing() const
{
    return mySpacing;
}

void BoxLayout::addWidget(Widget* widget)
{
    if (!parent()) return;
    item_list.push_back(Item(widget));
    invalidate();
    parent()->addChild(widget);
}

void BoxLayout::addLayout(Layout* layout)
{
    if (!parent()) return;
    // TODO: should we create a widtget and put the layout in?
    item_list.push_back(Item(layout));
    invalidate();
}

void BoxLayout::addSpacing(int size)
{
    if (!parent()) return;
    // TODO: create a widget with minimum and maximum size and policy fixed
    invalidate();
}

int BoxLayout::count() const
{
    return (int)item_list.size();
}

ppl7::grafix::Size BoxLayout::sizeHint() const
{
    return ppl7::grafix::Size(-1, -1);
}

ppl7::grafix::Size BoxLayout::maximumSize() const
{
    return ppl7::grafix::Size(-1, -1);
}

ppl7::grafix::Size BoxLayout::minimumSize() const
{
    return ppl7::grafix::Size(-1, -1);
}

void BoxLayout::update()
{
    std::list<Item>::iterator it;
    const Margins& m = contentsMargins();
    int x = m.left();
    int y = m.top();
    for (it = item_list.begin();it != item_list.end();++it) {
        x += mySpacing;
        if (it->type == ItemType::Widget) {
            it->widget->setPos(x, y);
            it->widget->setSize(50, 30);
        }
    }

}

}	// EOF namespace ppltk

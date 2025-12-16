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

BoxLayout::Item::Item(SpacerItem* s)
    : type(BoxLayout::ItemType::Spacer), spacer(s)
{

}



BoxLayout::BoxLayout(Direction dir, Widget* parent)
    : Layout(parent)
{
    myDirection = dir;
}

BoxLayout::~BoxLayout()
{
    for (auto it = item_list.begin();it != item_list.end();++it) {
        switch (it->type) {
        case ItemType::Layout:
            delete it->layout;
            break;
        case ItemType::Widget:
            delete it->widget;
            break;
        case ItemType::Spacer:
            delete it->spacer;
            break;
        }
    }
    item_list.clear();
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
    if (!parent()) return;  // Wieso?
    item_list.push_back(Item(widget));
    invalidate();
    parent()->addChild(widget);
}

/*
void BoxLayout::addLayout(Layout* layout)
{
    if (!parent()) return; // Wieso?
    item_list.push_back(Item(layout));
    invalidate();
}
*/

void BoxLayout::addSpacing(int size)
{
    if (!parent()) return; // Wieso?
    item_list.push_back(Item(new SpacerItem(size)));
    invalidate();
}


void BoxLayout::addSpacer(SpacerItem* spacer)
{
    if (!parent()) return; // Wieso?
    item_list.push_back(Item(spacer));
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
    //ppl7::PrintDebug("BoxLayout::update() called for layout");
    std::list<Item>::iterator it;
    const Margins& m = contentsMargins();
    Size totalSize = parent()->clientSize();
    //ppl7::PrintDebug("Client Size: %d x %d\n", totalSize.width, totalSize.height);
    //ppl7::PrintDebug("Margins: L:%d T:%d R:%d B:%d, mySpacing:%d\n", m.left(), m.top(), m.right(), m.bottom(), mySpacing);
    totalSize.width -= (m.left() + m.right());
    totalSize.height -= (m.top() + m.bottom());
    //ppl7::PrintDebug("Usable Size: %d x %d\n", totalSize.width, totalSize.height);

    int x = m.left();
    int y = m.top();
    // Evaluate sizes
    int items_total_width = 0;
    int spacers_count = 0;
    int item_count = 0;
    for (auto it = item_list.begin();it != item_list.end();++it) {
        if (it->type == ItemType::Widget) {
            Size s = it->widget->sizeHint();
            //ppl7::PrintDebug("Item Size Hint: %d x %d\n", s.width, s.height);
            items_total_width += s.width;
            items_total_width += mySpacing;
            it->sizeHint = s;
            item_count++;
        }
        else if (it->type == ItemType::Spacer) {
            if (it->spacer->hPolicy == SizePolicy::Fixed) {
                items_total_width += it->spacer->width;
                items_total_width += mySpacing;
                it->sizeHint.setWidth(it->spacer->width);
            }
            else {
                spacers_count++;
            }
        }
    }
    //ppl7::PrintDebug("Items total width: %d\n", items_total_width);
    // Set positions and sizes

    int freespace = totalSize.width - items_total_width;
    if (freespace < 0) freespace = 0;
    //ppl7::PrintDebug("Freespace: %d\n", freespace);

    int remaining_width = totalSize.width;

    for (it = item_list.begin();it != item_list.end();++it) {
        if (it->type == ItemType::Widget) {
            it->widget->setPos(x, y);
            int w = it->sizeHint.width;
            if (!spacers_count) {
                // Verteile den freien Platz auf die Widgets
                w = remaining_width / item_count;
                if (w < it->sizeHint.width) w = it->sizeHint.width;
                item_count--;
                remaining_width -= w;
            }
            it->widget->setSize(w, totalSize.height);
            x += (w + mySpacing);
        }
        else if (it->type == ItemType::Spacer) {
            if (it->spacer->hPolicy == SizePolicy::Fixed) {
                x += it->spacer->width;
                x += mySpacing;
            }
            else {
                int spacer_width = freespace / spacers_count;
                x += spacer_width;
                x += mySpacing;
            }
        }
    }
}

}	// EOF namespace ppltk

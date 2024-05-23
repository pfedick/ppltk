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
#ifndef _PPLTK_LAYOUT_INCLUDE
#define _PPLTK_LAYOUT_INCLUDE

#ifndef _PPL7_INCLUDE
#include <ppl7.h>
#endif

#ifndef _PPL7_GRAFIX
#include <ppl7-grafix.h>
#endif

#include <list>



namespace ppltk {

class Widget;

class Margins
{
private:
    int _left, _right, _top, _bottom;
public:
    Margins();
    Margins(int left, int top, int right, int bottom);
    int left() const;
    int right() const;
    int top() const;
    int bottom() const;
    bool isNull() const;

    void set(int left, int top, int right, int bottom);
    void setLeft(int left);
    void setRight(int right);
    void setTop(int top);
    void setBottom(int bottom);
    Margins& operator*=(int factor);
    Margins& operator*=(float factor);
    Margins& operator+=(const Margins& other);
    Margins& operator+=(int value);
    Margins& operator-=(const Margins& other);
    Margins& operator-=(int value);
    Margins& operator/=(int divisor);
    Margins& operator/=(float divisor);
};

bool operator!=(const Margins& m1, const Margins& m2);
bool operator==(const Margins& m1, const Margins& m2);
Margins operator*(const Margins& margins, int factor);
Margins operator*(int factor, const Margins& margins);
Margins operator*(const Margins& margins, float factor);
Margins operator*(float factor, const Margins& margins);
Margins operator+(const Margins& m1, const Margins& m2);
Margins operator+(const Margins& m1, int value);
Margins operator+(int value, const Margins& m1);
Margins operator-(const Margins& m1, const Margins& m2);
Margins operator-(const Margins& lhs, int rhs);
Margins operator/(const Margins& margins, int divisor);
Margins operator/(const Margins& margins, float divisor);
Margins operator|(const Margins& m1, const Margins& m2);



class Layout
{
private:
    Widget* myParent;
    Margins myMargins;

public:
    Layout(Widget* parent=NULL);
    Widget* parent() const;

    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const Margins& margins);

    virtual int count() const=0;



};

class Spacer : public Layout
{

};

class BoxLayout : public Layout
{
public:
    enum class Direction {
        LeftToRight=0,
        TopToBottom=2
    };

private:
    enum class ItemType {
        Widget,
        Layout,
        Space
    };

    class SpaceItem {
    public:
        int size;
    };

    class Item {
    public:
        ItemType type;
        union element {
            Widget* widget;
            Layout* layout;
            SpaceItem* space;
        };


    };
    Direction myDirection;
    int mySpacing;

public:
    BoxLayout(Direction dir, Widget* parent=NULL);
    virtual ~BoxLayout();
    void addWidget(Widget* widget);
    void addLayout(Layout* layout);
    void addSpacing(int size);
    //void addStretch(int stretch=0);

    void setSpacing(int spacing);
    int spacing() const;
    Direction direction() const;
    virtual int count() const override;
};

class HBoxLayout : public BoxLayout
{
private:
public:
    HBoxLayout();
    HBoxLayout(Widget* parent);
    virtual ~HBoxLayout();

};

class VBoxLayout : public BoxLayout
{
public:
    VBoxLayout();
    VBoxLayout(Widget* parent);
    virtual ~VBoxLayout();
};

class FormLayout : public Layout
{
private:
public:
    FormLayout();
    FormLayout(Widget* parent);
    ~FormLayout();

    void addRow(Widget* label, Widget* field);
    void addRow(Widget* label, Layout* field);

    void addRow(const ppl7::String& labelText, Widget* field);
    void addRow(const ppl7::String& labelText, Layout* field);
    int rowCount() const;


    virtual int count() const override;
};

} // EOF namespace ppltk

#endif

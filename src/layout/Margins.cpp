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

Margins::Margins()
{
    _left=_right=_top=_bottom=0;
}

Margins::Margins(int left, int top, int right, int bottom)
{
    _left=left;
    _top=top;
    _right=right;
    _bottom=bottom;
}

int Margins::left() const
{
    return _left;
}

int Margins::right() const
{
    return _right;
}

int Margins::top() const
{
    return _top;
}

int Margins::bottom() const
{
    return _bottom;
}

bool Margins::isNull() const
{
    if (_left != 0) return false;
    if (_top != 0) return false;
    if (_right != 0) return false;
    if (_bottom != 0) return false;
    return true;
}

void Margins::set(int left, int top, int right, int bottom)
{
    _left=left;
    _top=top;
    _right=right;
    _bottom=bottom;
}

void Margins::setLeft(int left)
{
    _left=left;
}

void Margins::setRight(int right)
{
    _right=right;
}

void Margins::setTop(int top)
{
    _top=top;
}

void Margins::setBottom(int bottom)
{
    _bottom=bottom;
}

Margins& Margins::operator*=(int factor)
{
    _left=_left * factor;
    _right=_right * factor;
    _top=_top * factor;
    _bottom=_bottom * factor;
    return *this;
}

Margins& Margins::operator*=(float factor)
{
    _left=(int)((float)_left * factor);
    _right=(int)((float)_right * factor);
    _top=(int)((float)_top * factor);
    _bottom=(int)((float)_bottom * factor);
    return *this;
}

Margins& Margins::operator+=(const Margins& other)
{
    _left+=other._left;
    _right+=other._right;
    _top+=other._top;
    _bottom+=other._bottom;
    return *this;
}

Margins& Margins::operator+=(int value)
{
    _left=_left + value;
    _right=_right + value;
    _top=_top + value;
    _bottom=_bottom + value;
    return *this;
}

Margins& Margins::operator-=(const Margins& other)
{
    _left-=other._left;
    _right-=other._right;
    _top-=other._top;
    _bottom-=other._bottom;
    return *this;
}

Margins& Margins::operator-=(int value)
{
    _left=_left - value;
    _right=_right - value;
    _top=_top - value;
    _bottom=_bottom - value;
    return *this;
}

Margins& Margins::operator/=(int divisor)
{
    _left=_left / divisor;
    _right=_right / divisor;
    _top=_top / divisor;
    _bottom=_bottom / divisor;
    return *this;
}

Margins& Margins::operator/=(float divisor)
{
    _left=(int)((float)_left / divisor);
    _right=(int)((float)_right / divisor);
    _top=(int)((float)_top / divisor);
    _bottom=(int)((float)_bottom / divisor);
    return *this;
}

bool operator!=(const Margins& m1, const Margins& m2)
{
    if (m1.left() == m2.left()
        && m1.top() == m2.top()
        && m1.right() == m2.right()
        && m1.bottom() == m2.bottom()) return false;
    return true;
}

bool operator==(const Margins& m1, const Margins& m2)
{
    if (m1.left() == m2.left()
        && m1.top() == m2.top()
        && m1.right() == m2.right()
        && m1.bottom() == m2.bottom()) return true;
    return false;
}


Margins operator*(const Margins& margins, int factor)
{
    return Margins(margins.left() * factor,
        margins.top() * factor,
        margins.right() * factor,
        margins.bottom() * factor);
}

Margins operator*(int factor, const Margins& margins)
{
    return Margins(margins.left() * factor,
        margins.top() * factor,
        margins.right() * factor,
        margins.bottom() * factor);
}

Margins operator*(const Margins& margins, float factor)
{
    return Margins((int)((float)margins.left() * factor),
        (int)((float)margins.top() * factor),
        (int)((float)margins.right() * factor),
        (int)((float)margins.bottom() * factor));
}

Margins operator*(float factor, const Margins& margins)
{
    return Margins((int)((float)margins.left() * factor),
        (int)((float)margins.top() * factor),
        (int)((float)margins.right() * factor),
        (int)((float)margins.bottom() * factor));
}

Margins operator+(const Margins& m1, const Margins& m2)
{
    return Margins(m1.left() + m2.left(),
        m1.top() + m2.top(),
        m1.right() + m2.right(),
        m1.bottom() + m2.bottom()
    );
}

Margins operator+(const Margins& m1, int value)
{
    return Margins(m1.left() + value,
        m1.top() + value,
        m1.right() + value,
        m1.bottom() + value
    );
}

Margins operator+(int value, const Margins& m1)
{
    return Margins(m1.left() + value,
        m1.top() + value,
        m1.right() + value,
        m1.bottom() + value
    );
}

Margins operator-(const Margins& m1, const Margins& m2)
{
    return Margins(m1.left() - m2.left(),
        m1.top() - m2.top(),
        m1.right() - m2.right(),
        m1.bottom() - m2.bottom()
    );
}

Margins operator-(const Margins& m1, int value)
{
    return Margins(m1.left() - value,
        m1.top() - value,
        m1.right() - value,
        m1.bottom() - value
    );
}

Margins operator/(const Margins& m1, int divisor)
{
    return Margins(m1.left() / divisor,
        m1.top() / divisor,
        m1.right() / divisor,
        m1.bottom() / divisor
    );
}

Margins operator/(const Margins& m1, float divisor)
{
    return Margins((int)((float)m1.left() / divisor),
        (int)((float)m1.top() / divisor),
        (int)((float)m1.right() / divisor),
        (int)((float)m1.bottom() / divisor)
    );
}

Margins operator|(const Margins& m1, const Margins& m2)
{
    return Margins(m1.left() > m2.left() ? m1.left() : m2.left(),
        m1.top() > m2.top() ? m1.top() : m2.top(),
        m1.right() > m2.right() ? m1.right() : m2.right(),
        m1.bottom() > m2.bottom() ? m1.bottom() : m2.bottom());
}


}	// EOF namespace ppltk

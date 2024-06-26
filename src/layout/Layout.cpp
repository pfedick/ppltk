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

Layout::Layout(Widget* parent)
{
	myParent = parent;
	if (parent) parent->setLayout(this);
	myMargins.set(0, 0, 0, 0);
	isValid=false;
}

Layout::~Layout()
{

}

Widget* Layout::parent() const
{
	return myParent;
}

void Layout::setContentsMargins(int left, int top, int right, int bottom)
{
	myMargins.set(left, top, right, bottom);
}

void Layout::setContentsMargins(const Margins& margins)
{
	myMargins=margins;
}

const Margins& Layout::contentsMargins() const
{
	return myMargins;
}



void Layout::invalidate()
{
	if (isValid) {
		isValid=false;
		if (myParent) myParent->invalidateLayout();
	}
}

void Layout::recalculate()
{
	update();
	isValid=true;
}



}	// EOF namespace ppltk

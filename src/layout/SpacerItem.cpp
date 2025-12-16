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

SpacerItem::SpacerItem()
{
    width = 40;
    height = 40;
    hPolicy = SizePolicy::Maximum;
    vPolicy = SizePolicy::Maximum;
}

SpacerItem::SpacerItem(int size)
{
    width = size;
    height = size;
    hPolicy = SizePolicy::Fixed;
    vPolicy = SizePolicy::Fixed;
}

SpacerItem::SpacerItem(int width, int height)
{
    this->width = width;
    this->height = height;
    hPolicy = SizePolicy::Fixed;
    vPolicy = SizePolicy::Fixed;
}

SpacerItem::SpacerItem(int width, int height, SizePolicy::Policy hPolicy, SizePolicy::Policy vPolicy)
{
    this->width = width;
    this->height = height;
    this->hPolicy = hPolicy;
    this->vPolicy = vPolicy;
}



} // EOF namespace ppltk
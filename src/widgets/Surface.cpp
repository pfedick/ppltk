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

using namespace ppl7;
using namespace ppl7::grafix;

static void defLock(void* privatedata, Drawable& draw) {};
static void defUnlock(void* privatedata) {};
static void defDestroy(void* privatedata) {};
static void defUpdate(void* privatedata, const Drawable& source) {};

static PRIV_SURFACE_FUNCTIONS defFunctions ={
		defLock,
		defUnlock,
		defDestroy,
		defUpdate };



Surface::Surface()
{
	myFlags=DefaultSurface;
	privatedata=NULL;
	fn=&defFunctions;
	w=0;
	h=0;
}

Surface::~Surface()
{
	fn->destroy(privatedata);
}

const RGBFormat& Surface::rgbFormat() const
{
	return myFormat;
}

Surface::SurfaceFlags Surface::flags() const
{
	return (SurfaceFlags)myFlags;
}

int Surface::width() const
{
	return w;
}

int Surface::height() const
{
	return h;
}

bool Surface::isLoackable() const
{
	if (myFlags & Lockable) return true;
	return false;
}

void Surface::lock(Drawable& draw)
{
	fn->lock(privatedata, draw);
}

void Surface::unlock()
{
	fn->unlock(privatedata);
}

void Surface::setFlags(SurfaceFlags flags)
{
	myFlags=flags;
}

void Surface::setRGBFormat(const RGBFormat& format)
{
	myFormat=format;
}

void Surface::setSize(int width, int height)
{
	w=width;
	h=height;
}

void Surface::setPrivateData(void* data, PRIV_SURFACE_FUNCTIONS* fn)
{
	privatedata=data;
	this->fn=fn;
}

void* Surface::getPrivateData()
{
	return privatedata;
}


}	// EOF namespace ppltk

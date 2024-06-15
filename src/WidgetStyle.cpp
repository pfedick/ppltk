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



WidgetStyle::WidgetStyle(WidgetStyle::PredefinedStyle style)
{
	setStyle(style);
}

void WidgetStyle::setStyle(PredefinedStyle style)
{
	labelFont.setName("Default");
	labelFont.setSize(12);
	labelFont.setAntialias(true);
	buttonFont.setName("Default");
	buttonFont.setSize(12);
	buttonFont.setAntialias(true);
	buttonFont.setBold(true);

	inputFont.setName("Default");
	inputFont.setAntialias(true);
	inputFont.setSize(12);
	inputFont.setBold(true);

	if (style == Light) {
		windowBackgroundColor.setColor(220, 220, 220, 255);
		frameBackgroundColor.setColor(230, 230, 230, 255);
		frameBorderColorLight.setColor(250, 250, 250, 255);
		frameBorderColorShadow.setColor(96, 96, 96, 255);
		labelFontColor.setColor(0, 0, 0, 255);
		buttonBackgroundColor.setColor(230, 230, 230, 255);
		buttonFontColor.setColor(0, 0, 0, 255);
		inputFontColor.setColor(0, 0, 0, 255);
		inputBackgroundColor.setColor(255, 255, 255, 255);
		inputInvalidBackgroundColor.setColor(255, 200, 200, 255);
		inputSelectedBackgroundColor.setColor(196, 196, 255, 255);
		sliderHighlightColor.setColor(61, 174, 233, 255);
		sliderSelectedColor.setColor(61, 61, 55, 255);
		buttonSymbolColor.setColor(26, 16, 16, 255);
		tabNonSelectedBackgroundColor.setColor(200, 200, 200, 255);
		tabDisabledFontColor.setColor(96, 96, 96, 255);
		listBackgroundColor.setColor(240, 240, 240, 255);
		comboBoxBackgroundColor.setColor(235, 235, 238, 255);

	} else {
		windowBackgroundColor.setColor(47, 47, 47, 255);
		frameBackgroundColor.setColor(71, 71, 74, 255);
		frameBorderColorLight.setColor(127, 127, 123, 255);
		frameBorderColorShadow.setColor(81, 86, 88, 255);
		labelFontColor.setColor(216, 216, 214, 255);
		buttonBackgroundColor.setColor(91, 91, 98, 255);
		buttonFontColor.setColor(223, 223, 220, 255);
		inputFontColor.setColor(240, 240, 235, 255);
		inputBackgroundColor.setColor(50, 50, 47, 255);
		inputInvalidBackgroundColor.setColor(80, 40, 40, 255);
		inputSelectedBackgroundColor.setColor(40, 78, 118, 255);
		sliderHighlightColor.setColor(61, 174, 233, 255);
		sliderSelectedColor.setColor(210, 210, 205, 255);
		buttonSymbolColor.setColor(255, 250, 250, 255);
		tabNonSelectedBackgroundColor.setColor(61, 61, 64, 255);
		tabDisabledFontColor.setColor(140, 140, 140, 255);
		listBackgroundColor.setColor(40, 38, 38, 255);
		comboBoxBackgroundColor.setColor(65, 62, 62, 255);

	}
}


}	// EOF namespace ppltk

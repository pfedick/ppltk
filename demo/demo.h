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

#ifndef DEMO_H_
#define DEMO_H_

#include "ppltk.h"

class FrameRate
{
private:
    int desiredFrameRate;
    int fps;
    int framecount;
    ppl7::ppl_time_t lastFpsTime;

public:
    FrameRate();
    void setFrameRate(int fps);
    int getFrameCount() const;
    int getFPS() const;
    void updateFPS();
};

class MainWindow : public ppltk::Window
{
private:
    ppl7::grafix::Grafix* gfx;
    ppltk::WindowManager* wm;
    ppltk::WidgetStyle Style;

    ppltk::Frame* menue;
    ppltk::Label* fpsLabel;
    ppltk::Button* exitButton;


    ppltk::Frame* toolbar;
    ppltk::TextEdit* text;

    ppltk::LineInput* input;

    FrameRate fps;

    virtual void closeEvent(ppltk::Event* event);
    virtual void mouseClickEvent(ppltk::MouseEvent* event);

public:
    MainWindow();
    ~MainWindow();

    void create(int width, int height, bool fullscreen);
    void updateFrameRate();


};

#endif /* DEMO_H_ */

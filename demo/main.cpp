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

#include <stdlib.h>
#include <ppl7.h>
#include <ppl7-grafix.h>
#include "demo.h"

int start(int agrc, char** argv)
{

#ifdef WIN32
    ppl7::String::setGlobalEncoding("UTF-8");
#endif

    if (setlocale(LC_CTYPE, "") == NULL) {
        printf("setlocale fehlgeschlagen\n");
        throw std::exception();
    }
    if (setlocale(LC_NUMERIC, "C") == NULL) {
        printf("setlocale fuer LC_NUMERIC fehlgeschlagen\n");
        throw std::exception();
    }

    ppl7::grafix::Grafix gfx;
    ppltk::WindowManager_SDL2 wm;

    MainWindow win;
    win.create(1280, 720, false);


    //SDL_Renderer *renderer=(SDL_Renderer*)win.getRenderer();
    while (wm.numWindows() > 0) {
        win.updateFrameRate();
        win.clearScreen();
        win.drawWidgets();
        win.presentScreen();

        wm.handleEvents();

    }

    //wm.destroyWindow(MainWindow);

    return 0;
}

MainWindow::MainWindow()
{
    gfx=ppl7::grafix::GetGrafix();
    setWindowTitle("PPL7 Toolkit-Demo");
    setBackgroundColor(ppl7::grafix::Color(0, 0, 0, 0));
    setWindowIcon(gfx->Icons32.getDrawable(30));
    menue=NULL;
    fpsLabel=NULL;
    exitButton=NULL;
    testframe=NULL;
    ppltk::WidgetStyle s(ppltk::WidgetStyle::Dark);
    Style=s;
    wm=ppltk::GetWindowManager();
    wm->setWidgetStyle(Style);

}

MainWindow::~MainWindow()
{

}


void MainWindow::create(int width, int height, bool fullscreen)
{
    setSize(width, height);
    setBackgroundColor(ppl7::grafix::Color(92, 92, 92, 92));
    int flags=ppltk::Window::DefaultWindow;
    flags|=ppltk::Window::Resizeable;

    if (fullscreen) {
        flags=ppltk::Window::DefaultFullscreen;
    }
    setFlags(flags);
    ppltk::GetWindowManager()->createWindow(*this);

    menue=new ppltk::Frame(0, 0, this->width(), 32);
    addChild(menue);
    ppl7::grafix::Size menueSize=menue->clientSize();

    layout_menue=new ppltk::HBoxLayout();
    menue->setLayout(layout_menue);


    testframe=new ppltk::Frame(64, 64, this->width() - 128, this->height() - 92);
    addChild(testframe);

    text=new ppltk::TextEdit(0, 32, testframe->clientSize().width, 300);
    text->setEventHandler(this);
    testframe->addChild(text);



    int x=0;
    ppltk::Label* label=new ppltk::Label(x, 0, 60, menueSize.height, "FPS:");
    layout_menue->addWidget(label);

    fpsLabel=new ppltk::Label("0", ppltk::Label::Inset);
    layout_menue->addWidget(fpsLabel);

    exitButton=new ppltk::Button("Exit", gfx->Toolbar.getDrawable(68));
    exitButton->setEventHandler(this);
    layout_menue->addWidget(exitButton);

}

void MainWindow::closeEvent(ppltk::Event* event)
{
    ppltk::WindowManager* wm=ppltk::GetWindowManager();
    wm->destroyWindow(*this);
    event->accept();
}

void MainWindow::mouseClickEvent(ppltk::MouseEvent* event)
{
    Widget* w=event->widget();
    if (w == exitButton) {
        ppltk::WindowManager* wm=ppltk::GetWindowManager();
        wm->destroyWindow(*this);
        event->accept();
    }
}

void MainWindow::resizeEvent(ppltk::ResizeEvent* event)
{
    testframe->setSize(this->width() - 128, this->height() - 92);
    menue->setWidth(this->width());
    event->accept();

}


void MainWindow::updateFrameRate()
{
    fps.updateFPS();
    fpsLabel->setText(ppl7::ToString("%d", fps.getFPS()));
}



FrameRate::FrameRate()
{
    desiredFrameRate=0;
    fps=0;
    lastFpsTime=ppl7::GetTime();
    framecount=0;
}


void FrameRate::setFrameRate(int fps)
{
    desiredFrameRate=fps;
}



int FrameRate::getFPS() const
{
    return fps;
}

void FrameRate::updateFPS()
{
    framecount++;
    ppl7::ppl_time_t now=ppl7::GetTime();
    if (now > lastFpsTime) {
        lastFpsTime=now;
        fps=framecount;
        framecount=0;
    }
}


#ifdef WIN32
int WinMain()
{
    return start(__argc, __argv);
}
#endif

int main(int argc, char** argv)
{
    return start(argc, argv);
}

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
    layout_menue=NULL;
    tab=NULL;
    testframe=NULL;
    text=NULL;
    smalltext=NULL;
    input=NULL;
    Style.setStyle(ppltk::WidgetStyle::Dark);
    //Style.labelFont.setSize(20);
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

    ppl7::String debugText;
    debugText.set("Dies ist ein Demo-Text, der über mehrere Zeilen geht.\n"
        "Hier die zweite Zeile, danach eine Leerzeile\n\n"
        "Und jetzt kommt ein etwas längerer Text ohne Zeilenumbruch. Dieser muss von selbst umbrechen, "
        "insbesondere auch, wenn das Fenster verkleinert oder vergrößert wird. Mal schauen, wie gut das "
        "klappt, wie man es rendert, und wie man da drin navigieren kann. Ok, das sollte jetzt lang "
        "genug sein.");


    int flags=ppltk::Window::DefaultWindow;
    flags|=ppltk::Window::Resizeable;

    if (fullscreen) {
        flags=ppltk::Window::DefaultFullscreen;
    }
    setFlags(flags);
    ppltk::GetWindowManager()->createWindow(*this);

    menue=new ppltk::Frame(0, 0, this->width(), 32);
    addChild(menue);
    //ppl7::grafix::Size menueSize=menue->clientSize();

    layout_menue=new ppltk::HBoxLayout();
    menue->setLayout(layout_menue);

    tab=new ppltk::TabWidget(64, 64, this->width() - 128, this->height() - 92);
    addChild(tab);

    testframe=new ppltk::Widget();
    //testframe->setTransparent(true);
    //addChild(testframe);
    tab->addTab(testframe, "Text Input", wm->Toolbar.getDrawable(68));

    ppltk::Widget* w=new ppltk::Widget();
    tab->addTab(w, "zweites Widget");
    smalltext=new ppltk::TextEdit(0, 40, 200, 400);
    smalltext->setText(debugText);
    w->addChild(smalltext);


    //tab->setTabEnabled(1, false);
    //tab->setTabVisible(1, false);


    w=new ppltk::Widget();
    tab->addTab(w, "drittes Widget");

    ppltk::Scrollbar *scroll=new ppltk::Scrollbar(0,0,25,500);
    scroll->setSize(100);
    scroll->setVisibleItems(25);
    w->addChild(scroll);

    tab->setCurrentIndex(0);


    text=new ppltk::TextEdit(0, 40, testframe->clientSize().width, testframe->clientSize().height - 40);

    text->setText(debugText);
    text->setEventHandler(this);
    testframe->addChild(text);


    input=new ppltk::LineInput(0, 0, testframe->clientSize().width, 30, "Ein einzeiliger Test Text");
    testframe->addChild(input);


    ppltk::Label* label=new ppltk::Label("FPS:");
    layout_menue->addWidget(label);

    fpsLabel=new ppltk::Label("0", ppltk::Label::Inset);
    layout_menue->addWidget(fpsLabel);

    exitButton=new ppltk::Button("Exit", wm->Toolbar.getDrawable(68));
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
    if (tab) tab->setSize(this->width() - 128, this->height() - 92);
    if (menue) menue->setWidth(this->width());
    //if (testframe) testframe->setSize(tab->clientSize());
    if (text) text->setSize(testframe->clientSize().width, testframe->clientSize().height - 40);
    //text->setSize(200, testframe->clientSize().height - 40);
    if (input) input->setSize(testframe->clientSize().width, 30);
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

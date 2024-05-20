#include <stdlib.h>
#include <ppl7.h>
#include <ppl7-grafix.h>
#include <ppl7-tk.h>
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
    ppl7::tk::WindowManager_SDL2 wm;

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
    toolbar=NULL;
    ppl7::tk::WidgetStyle s(ppl7::tk::WidgetStyle::Dark);
    Style=s;
    wm=ppl7::tk::GetWindowManager();
    wm->setWidgetStyle(Style);

}

MainWindow::~MainWindow()
{

}


void MainWindow::create(int width, int height, bool fullscreen)
{
    setSize(width, height);
    setBackgroundColor(ppl7::grafix::Color(92, 92, 92, 92));
    int flags=ppl7::tk::Window::DefaultWindow;
    if (fullscreen) {
        flags=ppl7::tk::Window::DefaultFullscreen;
    }
    setFlags(flags);
    ppl7::tk::GetWindowManager()->createWindow(*this);

    menue=new ppl7::tk::Frame(0, 0, this->width(), 32);
    addChild(menue);
    ppl7::grafix::Size menueSize=menue->clientSize();

    toolbar=new ppl7::tk::Frame(64, 64, this->width() - 128, this->height() - 92);
    addChild(toolbar);

    int x=0;
    ppl7::tk::Label* label=new ppl7::tk::Label(x, 0, 60, menueSize.height, "FPS:");
    menue->addChild(label);

    fpsLabel=new ppl7::tk::Label(x + 60, 0, 60, menueSize.height, "0");
    fpsLabel->setBorderStyle(ppl7::tk::Label::Inset);
    menue->addChild(fpsLabel);

    exitButton=new ppl7::tk::Button(menueSize.width - menueSize.height, 0, menueSize.height, menueSize.height, "Exit");
    exitButton->setIcon(gfx->Toolbar.getDrawable(68));
    exitButton->setEventHandler(this);
    menue->addChild(exitButton);

}

void MainWindow::closeEvent(ppl7::tk::Event* event)
{
    ppl7::tk::WindowManager* wm=ppl7::tk::GetWindowManager();
    wm->destroyWindow(*this);
    event->accept();
}

void MainWindow::mouseClickEvent(ppl7::tk::MouseEvent* event)
{
    Widget* w=event->widget();
    if (w == exitButton) {
        ppl7::tk::WindowManager* wm=ppl7::tk::GetWindowManager();
        wm->destroyWindow(*this);
        event->accept();
    }
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

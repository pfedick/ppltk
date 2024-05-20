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

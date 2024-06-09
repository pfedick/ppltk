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


DoubleAbstractSlider::DoubleAbstractSlider(int x, int y, int width, int height)
    : ppltk::Widget()
{
    create(x, y, width, height);
    setClientOffset(0, 0, 0, 0);
    min=0.0f;
    max=1.0f;
    current_value=0.0f;
    my_steps=0.1f;
}


void DoubleAbstractSlider::setMinimum(double value)
{
    min=value;
}

void DoubleAbstractSlider::setMaximum(double value)
{
    max=value;
}

void DoubleAbstractSlider::setSteps(double value)
{
    my_steps=value;
}

void DoubleAbstractSlider::setLimits(double min, double max)
{
    this->min=min;
    this->max=max;
}

void DoubleAbstractSlider::setValue(double value)
{
    if (value == current_value) return;
    if (value >= min && value <= max) current_value=value;
    if (value < min) current_value=min;
    if (value > max) current_value=max;
    parentMustRedraw();
    needsRedraw();
    sliderValueChanged(current_value);
}

double DoubleAbstractSlider::value() const
{
    return current_value;
}

double DoubleAbstractSlider::minimum() const
{
    return min;
}

double DoubleAbstractSlider::maximum() const
{
    return max;
}

double DoubleAbstractSlider::steps() const
{
    return my_steps;
}

double DoubleAbstractSlider::stepSize() const
{
    int step=(max - min) / my_steps;
    if (step < 1) step=1;
    return step;
}



DoubleHorizontalSlider::DoubleHorizontalSlider(int x, int y, int width, int height)
    : DoubleAbstractSlider(x, y, width, height)
{
    drag_started=false;
    start_x=0;
    spinbox=NULL;
}

DoubleHorizontalSlider::~DoubleHorizontalSlider()
{
    if (drag_started) {
        drag_started=false;
        ppltk::GetWindowManager()->releaseMouse(this);
    }
}

void DoubleHorizontalSlider::enableSpinBox(bool enabled, double stepsize, int decimals, int width)
{
    if (spinbox) delete spinbox;
    spinbox=NULL;
    start_x=0;
    if (enabled) {
        start_x=width + 10;
        spinbox=new DoubleSpinBox(0, 0, width, height(), value(), decimals);
        spinbox->setLimits(minimum(), maximum());
        spinbox->setStepSize(stepsize);
        spinbox->setValue(value());
        spinbox->setEventHandler(this);
        addChild(spinbox);
    }
    needsRedraw();
}

void DoubleHorizontalSlider::sliderValueChanged(double value)
{
    if (spinbox) spinbox->setValue(value);
}

void DoubleHorizontalSlider::valueChangedEvent(ppltk::Event* event, double value)
{
    //printf("DoubleHorizontalSlider::valueChangedEvent: %0.1f\n", value);
    if (event->widget() == spinbox && spinbox != NULL) {
        if (value != this->value()) {
            setValue(value);
            ppltk::Event ev(ppltk::Event::ValueChanged);
            ev.setWidget(this);
            valueChangedEvent(&ev, value);
        }
    }
    DoubleAbstractSlider::valueChangedEvent(event, value);
}


void DoubleHorizontalSlider::paint(ppl7::grafix::Drawable& draw)
{
    const ppltk::WidgetStyle& style=ppltk::GetWidgetStyle();

    bool has_focus=hasFocus();
    int y1=draw.height() * 1 / 5;
    int y2=draw.height() * 4 / 5;
    int h=y2 - y1;
    int draw_range=draw.width() - h - start_x;
    double slider_range=maximum() - minimum();
    int x1=start_x + (value() - minimum()) * draw_range / slider_range;
    int x2=x1 + h;
    slider_pos.setRect(x1, y1, h, h);


    y1=draw.height() * 2 / 5;
    y2=draw.height() * 3 / 5;

    draw.fillRect(start_x, y1, draw.width(), y2, style.buttonBackgroundColor);
    draw.fillRect(start_x, y1, x1, y2, style.sliderHighlightColor);
    draw.drawRect(start_x, y1, draw.width(), y2, style.frameBorderColorLight);

    y1=draw.height() * 1 / 5;
    y2=draw.height() * 4 / 5;

    if (has_focus) {
        draw.fillRect(x1, y1, x2, y2, style.sliderSelectedColor);
    } else {
        draw.fillRect(x1, y1, x2, y2, style.windowBackgroundColor);
    }
    draw.drawRect(x1, y1, x2, y2, style.frameBorderColorLight);


}

void DoubleHorizontalSlider::mouseDownEvent(ppltk::MouseEvent* event)
{
    if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
        //printf("HorizontalSlider::mouseDownEvent: %d, %d\n", event->p.x, event->p.y);
        if (event->p.inside(slider_pos)) {
            drag_started=true;
            drag_offset=event->p.x - slider_pos.x1 + start_x;
            drag_start_pos=event->p;
            ppltk::GetWindowManager()->grabMouse(this);
        } else if (event->p.x < slider_pos.x1 && event->p.x>start_x) {
            setValue(value() - stepSize());
            ppltk::Event ev(ppltk::Event::ValueChanged);
            ev.setWidget(this);
            valueChangedEvent(&ev, value());
        } else if (event->p.x > slider_pos.x2) {
            setValue(value() + stepSize());
            ppltk::Event ev(ppltk::Event::ValueChanged);
            ev.setWidget(this);
            valueChangedEvent(&ev, value());
        }
    }
}

void DoubleHorizontalSlider::lostFocusEvent(ppltk::FocusEvent* event)
{
    if (drag_started) {
        drag_started=false;
        ppltk::GetWindowManager()->releaseMouse(this);
    }
}

void DoubleHorizontalSlider::mouseUpEvent(ppltk::MouseEvent* event)
{
    if (drag_started) {
        drag_started=false;
        ppltk::GetWindowManager()->releaseMouse(this);
    }
}

void DoubleHorizontalSlider::mouseMoveEvent(ppltk::MouseEvent* event)
{
    if (event->buttonMask & ppltk::MouseEvent::MouseButton::Left) {
        if (drag_started) {
            int y1=height() * 1 / 5;
            int y2=height() * 4 / 5;
            int h=y2 - y1;
            int draw_range=width() - h - start_x;
            double v=minimum() + (event->p.x - drag_offset) * (maximum() - minimum()) / draw_range;
            setValue(v);
            ppltk::Event ev(ppltk::Event::ValueChanged);
            ev.setWidget(this);
            valueChangedEvent(&ev, value());

        }
    } else if (drag_started) {
        drag_started=false;
        ppltk::GetWindowManager()->releaseMouse(this);
    }

}

void DoubleHorizontalSlider::mouseWheelEvent(ppltk::MouseEvent* event)
{
    if (event->wheel.y < 0 && value()>minimum()) {
        setValue(value() - stepSize());
        ppltk::Event ev(ppltk::Event::ValueChanged);
        ev.setWidget(this);
        valueChangedEvent(&ev, value());
    } else if (event->wheel.y > 0 && value() < maximum()) {
        setValue(value() + stepSize());
        ppltk::Event ev(ppltk::Event::ValueChanged);
        ev.setWidget(this);
        valueChangedEvent(&ev, value());
    }
}

}   // EOF namespace

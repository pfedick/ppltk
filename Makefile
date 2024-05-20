CXX		= g++
EXTRA_CFLAGS =
INCLUDE	= -I include
CFLAGS	= -ggdb -O3 -march=native -Wall  $(INCLUDE) $(EXTRA_CFLAGS) `ppl7-config --cflags release`
LIB		= `ppl7-config --libs release`  -lstdc++
LIBDEP	:= $(shell ppl7-config --ppllib release)
PROGRAM	= ppltk


OBJECTS = compile/Button.o \
	compile/checkbox.o \
	compile/combobox.o \
	compile/Divider.o \
	compile/doubleslider.o \
	compile/Event.o \
	compile/Frame.o \
	compile/Label.o \
	compile/Layout.o \
	compile/LineInput.o \
	compile/listwidget.o \
	compile/radiobutton.o \
	compile/scrollbar.o \
	compile/slider.o \
	compile/SpinBox.o \
	compile/Surface.o \
	compile/Widget.o \
	compile/WidgetStyle.o \
	compile/Window.o \
	compile/WindowManager.o \
	compile/WindowManager_SDL2.o


$(PROGRAM): $(OBJECTS) compile/main.o $(LIBDEP)
	$(CC) -o $(PROGRAM) $(OBJECTS) compile/main.o $(CFLAGS) $(LIB)
	-chmod 755 $(PROGRAM)

all: $(PROGRAM)

clean:
	-rm -f *.o $(PROGRAM) *.core compile


compile/main.o: demo/main.cpp Makefile include/ppltk.h demo/demo.h
	- @mkdir -p compile
	$(CXX) -o compile/main.o -c demo/main.cpp $(CFLAGS)

compile/Button.o: src/Button.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Button.o -c src/Button.cpp $(CFLAGS)

compile/checkbox.o: src/checkbox.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/checkbox.o -c src/checkbox.cpp $(CFLAGS)

compile/combobox.o: src/combobox.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/combobox.o -c src/combobox.cpp $(CFLAGS)

compile/Divider.o: src/Divider.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Divider.o -c src/Divider.cpp $(CFLAGS)

compile/doubleslider.o: src/doubleslider.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/doubleslider.o -c src/doubleslider.cpp $(CFLAGS)

compile/Event.o: src/Event.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Event.o -c src/Event.cpp $(CFLAGS)

compile/Frame.o: src/Frame.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Frame.o -c src/Frame.cpp $(CFLAGS)

compile/Label.o: src/Label.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Label.o -c src/Label.cpp $(CFLAGS)

compile/Layout.o: src/Layout.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Layout.o -c src/Layout.cpp $(CFLAGS)

compile/LineInput.o: src/LineInput.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/LineInput.o -c src/LineInput.cpp $(CFLAGS)

compile/listwidget.o: src/listwidget.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/listwidget.o -c src/listwidget.cpp $(CFLAGS)

compile/radiobutton.o: src/radiobutton.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/radiobutton.o -c src/radiobutton.cpp $(CFLAGS)

compile/scrollbar.o: src/scrollbar.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/scrollbar.o -c src/scrollbar.cpp $(CFLAGS)

compile/slider.o: src/slider.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/slider.o -c src/slider.cpp $(CFLAGS)

compile/SpinBox.o: src/SpinBox.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/SpinBox.o -c src/SpinBox.cpp $(CFLAGS)

compile/Surface.o: src/Surface.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Surface.o -c src/Surface.cpp $(CFLAGS)

compile/Widget.o: src/Widget.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Widget.o -c src/Widget.cpp $(CFLAGS)

compile/WidgetStyle.o: src/WidgetStyle.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/WidgetStyle.o -c src/WidgetStyle.cpp $(CFLAGS)

compile/Window.o: src/Window.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/Window.o -c src/Window.cpp $(CFLAGS)

compile/WindowManager.o: src/WindowManager.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/WindowManager.o -c src/WindowManager.cpp $(CFLAGS)

compile/WindowManager_SDL2.o: src/WindowManager_SDL2.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/WindowManager_SDL2.o -c src/WindowManager_SDL2.cpp $(CFLAGS)

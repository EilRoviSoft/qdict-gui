//qt
#include <QApplication>

//dict_gui
#include "dictionary_gui.hpp"

//winapi
#include "Windows.h"

int main(int argc, char* argv[]) {
    SetConsoleCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IONBF, 0);

    QApplication application(argc, argv);
    dict_gui::DictionaryGui widget;
    widget.show();
    return application.exec();
}

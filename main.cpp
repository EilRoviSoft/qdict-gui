//qt
#include <QApplication>

//dict_gui
#include "dictionary_gui.hpp"

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);
    dict_gui::DictionaryGui widget;
    widget.show();
    return application.exec();
}

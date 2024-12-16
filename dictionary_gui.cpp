//dict_gui
#include "dictionary_gui.hpp"

//ui
#include "ui_dictionary_gui.h"

namespace dict_gui {
    DictionaryGui::DictionaryGui(QWidget* parent) :
        QMainWindow(parent),
        ui(new Ui::DictionaryGui) {
        ui->setupUi(this);
    }

    DictionaryGui::~DictionaryGui() {
        delete ui;
    }
}

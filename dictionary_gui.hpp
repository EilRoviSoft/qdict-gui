#pragma once

//qt
#include <QMainWindow>
#include <QStandardItemModel>

//dict_gui
#include "src/database.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
    class DictionaryGui;
}
QT_END_NAMESPACE

namespace dict_gui {
    class DictionaryGui : public QMainWindow {
        Q_OBJECT

    public:
        DictionaryGui(QWidget* parent = nullptr);
        ~DictionaryGui() override;

    private:
        Ui::DictionaryGui* _ui;
        QStandardItemModel _model;
        Database _db;
        int _active_row = -1;

        void _init();
    };
}

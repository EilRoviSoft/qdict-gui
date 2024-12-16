#pragma once

//qt
#include <QMainWindow>

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
        Ui::DictionaryGui* ui;
    };
}

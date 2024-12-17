#include "dictionary_gui.hpp"

//std
#include <iostream>
#include <ranges>
#include <set>

//qt
#include <QStandardItemModel>

//ui
#include "ui_dictionary_gui.h"

std::string vector_to_string(const std::vector<std::string>& content) {
    std::string result;
    for (size_t i = 0; i < content.size() - 1; i++)
        result += content[i] + " ";
    result += content.back();
    return result;
}

std::vector<std::string> split_string(const std::string& str, const char& delim = ' ') {
    std::vector<std::string> result;
    for (auto it : std::views::split(str, delim))
        result.emplace_back(it.data(), it.size());
    return result;
}

void init_model_headers(QStandardItemModel& model, const std::vector<std::string>& headers) {
    for (int i = 0; i < headers.size(); i++) {
        const auto on_push = new QStandardItem;
        on_push->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
        on_push->setText(headers[i].c_str());
        model.setHorizontalHeaderItem(i, on_push);
    }
}
void apply_table_resize_modes(const QTableView& table_view, const std::vector<QHeaderView::ResizeMode>& modes) {
    const int lim = std::min(table_view.width(), (int) modes.size());
    for (int i = 0; i < lim; i++)
        table_view.horizontalHeader()->setSectionResizeMode(i, modes[i]);
}
void apply_table_equal_section_sizes(QTableView& table_view, const int& columns, const int& width) {
    for (int i = 0; i < columns - 1; i++) {
        table_view.horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        table_view.setColumnWidth(i, width / columns);
    }
    table_view.horizontalHeader()->setSectionResizeMode(columns - 1, QHeaderView::Stretch);
}

void emplace_record_to_model(QStandardItemModel& model, const int& idx, const dict_gui::record_ptr& record) {
    model.setItem(idx, 0, new QStandardItem(vector_to_string(record->english).c_str()));
    model.setItem(idx, 1, new QStandardItem(vector_to_string(record->romaji).c_str()));
    model.setItem(idx, 2, new QStandardItem(vector_to_string(record->japanese).c_str()));
}

bool has_element(const QStandardItemModel& model, const int& idx, const QString& element) {
    for (size_t i = 0; i < model.rowCount(); i++)
        if (model.index(i, idx).data().toString() == element)
            return true;
    return false;
}

namespace dict_gui {
    DictionaryGui::DictionaryGui(QWidget* parent) :
        QMainWindow(parent),
        _ui(new Ui::DictionaryGui),
        _model(new QStandardItemModel) {
        _ui->setupUi(this);
        _init();
    }

    DictionaryGui::~DictionaryGui() {
        delete _ui;
    }

    void DictionaryGui::_init() {
        constexpr auto table_width = 694;

        _ui->table_view->setModel(&_model);

        connect(_ui->search_button, &QPushButton::released, this, [this] {
            const size_t size = _model.rowCount();
            std::set<int> show_indexes;

            const auto& english = _ui->line_edit_english->displayText().toStdString();
            const auto& romaji = _ui->line_edit_romaji->displayText().toStdString();
            const auto& japanese = _ui->line_edit_japanese->displayText().toStdString();

            if (!english.empty()) {
                for (int i = 0; i < size; i++) {
                    const auto split = split_string(_model.index(i, 0).data().toString().toStdString());
                    for (const auto& jt : split) {
                        if (jt.contains(english)) {
                            show_indexes.emplace(i);
                            break;
                        }
                    }
                }
            }

            if (!romaji.empty()) {
                for (int i = 0; i < size; i++) {
                    const auto split = split_string(_model.index(i, 1).data().toString().toStdString());
                    for (const auto& jt : split) {
                        if (jt.contains(romaji)) {
                            show_indexes.emplace(i);
                            break;
                        }
                    }
                }
            }

            if (!japanese.empty()) {
                for (int i = 0; i < size; i++) {
                    const auto split = split_string(_model.index(i, 2).data().toString().toStdString());
                    for (const auto& jt : split) {
                        if (jt.contains(japanese)) {
                            show_indexes.emplace(i);
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i < size; i++) {
                if (show_indexes.contains(i)) {
                    if (_ui->table_view->isRowHidden(i))
                        _ui->table_view->showRow(i);
                } else
                    _ui->table_view->hideRow(i);
            }
        });
        connect(_ui->clear_button, &QPushButton::released, this, [this] {
            _ui->table_view->clearSelection();
            _active_row = -1;

            _ui->line_edit_english->setText("");
            _ui->line_edit_romaji->setText("");
            _ui->line_edit_japanese->setText("");

            for (int i = 0; i < _model.rowCount(); i++) {
                if (_ui->table_view->isRowHidden(i))
                    _ui->table_view->showRow(i);
            }
        });
        connect(_ui->insert_button, &QPushButton::released, this, [this] {
            const auto& english = _ui->line_edit_english->displayText();
            const auto& romaji = _ui->line_edit_romaji->displayText();
            const auto& japanese = _ui->line_edit_japanese->displayText();

            if (english == nullptr || romaji == nullptr || japanese == nullptr ||
                (has_element(_model, 0, english) && has_element(_model, 1, romaji) && has_element(_model, 2, japanese)))
                return;

            const auto record = std::make_shared<record_t>(
                split_string(english.toStdString()),
                split_string(romaji.toStdString()),
                split_string(japanese.toStdString())
            );

            emplace_record_to_model(_model, _model.rowCount(), record);
        });
        connect(_ui->remove_button, &QPushButton::released, this, [this] {
            if (_active_row == -1)
                return;

            _model.removeRow(_active_row);

            _ui->table_view->clearSelection();
            _active_row = -1;
        });
        connect(_ui->edit_button, &QPushButton::released, this, [this] {
            if (_active_row == -1)
                return;

            const auto& english = _ui->line_edit_english->displayText();
            const auto& romaji = _ui->line_edit_romaji->displayText();
            const auto& japanese = _ui->line_edit_japanese->displayText();

            if (english == nullptr || romaji == nullptr || japanese == nullptr)
                return;

            _model.setItem(_active_row, 0, new QStandardItem(english));
            _model.setItem(_active_row, 1, new QStandardItem(romaji));
            _model.setItem(_active_row, 2, new QStandardItem(japanese));
        });
        connect(_ui->load_button, &QPushButton::released, this, [this] {
            _model.clear();
            init_model_headers(_model, {
                "English", "Romaji", "Japanese"
            });

            _db.load("data.json");

            const size_t db_size = _db.size();
            for (int i = 0; i < db_size; i++)
                emplace_record_to_model(_model, i, _db[i]);

            apply_table_equal_section_sizes(*_ui->table_view, _model.columnCount(), table_width);

            _db.clear();
        });
        connect(_ui->save_button, &QPushButton::released, this, [this] {
            const auto rows = _model.rowCount();

            for (int i = 0; i < rows; i++) {
                _db.emplace(std::make_shared<record_t>(
                    split_string(_model.index(i, 0).data().toString().toStdString()),
                    split_string(_model.index(i, 1).data().toString().toStdString()),
                    split_string(_model.index(i, 2).data().toString().toStdString())
                ));
            }

            _db.save("data.json");
            _db.clear();
        });

        connect(_ui->table_view, &QAbstractItemView::clicked, this, [this](const QModelIndex& index) {
            _active_row = index.row();
            _ui->line_edit_english->setText(_model.index(_active_row, 0).data().toString());
            _ui->line_edit_romaji->setText(_model.index(_active_row, 1).data().toString());
            _ui->line_edit_japanese->setText(_model.index(_active_row, 2).data().toString());
        });
    }
}

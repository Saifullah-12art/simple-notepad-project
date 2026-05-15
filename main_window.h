#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "spell_checker.h"
#include "spell_checker_highlighter.h"
#include "text_transform.h"

#include <QDialog>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QString>
#include <QTextDocument>
#include <QTextEdit>
#include <memory>
#include <vector>

namespace Ui {
    class find_replace_dialog;
    class word_frequency_dialog;
}

class main_window : public QMainWindow {
    Q_OBJECT

public:
    main_window();
    ~main_window() override;

private:
    void setup_file_menu();
    void setup_edit_menu();
    void setup_search_menu();
    void setup_format_menu();
    void setup_tools_menu();
    void setup_format_toolbar();
    void setup_status_bar();

    void open_file();
    void open_file_path(const QString& path);
    void save_file();
    void save_file_as();
    void update_title();
    void update_status_bar();
    void update_recent_files_menu();
    void add_to_recent_files(const QString& path);

    void apply_transform(const text_transform& transform) const;

    void show_find_replace_dialog();
    void find_next(const QString& term, QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_current(const QString& term, const QString& replacement,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;
    void replace_all(const QString& term, const QString& replacement,
        QTextDocument::FindFlags flags = QTextDocument::FindFlags()) const;

    void show_word_frequency();

    QTextEdit* editor { nullptr };
    QString current_file;
    std::vector<std::unique_ptr<text_transform>> transforms;

    QDialog* find_replace_dlg { nullptr };
    std::unique_ptr<Ui::find_replace_dialog> find_replace_ui;

    spell_checker* checker { nullptr };
    spell_checker_highlighter* spell_highlighter { nullptr };

    QMenu* recent_files_menu { nullptr };
    QSettings settings;
};

#endif // MAIN_WINDOW_H
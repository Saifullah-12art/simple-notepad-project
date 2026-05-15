#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>
#include <vector>

class syntax_highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit syntax_highlighter(QTextDocument* parent)
        : QSyntaxHighlighter(parent)
    {
        QTextCharFormat keyword_format;
        keyword_format.setForeground(QColor(86, 156, 214));
        keyword_format.setFontWeight(QFont::Bold);

        const QStringList keywords = {
            "alignas", "alignof", "and", "and_eq", "asm", "auto",
            "bitand", "bitor", "bool", "break", "case", "catch",
            "char", "char8_t", "char16_t", "char32_t", "class",
            "compl", "concept", "const", "consteval", "constexpr",
            "constinit", "const_cast", "continue", "co_await",
            "co_return", "co_yield", "decltype", "default", "delete",
            "do", "double", "dynamic_cast", "else", "enum", "explicit",
            "export", "extern", "false", "float", "for", "friend",
            "goto", "if", "inline", "int", "long", "mutable",
            "namespace", "new", "noexcept", "not", "not_eq", "nullptr",
            "operator", "or", "or_eq", "private", "protected", "public",
            "register", "reinterpret_cast", "requires", "return",
            "short", "signed", "sizeof", "static", "static_assert",
            "static_cast", "struct", "switch", "template", "this",
            "thread_local", "throw", "true", "try", "typedef", "typeid",
            "typename", "union", "unsigned", "using", "virtual", "void",
            "volatile", "wchar_t", "while", "xor", "xor_eq"
        };

        for (const auto& keyword : keywords) {
            rules.push_back({ QRegularExpression("\\b" + keyword + "\\b"), keyword_format });
        }

        QTextCharFormat string_format;
        string_format.setForeground(QColor(206, 145, 120));
        rules.push_back({ QRegularExpression("\"[^\"]*\""), string_format });
        rules.push_back({ QRegularExpression("'[^']*'"), string_format });

        QTextCharFormat comment_format;
        comment_format.setForeground(QColor(106, 153, 85));
        rules.push_back({ QRegularExpression("//[^\n]*"), comment_format });

        QTextCharFormat number_format;
        number_format.setForeground(QColor(181, 206, 168));
        rules.push_back({ QRegularExpression("\\b[0-9]+(\\.[0-9]+)?\\b"), number_format });

        QTextCharFormat preprocessor_format;
        preprocessor_format.setForeground(QColor(155, 155, 155));
        rules.push_back({ QRegularExpression("^\\s*#[^\n]*"), preprocessor_format });
    }

    void highlightBlock(const QString& text) override
    {
        for (const auto& [pattern, format] : rules) {
            auto it = pattern.globalMatch(text);
            while (it.hasNext()) {
                const auto match = it.next();
                setFormat(match.capturedStart(), match.capturedLength(), format);
            }
        }
    }

private:
    struct highlight_rule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    std::vector<highlight_rule> rules;
};

#endif // SYNTAX_HIGHLIGHTER_H
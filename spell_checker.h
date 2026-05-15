#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <QFile>
#include <QTextStream>
#include <set>
#include <string>
#include <vector>

class spell_checker {
public:
    explicit spell_checker(const QString& word_list_path)
    {
        QFile file(word_list_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }
        QTextStream in(&file);
        while (!in.atEnd()) {
            const auto line = in.readLine().trimmed().toLower().toStdString();
            if (!line.empty()) {
                words.insert(line);
            }
        }
    }

    [[nodiscard]] bool is_correct(const QString& word) const
    {
        if (word.isEmpty()) {
            return true;
        }
        const std::string lower = word.toLower().toStdString();
        return words.count(lower) > 0;
    }

    [[nodiscard]] std::vector<QString> suggestions(const QString& word) const
    {
        const std::string lower = word.toLower().toStdString();
        std::vector<QString> result;
        for (const auto& w : words) {
            if (result.size() >= 5) {
                break;
            }
            if (!w.empty() && !lower.empty()
                && w[0] == lower[0]
                && std::abs(static_cast<int>(w.size()) - static_cast<int>(lower.size())) <= 3
                && edit_distance(lower, w) <= 2) {
                result.push_back(QString::fromStdString(w));
            }
        }
        return result;
    }

private:
    std::set<std::string> words;

    [[nodiscard]] static int edit_distance(const std::string& a, const std::string& b)
    {
        const int m = static_cast<int>(a.size());
        const int n = static_cast<int>(b.size());
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        for (int i = 0; i <= m; ++i) {
            dp[i][0] = i;
        }
        for (int j = 0; j <= n; ++j) {
            dp[0][j] = j;
        }
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (a[i - 1] == b[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
                }
            }
        }
        return dp[m][n];
    }
};

#endif // SPELL_CHECKER_H
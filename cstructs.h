#ifndef CSTRUCTS_H
#define CSTRUCTS_H
#include <QStringList>

struct SWord {
    qint32 id;
    QString text;
    QList<qint8> availableDisBlock;
    qint8 currentDisBlock { -1 };
    QList<qint8> availablePositions;
    qint8 currentPosition { -1 };

    bool isCorrectPosition() { return availablePositions.contains(currentPosition); }
    bool isCorrectDisBlock() { return availableDisBlock.contains(currentDisBlock); }
    bool isCorrect() { return isCorrectPosition() && isCorrectDisBlock(); }
};

struct SExample {
    QStringList combinations; // {"1-2-3.4-5-6.7-8.9", "1-5-2.7-3-4.8-4.9", "2-1-4.3-5-4.5-6.9"};
    QList<SWord *> words;

    bool isCorrect()
    {
        for (auto word : words) {
            if (!word->isCorrect())
                return false;
        }
        return true;
    }
};

//! Разделяет ключи id слов в блоке
const auto SEPARATOR = QStringLiteral("-");
//! Разделяет блоки в комбинации
const auto BLOCK_SEPARATOR = QStringLiteral(".");

enum class EBlockState { none, correct, incorrect, noDifference };

const qint32 BLOCK_HEIGHT = 20;
const qint32 BLOCK_WIDTH = 20;
const qint32 XMARGIN_BETWEEN_CELLS = 5;
const qint32 YMARGIN_BETWEEN_CELLS = 3;
//! Единица размера строчки по горизонтали
const qint32 ONE_WIDTH = BLOCK_WIDTH + XMARGIN_BETWEEN_CELLS;
//! Единица размера строчки по вертикали
const qint32 ONE_HEIGHT = BLOCK_HEIGHT + YMARGIN_BETWEEN_CELLS;
const qint32 SCENE_HEIGHT = 650;
const qint32 SCENE_WIDTH = 950;

const QStringList DIS_BLOCK { "Основное заболевание:", "Фоновое заболевание:", "Осложнения основного заболевания:",
                              "Сопутствующие заболевания:" };

#endif // CSTRUCTS_H

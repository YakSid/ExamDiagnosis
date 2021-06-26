#ifndef CSTRUCTS_H
#define CSTRUCTS_H
#include <QStringList>

struct SBlock {
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
    QList<SBlock> blocks;

    bool isCorrect()
    {
        for (auto block : blocks) {
            if (!block.isCorrect())
                return false;
        }
        return true;
    }
};

#endif // CSTRUCTS_H

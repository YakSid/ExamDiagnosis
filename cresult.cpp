#include "cresult.h"
#include "ui_cresult.h"

CResult::CResult(QWidget *parent) : QDialog(parent), ui(new Ui::CResult)
{
    ui->setupUi(this);
}

CResult::~CResult()
{
    delete ui;
}

void CResult::init(const SExample &example)
{
    QString result;

    m_example = example;
    QString comb = example.combinations.first();
    auto blocks = comb.split(BLOCK_SEPARATOR);
    qint8 blockCounter = 0;
    for (auto block : blocks) {
        //Указываем заголовок
        switch (blockCounter) {
        case 0:
            result += "Основное заболевание:\n";
            break;
        case 1:
            result += "Фоновое заболевание:\n";
            break;
        case 2:
            result += "Осложнения основного заболевания:\n";
            break;
        case 3:
            result += "Сопутствующие заболевания:\n";
            break;
        default:
            break;
        }
        blockCounter++;
        //Записываем слова
        auto keysInBlock = block.split(SEPARATOR);
        for (auto key : keysInBlock) {
            auto keyInt = key.toInt();
            if (keyInt != 0)
                result += _findWordById(keyInt) + ". ";
        }
        result.chop(1);
        result += "\n\n";
    }

    result.chop(2);
    ui->te_text->setText(result);

    //Форматирование блоков
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setFontItalic(true);

    for (int i = 0; i < 4; i++) {
        QString header;
        if (i == 0) {
            header = "Основное заболевание:";
        } else if (i == 1) {
            header = "Фоновое заболевание:";
        } else if (i == 2) {
            header = "Осложнения основного заболевания:";
        } else if (i == 3) {
            header = "Сопутствующие заболевания:";
        }
        auto posStart = result.indexOf(header);
        auto posEnd = posStart + header.count();
        auto textCursor = ui->te_text->textCursor();
        textCursor.setPosition(posStart, QTextCursor::MoveAnchor);
        textCursor.setPosition(posEnd, QTextCursor::KeepAnchor);
        textCursor.setCharFormat(format);
    }
}

QString CResult::_findWordById(qint32 id)
{
    for (auto word : m_example.words) {
        if (word->id == id)
            return word->text;
    }
}

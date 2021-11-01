#include "cjsonmanager.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

void CJsonManager::saveToFile(QString filename, const SExample *example)
{
    auto jDoc = new QJsonDocument();
    auto jObjInsideDoc = jDoc->object();

    QJsonObject jObj;
    QJsonArray jWords;
    for (auto word : example->words) {
        auto jWord = new QJsonObject();
        jWord->insert("id", word->id);
        jWord->insert("text", word->text);
        //! availableDisBlock
        QJsonArray jADB;
        for (auto adb : word->availableDisBlock) {
            jADB.append(adb);
        }
        jWord->insert("availableDisBlock", jADB);
        // current-переменные не записываю в json
        //! availablePosition
        QJsonArray jAP;
        for (auto ap : word->availablePositions) {
            jAP.append(ap);
        }
        jWord->insert("availablePosition", jAP);

        jWords.append(*jWord);
    }
    jObj.insert("words", jWords);
    //Список комбинаций в виде строк
    QJsonArray jCombinations;
    for (auto combination : example->combinations) {
        jCombinations.append(combination);
    }
    jObj.insert("combinations", jCombinations);

    jObjInsideDoc["obj"] = jObj;
    jDoc->setObject(jObjInsideDoc);

    //Запись в файл
    QFile jFile(filename);
    if (!jFile.open(QIODevice::WriteOnly))
        return;
    // Добавляю "4" в начало после кодирования в base64, чтобы перебор кодировок в декодере не выдал base64
    jFile.write("4" + jDoc->toJson().toBase64());
    jFile.close();
}

SExample CJsonManager::loadFromFile(QString filename)
{
    QFile jFile(filename);
    jFile.open(QFile::ReadOnly);
    auto bytes = jFile.readAll();
    bytes.remove(0, 1); //см. note в функции saveToFile
    auto jDoc = QJsonDocument().fromJson(QByteArray::fromBase64(bytes));
    jFile.close();

    SExample example;
    QList<SWord *> words;

    QJsonObject jObjInsideDoc = jDoc.object();
    QJsonObject jObj = jObjInsideDoc["obj"].toObject();
    QJsonArray jWords = jObj["words"].toArray();
    for (auto jRefWord : jWords) {
        auto jWord = jRefWord.toObject();
        auto word = new SWord();
        word->id = jWord["id"].toInt();
        word->text = jWord["text"].toString();
        QJsonArray jADB = jWord["availableDisBlock"].toArray();
        for (auto jRef : jADB) {
            word->availableDisBlock.append(static_cast<qint8>(jRef.toInt()));
        }
        word->currentDisBlock = -1;
        QJsonArray jAP = jWord["availablePosition"].toArray();
        for (auto jRef : jAP) {
            word->availablePositions.append(static_cast<qint8>(jRef.toInt()));
        }
        word->currentPosition = -1;
        words.append(word);
    }
    example.words = words;

    QStringList combinations;
    QJsonArray jCombinations = jObj["combinations"].toArray();
    for (auto jRefComb : jCombinations) {
        combinations.append(jRefComb.toString());
    }
    example.combinations = combinations;

    return example;
}

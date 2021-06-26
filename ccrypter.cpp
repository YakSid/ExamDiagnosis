#include "ccrypter.h"

QString CCrypter::crypt(const QString &toCrypt)
{
    QString resultString = "";

    for (int i = 0; i < toCrypt.length(); i++) {
        resultString.append(QString(QChar(toCrypt[i]).unicode() ^ QChar(kEncryptorString[keyIndex(i)]).unicode()));
    }
    return resultString;
}

QString CCrypter::decrypt(const QString &toDecrypt)
{
    QString resultString = "";
    for (int i = 0; i < toDecrypt.length(); i++) {
        resultString.append(QString(QChar(toDecrypt[i]).unicode() ^ QChar(kEncryptorString[keyIndex(i)]).unicode()));
    }
    return resultString;
}

int CCrypter::keyIndex(int index)
{
    int len = kEncryptorString.length();
    int multiple = index / len;

    if (index >= len) {
        return index - (len * multiple);
    }
    return index;
}

#ifndef CLASS_CRYPTER_H
#define CLASS_CRYPTER_H

#include <QCryptographicHash>
#include <QObject>

static const QString kEncryptorString = QString("EXAM DIAGNO"); // here is key to crypt/decrypt QString

class CCrypter : public QObject
{
    Q_OBJECT
public:
    static QString crypt(const QString &toCrypt);
    static QString decrypt(const QString &toDecrypt);

private:
    CCrypter(QObject *parent = nullptr);

    static int keyIndex(int index);
};

#endif // CLASS_CRYPTER_H

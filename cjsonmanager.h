#ifndef CJSONMANAGER_H
#define CJSONMANAGER_H

#include <QObject>
#include "cstructs.h"
#include "ccrypter.h"

class CJsonManager : public QObject
{
    Q_OBJECT
public:
    static void saveToFile(QString filename, const SExample *example);
    static SExample loadFromFile(QString filename);

private:
    CJsonManager(QObject *parent = nullptr);
};

#endif // CJSONMANAGER_H

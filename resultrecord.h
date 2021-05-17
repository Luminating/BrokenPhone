#ifndef RESULTRECORD_H
#define RESULTRECORD_H
#include <QString>
#include <QImage>

class ResultRecord
{
public:
    ResultRecord();

    int gameStep;
    int playerID;
    QString message;
    QImage image;
};

#endif // RESULTRECORD_H

#ifndef PLAYER_H
#define PLAYER_H
#include <QString>

class Player
{
public:
    Player();

    int id = 0;
    QString username;
    QString usercode;
    QString userIP;    
    QString userstatus;
    QString roomUsername;
};

#endif // PLAYER_H

//
// Created by BreizhHardware on 30/04/2024.
//

#ifndef TOWER_H
#define TOWER_H
#include <QObject>
#include <QPointF>
#include <vector>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "Enemy.h"
#include "Game.h"

class Enemy;

class Game;

class Tower : public QObject
{
    QOBJECT_H
protected:
    int damage;
    int fireRate;
    int range;
    int level;
    int cost;
    QPointF position;
    std::string avatarPath;
    QGraphicsPixmapItem* graphics{};
    Game& game;
    QTimer* fireTimer;

public:
    Tower(int damage, int fireRate, int range, int level, int cost, QPointF position, std::string avatarPath, Game& game);
    virtual void upgrade() = 0;
    virtual ~Tower() = default;
    QGraphicsPixmapItem* getGraphics();
    void fireAtClosest(Enemy* target = nullptr) const;
    Enemy* getClosestEnemyInRange(const std::vector<Enemy*>& enemies);

public slots:
    void fire();
};

class LaserTower : public Tower {
public:
    explicit LaserTower(QPointF position);
    void upgrade() override;
};

class BalisticTower : public Tower {
public:
    explicit BalisticTower(QPointF position);
    void upgrade() override;
};

class DistorionTower : public Tower {
public:
    explicit DistorionTower(QPointF position);
    void upgrade() override;
};



#endif //TOWER_H
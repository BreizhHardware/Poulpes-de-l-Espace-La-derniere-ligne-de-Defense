//
// Created by BreizhHardware on 30/04/2024.
//

#include <memory_resource>
#include "Tower.h"


Tower::Tower(int damage, double fireRate, int range, int level, int cost, QPointF position,
             std::string avatarPath, Game& game) : game(game) {
    this->damage = damage;
    this->fireRate = fireRate;
    this->range = range;
    this->level = level;
    this->cost = cost;
    this->position = position;
    this->avatarPath = avatarPath;
    fireTimer = new QTimer();
    connect(fireTimer, &QTimer::timeout, this, &Tower::fire);
    fireTimer->start(fireRate * 1000);
    int xTile = position.x() * 50;
    int yTile = position.y() * 50;
    /*
    rangeIndicator = new QGraphicsEllipseItem(xTile - range * 10, yTile - range * 10, range * 25, range * 25, this);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(0, 0, 255, 25)); // Opacité de 10%
    rangeIndicator->setBrush(brush);
    rangeIndicator->setZValue(5);
    QGraphicsScene* scene = game.scene();
    if(scene != nullptr) {
        scene->addItem(rangeIndicator);
    }
    */
}

void Tower::fireAtClosest(Enemy* target) const {
    if (target == nullptr) {
        return;
    }
    target->takeDamage(damage);
}

Enemy* Tower::getClosestEnemyInRange(const QVector<Enemy*>& enemies) {
    Enemy* closestEnemy = nullptr;
    // Draw a circle around the tower
    double minDistance = range / 2;

    for (Enemy* enemy : enemies) {
        double distance = sqrt(pow(enemy->getX() - position.x(), 2) + pow(enemy->getY() - position.y(), 2));
        if (distance < minDistance) {
            minDistance = distance;
            closestEnemy = enemy;
        }
    }
    return closestEnemy;
}

void Tower::fire() {
    if (&game == nullptr) {
        return;
    }
    QVector<Enemy*>& enemies = game.currentEnemies;
    if(enemies.isEmpty()) {
        return;
    }
    Enemy* target = getClosestEnemyInRange(enemies);
    if(target == nullptr) {
        return;
    }
    auto* projectile = new Projectile(position, target->getPosition());
    projectiles.push_back(projectile);
    game.scene()->addItem(projectile);
    fireAtClosest(target);
}

void Tower::upgradeDamage(){
    if(damageUpgrades < 5){
        damage *= 1.25;
        damageUpgrades++;
    }
    else{
        return;
    }
}

void Tower::upgradeFireRate() {
    if(fireRateUpgrades < 5){
        fireRate *= 0.9;
        fireTimer->setInterval(fireRate * 1000);
        fireRateUpgrades++;
    }
    else{
        return;
    }
}

LaserTower::LaserTower(QPointF position, Game& game) : Tower(50, 1, 10, 1, 50, position,
                                ":/ressources/Tower/Laser_Tower.png", game) {
    QPixmap pixmap(QString::fromStdString(avatarPath));
    // Check if the pixmap is null
    if(pixmap.isNull()) {
    } else {
        // Get x and y from the position
        int x = position.x();
        int y = position.y();
        graphics = new QGraphicsPixmapItem();
        QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Scale the pixmap to 50x50 pixels
        graphics->setPixmap(scaledPixmap);
        graphics->setPos(x * 50, y * 50);
        graphics->setZValue(2);
    }
}

QGraphicsPixmapItem* Tower::getGraphics() {
    return graphics;
}

BalisticTower::BalisticTower(QPointF position, Game& game) : Tower(150, 2, 6, 1, 100, position,
                                    ":/ressources/Tower/Balistic_Tower.png", game) {
    QPixmap pixmap(QString::fromStdString(avatarPath));
    // Check if the pixmap is null
    if(pixmap.isNull()) {
    } else {
        // Get x and y from the position
        int x = position.x();
        int y = position.y();
        graphics = new QGraphicsPixmapItem();
        QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Scale the pixmap to 50x50 pixels
        graphics->setPixmap(scaledPixmap);
        graphics->setPos(x * 50, y * 50);
        graphics->setZValue(2);
    }
}

DistorionTower::DistorionTower(QPointF position, Game& game) : Tower(100, 1, 7, 1, 75, position,
                                        ":/ressources/Tower/Distortion_Tower.png", game) {
    QPixmap pixmap(QString::fromStdString(avatarPath));
    // Check if the pixmap is null
    if(pixmap.isNull()) {
    } else {
        // Get x and y from the position
        int x = position.x();
        int y = position.y();
        graphics = new QGraphicsPixmapItem();
        QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Scale the pixmap to 50x50 pixels
        graphics->setPixmap(scaledPixmap);
        graphics->setPos(x * 50, y * 50);
        graphics->setZValue(2);
    }
}

int Tower::getDamageUpgrades() const {
    return damageUpgrades;
}

int Tower::getFireRateUpgrades() const {
    return fireRateUpgrades;
}

QGraphicsEllipseItem* Tower::getRangeIndicator() {
    return rangeIndicator;
}
//
// Created by breizhhardware on 26/04/24.
//

#include "Player.h"
#include <iostream>

Player::Player(int health, int shield, int damage, int regenerationRate, int speed, const std::string& avatarPath, int x, int y, Map& gameMap, Game& game)
        : Mob(health, shield, damage, regenerationRate, speed, avatarPath, x, y), gameMap(gameMap), game(game) {
    this->x = x;
    this->y = y;
    QPixmap pixmap(QString::fromStdString(avatarPath));
    if (pixmap.isNull()) {
        std::cerr << "Failed to load image from path: " << avatarPath << std::endl;
    } else {
        graphics = new QGraphicsPixmapItem();
        QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Scale the pixmap to 50x50 pixels
        graphics->setPixmap(scaledPixmap);
        graphics->setPos(x * 50, y * 50);
        graphics->setZValue(3); // Set the Z-value to 1 to draw the player on top of the map tiles
    }
}

void Player::setPosition(Tile* tile) {
    this->x = tile->gridX();
    this->y = tile->gridY();
    for (Enemy* enemy : game.currentEnemies) {
        if (enemy->getX() == x && enemy->getY() == y) {
            // If there is an enemy, call the touchEnemy method
            touchEnemy(enemy);
            if(health <= 0) {
                return;
            }
            break;
        }
    }
}

void Player::setPosition(int x, int y) {
    int mapWidth = gameMap.getWidth();
    int mapHeight = gameMap.getHeight();
    for (Enemy* enemy : game.currentEnemies) {
        if (enemy->getX() == x && enemy->getY() == y) {
            // If there is an enemy, call the touchEnemy method
            touchEnemy(enemy);
            if(health <= 0) {
                return;
            }
            break;
        }
    }
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        this->x = x;
        this->y = y;
        graphics->setPos(x * 50, y * 50);
    }
}

void Player::getPlayerPosition() const{
    std::cout << "Player position: x: " << x << " y: " << y << std::endl;
}

QGraphicsPixmapItem* Player::getGraphics() const{
    return graphics;
}

void Player::touchEnemy(Enemy* enemy) {
    // Subtract the enemy's damage from the player's health
    try {
        takeDamage(enemy->getDamage());
    } catch (PlayerDeadException& e) {
        health = 0;
        return;
    }
    // Add the enemy's coin drop to the player's gold
    game.userGold += enemy->getCoinDrop();

    // Remove the enemy from the game
    gameMap.removeItem(enemy->getGraphics());
    auto it = std::find(game.currentEnemies.begin(), game.currentEnemies.end(), enemy);
    if (it != game.currentEnemies.end()) {
        game.currentEnemies.erase(it);
    }
    delete enemy;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        // Game over
        game.gameOver();
        health = 0;
        throw PlayerDeadException();
    }
}

void Player::updatePreviousHealth() {
    previousHealth = health;
}

int Player::getPreviousHealth() const {
    return previousHealth;
}

void Player::heal(int amount) {
    health += amount;
}

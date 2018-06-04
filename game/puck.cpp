#include "puck.h"

Puck::Puck()
{

}

Puck::Puck(qreal radius, Qt::BrushStyle style, Qt::GlobalColor color, qreal x, qreal y)
{
    this->velocity = new VectorXY();
    this->acceleration = new VectorXY();
    this->radius=radius;
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setRect(x,y,2*radius,2*radius);
    this->setBrush(this->brush);

    this->puckStrikerSound = new QMediaPlayer();
    this->puckStrikerSound->setMedia(QUrl("qrc:/soundEffects/striker_puck.wav"));

    this->puckWallSound = new QMediaPlayer();
    this->puckWallSound->setMedia(QUrl("qrc:/soundEffects/wall_puck.wav"));

}

void Puck::setColor(Qt::GlobalColor color)
{
    this->brush.setColor(color);
    this->setBrush(this->brush);
    return;
}

Puck::~Puck()
{
    delete this->velocity;
    delete this->acceleration;
}

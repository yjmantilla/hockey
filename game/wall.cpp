#include "wall.h"

Wall::Wall()
{

}

Wall::Wall(qreal x1, qreal y1, qreal x2, qreal y2)
{
    this->setLine(x1,y1,x2,y2);
}

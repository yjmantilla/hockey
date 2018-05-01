#include "game.h"

Game::Game()
{

}

void Disk::move()
{
    this->x = this->x + this->vx + (0.5)*this->ax;
    this->y = this->y + this->vy + (0.5)*this->ay;
}

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>


class Disk
{
public:

    //properties
    float radius;
    int color;
    float mass;

    //kinematics
    int x;
    int y;
    int vx;
    int vy;
    int ax;
    int ay;

    //methods
    Disk();
    Disk(float radius, int color, float mass);
    void move();
    void bounce(float restitution);
}

class Striker
{
public:
    //properties
    float height;
    float width;
    int color;
    float mass;
    float restitution;

    //kinematics
    int x;
    int y;

    //methods

}

class Field
{
public:
    //geometry
    float height;
    float width;

    //properties
    float viscosity;//kinematic viscosity or friction?
    int color;
    float restitution; //of walls

}

class Goal
{
    float width;
}

class RandomBox
{
public:
    //position
    int x;
    int y;

    int color;
    float height;
    float width;
}

class Player
{
    std::string name;
    int score;
}


class Match
{
public:

    Player player1;
    Player player2;

    Match();
};

class User
{
public:
    std::string name;
    std::string password;
    int wins;
    int loses;
}

class MatchHistory
{
public:
    std::vector<Match> Matches;
}
#endif // GAME_H

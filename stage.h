#pragma once

#include <list>

/**
 * StageExtra class
 *
 * This auxiliary class is used to avoid passing many parameters to
 * constructors when creating stage objects. It encapsulates extra
 * data needed for specific object types.
 *
 * BALL OBJECT:
 *   ex1 = size (0=max, 3=min)
 *   ex2 = top value
 *   ex3 = dirX
 *   ex4 = dirY
 *   ex5 = ball type
 *
 * FLOOR OBJECT:
 *   ex1 = floor type
 */
class StageExtra
{
public:
    bool use;
    int ex1, ex2, ex3, ex4, ex5;

    StageExtra() : use(false), ex1(0), ex2(0), ex3(0), ex4(0), ex5(0) {}
};

/**
 * StageObject struct
 * Stores information about an object: position, ID, and when it appears.
 */
struct StageObject
{
    int id;
    int start; // start time
    int x, y;
    StageExtra extra;

    StageObject(int id = 0, int start = 0)
        : id(id), start(start), x(0), y(0)
    {
        extra.use = false;
    }

    StageObject(int id, int start, StageExtra extra)
        : id(id), start(start), x(0), y(0), extra(extra)
    {
        this->extra.use = true;
    }
};

/**
 * Stage class
 *
 * This class stores all the information specific to each stage:
 * screen layout, background image, music, time limit, and the
 * list of objects to be spawned.
 */
class Stage
{
public:
    int id;
    char back[64];
    char music[64];
    int timelimit;
    int itemsleft;
    int xpos[2]; // initial positions for players 1 and 2
                   
private:
    std::list<StageObject*> sequence;

public:
    Stage() : id(0), timelimit(0), itemsleft(0) {
        xpos[0] = xpos[1] = 0;
        back[0] = '\0';
        music[0] = '\0';
    }

    void reset();
    void setBack(const char* backFile);
    void setMusic(const char* musicFile);
    void add(int idObject, int start);
    void addX(int idObject, int start, StageExtra extra);
    void add(int idObject, int x, int y, int start);
    void addX(int idObject, int x, int y, int start, StageExtra extra);

    StageObject pop(int time);
};
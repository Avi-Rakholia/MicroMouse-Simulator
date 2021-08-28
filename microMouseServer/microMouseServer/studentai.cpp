#include "micromouseserver.h"


//Relative direction with assigned calling value
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3


//Function to change coordinal position relative to direction mouse points
void transpose(int dir, int *x, int*y)
{
    //parameter = current_direction. Checks which case from ENUM then accordingly updates coordinates
    switch(dir)
    {
        case NORTH:
            (*y)++;
        break;
        case EAST:
            (*x)++;
        break;
        case SOUTH:
            (*y)--;
        break;
        case WEST:
            (*x)--;
    }
}

void microMouseServer::studentAI()
{

/*
 * The following are the eight functions that you can call. Feel free to create your own fuctions as well.
 * Remember that any solution that calls moveForward more than once per call of studentAI() will have points deducted.
 *
 *The following functions return if there is a wall in their respective directions
 *bool isWallLeft();
 *bool isWallRight();
 *bool isWallForward();
 *
 *The following functions move the mouse. Move forward returns if the mouse was able to move forward and can be used for error checking
 *bool moveForward();
 *void turnLeft();
 *void turnRight();
 *
 * The following functions are called when you need to output something to the UI or when you have finished the maze
 * void foundFinish();
 * void printUI(const char *mesg);
*/

    /* ----- Right-Hand Rule Solution (WORKS) -------

    //Track amount of lefts (solution case)
    static int count = 0;

    //Right hand method - moving forward
    if (isWallRight() && !isWallForward())
    {
        moveForward();
        count = 0;
    }
    //Right hand method - turning right
    else if (!isWallRight())
    {
        turnRight();
        moveForward();
        count = 0;
    }
    //Right hand method - in case mouse in corner turn around
    else if (isWallRight() && isWallForward() && isWallLeft())
    {
        turnLeft();
        turnLeft();
        count = 0;
    }
    //Right hand method - track amount of times solution case is used
    else if (isWallRight() && isWallForward())
    {
        turnLeft();
        moveForward();
        count = count + 1;
    }
    //Right hand method - if solution case used three times, maze solved
    if (count == 3)
    {
        foundFinish();
    }

    */


    //Creates 2d array (20 rows 20 columns)
    const int y_vals = 20;
    const int x_vals = 20;
    static unsigned int pos[x_vals][y_vals] = {{0}};
    //Current Direction from ENUM set according to value: North (0), East (1), South (2), West (3)
    static int current_direction = 0;
    //Current row and column
    static int x = 0;
    static int y = 0;
    //Track number of times for a turn
    int timesL = 0;
    int timesR = 0;
    int timesF = 0;
    //Track number of left turns (3 for found)
    static int found_count = 0;

    //Track number of times certain turn completed for every position
    switch(current_direction)
    {
    //Different values because of direction facing
        case NORTH:
            timesL = pos[x-1][y];
            timesR = pos[x+1][y];
            timesF = pos[x][y+1];
        break;
        case EAST:
            timesL = pos[x][y+1];
            timesR = pos[x][y-1];
            timesF = pos[x+1][y];
        break;
        case SOUTH:
            timesL = pos[x+1][y];
            timesR = pos[x-1][y];
            timesF = pos[x][y-1];
        break;
        case WEST:
            timesL = pos[x][y-1];
            timesR = pos[x][y+1];
            timesF = pos[x+1][y];
    }
    //Set each tracker to large number so to make sure number is never reached
    if (isWallRight())
        timesR = 10000000;
    if (isWallLeft())
        timesL = 10000000;
    if (isWallForward())
        timesF = 10000000;

    //Right hand method - turn right case
    if (!isWallRight() && timesR <= timesL && timesR <= timesF)
    {
        //Turn right then increment amount of times turned right in that position
        turnRight();
        timesR++;
        //Add 1 to current direction after turning then mod 4 to get remainder (current direction for current position and not the next position)
        current_direction = (current_direction+1) % 4;
        //Pass direction, x, and y to find position in 2d array
        transpose(current_direction, &x, &y);
        //Make sure found_count isn't incremented
        found_count = 0;
        //Add 1 to current position (not changing position) to mark that mouse has been there
        pos[x][y] += 1;
    }
    //Right hand method - move forward case
    else if (!isWallForward() && timesF <= timesL && timesF <= timesR)
    {
        //moveForward();
        //Increment amount of times moved forward in that position
        timesF++;
        //Pass direction, x, and y (& for pointer address) to find position in 2d array
        transpose(current_direction, &x, &y);
        //Add 1 to current position (not changing the position) to mark that it's been there and make sure moving forward doesn't increment found counter
        pos[x][y] += 1;
        found_count = 0;
    }
    //Right hand method - turn left case
    else if (!isWallLeft() && timesL <= timesR && timesL <= timesF)
    {
        //Turn left then increment amount of times turned left in that position
        turnLeft();
        timesL++;
        //Find current direction by subtracting one (since left moves opposite) and set to three if case reaches negative (West to North)
        current_direction = (current_direction-1) %4;
        if (current_direction < 0)
        {
            current_direction = 3;
        }
        //Find position, mark the position, and increment found (right-hand rule means solution will be three lefts)
        transpose(current_direction, &x, &y);
        found_count++;
        pos[x][y] += 1;
    }
    //Right hand method - turn around case (dead-end reached)
    else
    {
        //Turn right to turn around (no need to increment values for a dead-end)
        turnRight();
        turnRight();
        //Move direction twice since mouse faces forward to backwards (opposite)
        current_direction= (current_direction+2) %4;
        //Find current position in 2d array, don't increment found, and mark position
        transpose(current_direction, &x, &y);
        found_count = 0;
        pos[x][y] += 1;
    }
    if(!moveForward())
        printUI("oops");

    //Check if Maze solved (three left turns consecutively)
    if (found_count == 3)
    {
        //Output function foundFinish() if maze solved
        foundFinish();
    }
}

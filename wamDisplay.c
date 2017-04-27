/*
 * wamDisplay.c
 *
 *  Created on: Nov 28, 2016
 *      Author: hornej2
 */

#include "supportFiles/display.h"
#include "wamDisplay.h"
#include "wamControl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../switches_buttons_lab/switches.h"
#include "../interval_timers/buttons.h"

#define DISPLAY_HALF_WIDTH DISPLAY_WIDTH/2 //half width of display
#define DISPLAY_HALF_HEIGHT DISPLAY_HEIGHT/2 //half heigh of display
#define DISPLAY_SIXTH_HEIGHT DISPLAY_HEIGHT/6 //one sixth the display height
#define DISPLAY_SIXTH_WIDTH DISPLAY_WIDTH/6 //one sixth the display width
#define DISPLAY_FIFTH_EIGHTH_HEIGHT DISPLAY_HEIGHT/8*5 //5/8ths the display height
#define DISPLAY_FIFTH_EIGHTH_WIDTH DISPLAY_WIDTH/8*5 //5/8ths the display width
#define MAX_STR 255 //allows for a large string length
#define SWITCH_VALUE_9 9  // Binary 9 on the switches indicates 9 moles.
#define SWITCH_VALUE_6 6  // Binary 6 on the switches indicates 6 moles.
#define SWITCH_VALUE_4 4  // Binary 9 on the switches indicates 4 moles.
#define SWITCH_MASK 0xf   // Ignore potentially extraneous bits.
#define HOLE_RADIUS 30 //radius of holes
#define LEFT_HOLE_WIDTH 40 //left hole x coordinate
#define CENTER_HOLE_WIDTH DISPLAY_WIDTH/2 //center hole x coordinate
#define RIGHT_HOLE_WIDTH DISPLAY_WIDTH-40 //right hole x coordinate
#define TOP_HOLE_HEIGHT 40 //top hole y coordinate
#define MIDDLE_HOLE_HEIGHT DISPLAY_HEIGHT/2-10 //middle hole y coordinate
#define BOTTOM_HOLE_HEIGHT DISPLAY_HEIGHT-60 //bottom hole y coordinate
#define FOUR_MOLES 4 //4 moles
#define SIX_MOLES 6 //6 moles
#define NINE_MOLES 9 //9 moles
#define BIG_TEXT 3 //size of large text
#define SMALL_TEXT 2 //size of small text
#define MOLE_1 1 //mole one
#define MOLE_2 2 //mole two
#define MOLE_3 3 //mole three
#define MOLE_4 4 //mole four
#define MOLE_5 5 //mole five
#define MOLE_6 6 //mole six
#define MOLE_7 7 //mole seven
#define MOLE_8 8 //mole eight
#define MOLE_9 9 //mole nine
#define MOLE_1_INDEX 0 //index of mole one
#define MOLE_2_INDEX 1 //index of mole two
#define MOLE_3_INDEX 2 //index of mole three
#define MOLE_4_INDEX 3 //index of mole four
#define MOLE_5_INDEX 4 //index of mole five
#define MOLE_6_INDEX 5 //index of mole six
#define MOLE_7_INDEX 6 //index of mole seven
#define MOLE_8_INDEX 7 //index of mole eight
#define MOLE_9_INDEX 8 //index of mole nine
#define TOP_ROW_MIN_Y 10 //top edge of top row holes
#define TOP_ROW_MAX_Y 70 //bottom edge of top row holes
#define MIDDLE_ROW_MIN_Y 80 //top edge of middle row holes
#define MIDDLE_ROW_MAX_Y 140 //bottom edge of middle row holes
#define BOTTOM_ROW_MIN_Y 150 //top edge of bottom row holes
#define BOTTOM_ROW_MAX_Y 210 //bottom edge of bottom row holes
#define LEFT_COLUMN_MIN_X 10 //left edge of left column holes
#define LEFT_COLUMN_MAX_X 70 //right edge of left column holes
#define MIDDLE_COLUMN_MIN_X 130 //left edge of middle column holes
#define MIDDLE_COLUMN_MAX_X 190 //right edge of middle column holes
#define RIGHT_COLUMN_MIN_X 250 //left edge of right column holes
#define RIGHT_COLUMN_MAX_X 310 //right edge of right column holes
#define WHACK_A_MOLE_TEXT_X DISPLAY_SIXTH_WIDTH-20 //set x cursor for "whack a mole" text
#define WHACK_A_MOLE_TEXT_Y DISPLAY_HALF_HEIGHT-30 //set y cursor for "whack a mole" text
#define TOUCH_SCREEN_TO_START_TEXT_X DISPLAY_SIXTH_WIDTH-30 //set x cursor for "touch screen to start" text
#define TOUCH_SCREEN_TO_START_TEXT_Y DISPLAY_HALF_HEIGHT //set y cursor for "touch screen to start" text
#define RESET 0 //resets the variables
#define RESET_ACTIVE_MOLES 1 //start each level with one active mole
#define TOP_LEFT_EDGE 0 //top left corner of board
#define GREEN_BOX_HEIGHT DISPLAY_HEIGHT-20 //green background height
#define HITS_Y DISPLAY_HEIGHT-15 //y value of hits cursor
#define HITS_BLACK_RECT_X 60 //x cursor of rectangle to clear hits number
#define HITS_BLACK_RECT_Y DISPLAY_HEIGHT-20 //y cursor of rectangle to clear hits number
#define MISSES_X DISPLAY_SIXTH_WIDTH+40 //x value of misses cursor
#define MISSES_Y DISPLAY_HEIGHT-15 //y value of misses cursor
#define MISSES_BLACK_RECT_X DISPLAY_SIXTH_WIDTH+40+70 //x cursor of rectangle to clear misses number
#define MISSES_BLACK_RECT_Y DISPLAY_HEIGHT-20 //y cursor of rectangle to clear misses number
#define LEVEL_X DISPLAY_SIXTH_WIDTH+160 //x value of level cursor
#define LEVEL_Y DISPLAY_HEIGHT-15 //y value of level cursor
#define LEVEL_BLACK_RECT_X DISPLAY_SIXTH_WIDTH+160+60 //x cursor of rectangle to clear level number
#define LEVEL_BLACK_RECT_Y DISPLAY_HEIGHT-20 //y cursor of rectangle to clear misses number
#define BLACK_RECT_WIDTH 40 //width of black rectangle to clear updating numbers
#define BLACK_RECT_HEIGHT 20 //height of black rectangle to clear updating numbers
#define GAME_OVER_TEXT_X DISPLAY_SIXTH_WIDTH+20 //x value of game over cursor
#define GAME_OVER_TEXT_Y DISPLAY_HALF_HEIGHT-40 //y value of game over cursor
#define FINAL_HITS_TEXT_X DISPLAY_SIXTH_WIDTH+60 //x value of final hits cursor
#define FINAL_HITS_TEXT_Y DISPLAY_HALF_HEIGHT //y value of final hits cursor
#define FINAL_MISSES_TEXT_X DISPLAY_SIXTH_WIDTH+40 //x value of final misses cursor
#define FINAL_MISSES_TEXT_Y DISPLAY_HALF_HEIGHT+20 //y value of final misses cursor
#define FINAL_LEVEL_TEXT_X DISPLAY_SIXTH_WIDTH+20 //x value of final level cursor
#define FINAL_LEVEL_TEXT_Y DISPLAY_HALF_HEIGHT+40 //y value of final level cursor
#define TRY_AGAIN_TEXT_X DISPLAY_SIXTH_WIDTH-20 //x value of try again cursor
#define TRY_AGAIN_TEXT_Y DISPLAY_HALF_HEIGHT+80 //y value of try again cursor
#define ZERO 0 //used for checking and initializing values
#define LEVEL_CHANGE 5 //level increases every 5
#define ONE 1 //used for adjusting addresses, etc

uint32_t hitScore = ZERO; //init hitScore
uint32_t missScore = ZERO; //init missScore
uint32_t level = ZERO; //init level
char str[MAX_STR]; //str used for printing text to board
uint32_t moles = ZERO; //init number of moles
uint32_t activeMoles; //keeps track of active moles
wamDisplay_point_t moleCoordinate; //keeps track of what coordinate the moles are at

/********************** typedefs **********************/
// This keeps track of all mole information.
typedef struct {
    wamDisplay_point_t origin;  // This is the origin of the hole for this mole.
    // A mole is active if either of the tick counts are non-zero. The mole is dormant otherwise.
    // During operation, non-zero tick counts are decremented at a regular rate by the control state machine.
    // The mole remains in his hole until ticksUntilAwake decrements to zero and then he pops out.
    // The mole remains popped out of his hole until ticksUntilDormant decrements to zero.
    // Once ticksUntilDomant goes to zero, the mole hides in his hole and remains dormant until activated again.
    wamDisplay_moleTickCount_t ticksUntilAwake;  // Mole will wake up (pop out of hole) when this goes from 1 -> 0.
    wamDisplay_moleTickCount_t ticksUntilDormant; // Mole will go dormant (back in hole) this goes 1 -> 0.
} wamDisplay_moleInfo_t;

// This will contain pointers to all of the mole info records.
// This will ultimately be treated as an array of pointers.
static wamDisplay_moleInfo_t** wamDisplay_moleInfo;

void drawMole(wamDisplay_point_t origin, uint8_t erase){
    uint32_t color = DISPLAY_RED; //draw mole
    if (erase){ //if erase is true
        color = DISPLAY_BLACK; //erase the mole hole black
    }
    display_fillCircle(origin.x,origin.y,HOLE_RADIUS,color); //draw circle
}
uint16_t getTouchCoordinates(uint16_t x, uint16_t y){
    if (moles == FOUR_MOLES){ //if the board has four moles
        if (y > TOP_ROW_MIN_Y && y < TOP_ROW_MAX_Y){ //the touch was in the top row
            if (x > LEFT_COLUMN_MIN_X && x < LEFT_COLUMN_MAX_X){ //top left mole
                return MOLE_1; //return mole 1
            }
            else if (x < RIGHT_COLUMN_MAX_X && x > RIGHT_COLUMN_MIN_X){ //top right mole
                return MOLE_2; //return mole 2
            }
        }
        else if (y < BOTTOM_ROW_MAX_Y && y > BOTTOM_ROW_MIN_Y){ //the touch was in the bottom row
            if (x > LEFT_COLUMN_MIN_X && x < LEFT_COLUMN_MAX_X){ //bottom left mole
                return MOLE_3; //return mole 3
            }
            else if (x < RIGHT_COLUMN_MAX_X && x > RIGHT_COLUMN_MIN_X){ //bottom right mole
                return MOLE_4; //return mole 4
            }
        }
    }
    else if (moles == SIX_MOLES){ //if the board has 6 moles
        if (y > TOP_ROW_MIN_Y && y < TOP_ROW_MAX_Y){ //the touch was in the top row
            if (x > LEFT_COLUMN_MIN_X && x < LEFT_COLUMN_MAX_X){ //top left mole
                return MOLE_1; //return mole 1
            }
            else if (x < RIGHT_COLUMN_MAX_X && x > RIGHT_COLUMN_MIN_X){ //top right mole
                return MOLE_3; //return mole 3
            }
            else if (x < MIDDLE_COLUMN_MAX_X && x > MIDDLE_COLUMN_MIN_X){ //top middle mole
                return MOLE_2; //return mole 2
            }
        }
        else if (y < BOTTOM_ROW_MAX_Y && y > BOTTOM_ROW_MIN_Y){ //the touch was in the bottom row
            if (x > LEFT_COLUMN_MIN_X && x < LEFT_COLUMN_MAX_X){ //bottom left mole
                return MOLE_4; //return mole 4
            }
            else if (x < RIGHT_COLUMN_MAX_X && x > RIGHT_COLUMN_MIN_X){ //bottom right mole
                return MOLE_6; //return mole 6
            }
            else if (x < MIDDLE_COLUMN_MAX_X && x > MIDDLE_COLUMN_MIN_X){ //bottom middle mole
                return MOLE_5; //return mole 5
            }
        }
    }
    else { //if the board isnt 4 or 6 it is 9
        if (y > TOP_ROW_MIN_Y && y < TOP_ROW_MAX_Y){ //the touch was in the top row
            if (x > LEFT_COLUMN_MIN_X && x < LEFT_COLUMN_MAX_X){ //top left mole
                return MOLE_1; //return mole 1
            }
            else if (x < RIGHT_COLUMN_MAX_X && x > RIGHT_COLUMN_MIN_X){ //top right mole
                return MOLE_3; //return mole 3
            }
            else if (x < MIDDLE_COLUMN_MAX_X && x > MIDDLE_COLUMN_MIN_X){ //top middle mole
                return MOLE_2; //return mole 2
            }
        }
        else if (y < BOTTOM_ROW_MAX_Y && y > BOTTOM_ROW_MIN_Y){ //the touch was in the bottom row
            if (x > LEFT_COLUMN_MIN_X && x < LEFT_COLUMN_MAX_X){ //bottom left mole
                return MOLE_7; //return mole 7
            }
            else if (x < RIGHT_COLUMN_MAX_X && x > RIGHT_COLUMN_MIN_X){ //bottom right mole
                return MOLE_9; //return mole 9
            }
            else if (x < MIDDLE_COLUMN_MAX_X && x > MIDDLE_COLUMN_MIN_X){ //bottom middle mole
                return MOLE_8; //return mole 8
            }
        }
        else if (y < MIDDLE_ROW_MAX_Y && y > MIDDLE_ROW_MIN_Y){ //the touch was in the middle row
            if (x > LEFT_COLUMN_MIN_X && x < LEFT_COLUMN_MAX_X){ //middle left mole
                return MOLE_4; //return mole 4
            }
            else if (x < RIGHT_COLUMN_MAX_X && x > RIGHT_COLUMN_MIN_X){ //middle right mole
                return MOLE_6; //return mole 6
            }
            else if (x < MIDDLE_COLUMN_MAX_X && x > MIDDLE_COLUMN_MIN_X){ //middle middle mole
                return MOLE_5; //return mole 5
            }
        }
    }
    return false; //if the touch was not on a hole then return false
}

//gets the coordinates of the mole depending on the index. The mole coordinates will change based on the board
//because the indices will be from 0-3 ,0-5, and 0-8 based on number of moles.
void getMoleCoordinates(uint32_t index){
    if(moles == FOUR_MOLES){ //the board has 4 moles
        switch(index){
        case MOLE_1_INDEX: //set index 0 coordinates. top left mole
            moleCoordinate.x = LEFT_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_2_INDEX: //set index 1 coordinates. top right mole
            moleCoordinate.x = RIGHT_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_3_INDEX: //set index 2 coordinates. bottom left mole
            moleCoordinate.x = LEFT_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        case MOLE_4_INDEX: //set index 3 coordinates. bottom right mole
            moleCoordinate.x = RIGHT_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        }
    }
    else if(moles == SIX_MOLES){ //the board has 6 moles
        switch(index){
        case MOLE_1_INDEX: //set index 0 coordinates. top left mole
            moleCoordinate.x = LEFT_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_2_INDEX: //set index 1 coordinates. top middle mole
            moleCoordinate.x = CENTER_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_3_INDEX: //set index 2 coordinates. top right mole
            moleCoordinate.x = RIGHT_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_4_INDEX: //set index 3 coordinates. bottom left mole
            moleCoordinate.x = LEFT_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        case MOLE_5_INDEX: //set index 4 coordinates. bottom middle mole
            moleCoordinate.x = CENTER_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        case MOLE_6_INDEX: //set index 5 coordinates. bottom right mole
            moleCoordinate.x = RIGHT_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        }

    }
    else { //moles == NINE_MOLES
        switch(index){
        case MOLE_1_INDEX: //set index 0 coordinates. top left mole
            moleCoordinate.x = LEFT_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_2_INDEX: //set index 1 coordinates. top middle mole
            moleCoordinate.x = CENTER_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_3_INDEX: //set index 2 coordinates. top right mole
            moleCoordinate.x = RIGHT_HOLE_WIDTH;
            moleCoordinate.y = TOP_HOLE_HEIGHT;
            break;
        case MOLE_4_INDEX: //set index 3 coordinates. middle left mole
            moleCoordinate.x = LEFT_HOLE_WIDTH;
            moleCoordinate.y = MIDDLE_HOLE_HEIGHT;
            break;
        case MOLE_5_INDEX: //set index 4 coordinates. middle middle mole
            moleCoordinate.x = CENTER_HOLE_WIDTH;
            moleCoordinate.y = MIDDLE_HOLE_HEIGHT;
            break;
        case MOLE_6_INDEX: //set index 5 coordinates. middle right mole
            moleCoordinate.x = RIGHT_HOLE_WIDTH;
            moleCoordinate.y = MIDDLE_HOLE_HEIGHT;
            break;
        case MOLE_7_INDEX: //set index 6 coordinates. bottom left mole
            moleCoordinate.x = LEFT_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        case MOLE_8_INDEX: //set index 7 coordinates. bottom middle mole
            moleCoordinate.x = CENTER_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        case MOLE_9_INDEX: //set index 8 coordinates. bottom right mole
            moleCoordinate.x = RIGHT_HOLE_WIDTH;
            moleCoordinate.y = BOTTOM_HOLE_HEIGHT;
            break;
        }
    }
}
// Allocates the memory for wamDisplay_moleInfo_t records.
// Computes the origin for each mole assuming a simple row-column layout:
// 9 moles: 3 rows, 3 columns, 6 moles: 2 rows, 3 columns, 4 moles: 2 rows, 2 columns
// Also inits the tick counts for awake and dormant.
void wamDisplay_computeMoleInfo() {
    // Setup all of the moles, creates and inits mole info records.
    // Create the container array. It contains pointers to each of the mole-hole info records.
    wamDisplay_moleInfo = (wamDisplay_moleInfo_t**) malloc(moles * sizeof(wamDisplay_moleInfo_t*));
    //allocate memory for each myStruct.
    for (uint16_t i=ZERO; i<moles; i++) {
        // Second, allocate an instance of myStruct and point to it.
        wamDisplay_moleInfo[i] = (wamDisplay_moleInfo_t*) malloc(sizeof(wamDisplay_moleInfo_t));
    }
    //assign values to these structs.
    for (uint16_t j=ZERO; j<moles; j++) {
        getMoleCoordinates(j);
        wamDisplay_moleInfo[j]->origin = moleCoordinate;  // Just need a number.
        wamDisplay_moleInfo[j]->ticksUntilAwake = RESET; //initialize
        wamDisplay_moleInfo[j]->ticksUntilDormant = RESET; //initialize
    }
}

// Provide support to set games with varying numbers of moles. This function
// would be called prior to calling wamDisplay_init();
void wamDisplay_selectMoleCount(wamDisplay_moleCount_e moleCount){
    if(moleCount == wamDisplay_moleCount_9){ //if the mole count is 9
        moles = NINE_MOLES; //set the number of moles to 9
    }
    if(moleCount == wamDisplay_moleCount_6){ //if the mole count is 6
        moles = SIX_MOLES; //set the number of moles to 6
    }
    if(moleCount == wamDisplay_moleCount_4){ //if the mole count is 4
        moles = FOUR_MOLES; //set the number of moles to 4
    }
    wamDisplay_computeMoleInfo(); //compute and update the mole info
}

// Call this before using any wamDisplay_ functions.
void wamDisplay_init(){
    //display already initialized
}

// Draw the game display with a background and mole holes.
void wamDisplay_drawMoleBoard(){
    //fill the background green
    display_fillRect(TOP_LEFT_EDGE,TOP_LEFT_EDGE,DISPLAY_WIDTH,GREEN_BOX_HEIGHT,DISPLAY_GREEN);
    if(moles == FOUR_MOLES){ //the board has 4 moles
        display_fillCircle(LEFT_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top left hole
        display_fillCircle(LEFT_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom left hole
        display_fillCircle(RIGHT_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top right hole
        display_fillCircle(RIGHT_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom right hole
        //draw the 4 holes in black
    }
    else if(moles == SIX_MOLES){ //the board has 6 moles
        display_fillCircle(LEFT_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top left hole
        display_fillCircle(LEFT_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom left hole
        display_fillCircle(CENTER_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top center hole
        display_fillCircle(CENTER_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom center hole
        display_fillCircle(RIGHT_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top right hole
        display_fillCircle(RIGHT_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom right hole
        //draw the 6 holes in black
    }
    else { //moles == 9
        display_fillCircle(LEFT_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top left hole
        display_fillCircle(LEFT_HOLE_WIDTH,MIDDLE_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //middle left hole
        display_fillCircle(LEFT_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom left hole
        display_fillCircle(CENTER_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top center hole
        display_fillCircle(CENTER_HOLE_WIDTH,MIDDLE_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //middle center hole
        display_fillCircle(CENTER_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom center hole
        display_fillCircle(RIGHT_HOLE_WIDTH,TOP_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //top right hole
        display_fillCircle(RIGHT_HOLE_WIDTH,MIDDLE_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //middle right hole
        display_fillCircle(RIGHT_HOLE_WIDTH,BOTTOM_HOLE_HEIGHT,HOLE_RADIUS,DISPLAY_BLACK); //bottom right hole
        //draw the 9 holes in black
    }
    wamDisplay_drawScoreScreen(); //draw the score screen at the bottom
}

// Draw the initial splash (instruction) screen.
void wamDisplay_drawSplashScreen(){
    display_fillScreen(DISPLAY_BLACK); //fill screen black
    display_setCursor(WHACK_A_MOLE_TEXT_X,WHACK_A_MOLE_TEXT_Y); //set whack a mole text cursor
    display_setTextColor(DISPLAY_WHITE); //text will be white
    display_setTextSize(BIG_TEXT); //message will be larger text
    display_println("Whack a Mole!"); //print message
    display_setCursor(TOUCH_SCREEN_TO_START_TEXT_X,TOUCH_SCREEN_TO_START_TEXT_Y); //set touch screen to start text cursor
    display_setTextSize(SMALL_TEXT); //message will be smaller text
    display_println("Touch Screen To Start"); //print message
}

// Draw the game-over screen.
void wamDisplay_drawGameOverScreen(){
    display_fillScreen(DISPLAY_BLACK);
    //Game over
    display_setCursor(GAME_OVER_TEXT_X,GAME_OVER_TEXT_Y); //set cursor
    display_setTextColor(DISPLAY_WHITE); //text will be white
    display_setTextSize(BIG_TEXT); //message will have larger text
    display_println("Game Over"); //print message
    //Hits
    display_setCursor(FINAL_HITS_TEXT_X,FINAL_HITS_TEXT_Y); //set cursor
    display_setTextSize(SMALL_TEXT); //message will have smaller text
    sprintf(str, "Hits:%d", hitScore); //set the hitscore to str
    display_println(str);  // Print it to the LCD.
    //Misses
    display_setCursor(FINAL_MISSES_TEXT_X,FINAL_MISSES_TEXT_Y); //set cursor
    sprintf(str, "Misses:%d", missScore); //set missscore to str
    display_println(str);  // Print it to the LCD.
    //Level
    display_setCursor(FINAL_LEVEL_TEXT_X,FINAL_LEVEL_TEXT_Y); //set cursor
    sprintf(str, "Final Level:%d", level); //set level to str
    display_println(str);  // Print it to the LCD.
    //Try again
    display_setCursor(TRY_AGAIN_TEXT_X,TRY_AGAIN_TEXT_Y); //set cursor
    display_println("(Touch to Try Again)"); //print message
}

// Selects a random mole and activates it.
// Activating a mole means that the ticksUntilAwake and ticksUntilDormant counts are initialized.
// See the comments for wamDisplay_moleInfo_t for details.
// Returns true if a mole was successfully activated. False otherwise. You can
// use the return value for error checking as this function should always be successful
// unless you have a bug somewhere.
bool wamDisplay_activateRandomMole(){
    uint8_t index = rand()%(moles); //generate random index
    //if the mole is inactive
    if (wamDisplay_moleInfo[index]->ticksUntilAwake == ZERO && wamDisplay_moleInfo[index]->ticksUntilDormant == ZERO){
        wamDisplay_moleInfo[index]->ticksUntilAwake = wamControl_getRandomMoleAsleepInterval(); //set random ticks until awake
        wamDisplay_moleInfo[index]->ticksUntilDormant = wamControl_getRandomMoleAwakeInterval(); //set random ticks until dormant
        activeMoles++; //increase active mole count
        return true; //it worked!
    }
    else {
        return wamDisplay_activateRandomMole(); //recursive. keeps going to activate a non active mole
    }
    return false; //didnt work
}

// This takes the provided coordinates and attempts to whack a mole. If a
// mole is successfully whacked, all internal data structures are updated and
// the display and score is updated. You can only whack a mole if the mole is awake (visible).
// The return value can be used during testing (you could just print which mole is
// whacked without having to implement the entire game).
//This function will erase an active mole if the coordinates of the “whack” match the coordinates of an active mole.
wamDisplay_moleIndex_t wamDisplay_whackMole(wamDisplay_point_t* whackOrigin){
    int16_t index = getTouchCoordinates(whackOrigin->x, whackOrigin->y)-ONE; //set index to be the hole touched
    if(index < ZERO) //if no hole was touched
        return false; //youve got bad aim
    //if the mole is still awake and succesfully whacked
    if (wamDisplay_moleInfo[index]->ticksUntilAwake == ZERO && wamDisplay_moleInfo[index]->ticksUntilDormant != ZERO){
        wamDisplay_moleInfo[index]->ticksUntilDormant = ZERO; //mole goes back in hole
        drawMole(wamDisplay_moleInfo[index]->origin,true); //mole disappears
        wamDisplay_setHitScore(hitScore+ONE); //increase hitScore
        activeMoles--; //one mole down. decrease active moles
        wamDisplay_drawScoreScreen(); //redraw the new score
        if (wamDisplay_getHitScore()!=ZERO && (wamDisplay_getHitScore()%LEVEL_CHANGE == ZERO)){ //every 5 hits, increase level
            wamDisplay_incrementLevel(); //increase the difficulty
        }
        return index; //return the index of the mole
    }
}

// This updates the ticksUntilAwake/ticksUntilDormant clocks for all of the moles.
//Once the mole tick-counts have been updated, this function will either draw the mole
//(the mole has awakened) or erase the mole (the mole has become inactive).
void wamDisplay_updateAllMoleTickCounts(){
    for(uint16_t i = ZERO; i < moles; i++){ //parse through all the moles
        if(wamDisplay_moleInfo[i]->ticksUntilAwake > ZERO){ //if it still has ticks until awake
            wamDisplay_moleInfo[i]->ticksUntilAwake--; //decrement
            if(wamDisplay_moleInfo[i]->ticksUntilAwake == ZERO){ //once it gets to zero
                drawMole(wamDisplay_moleInfo[i]->origin, false); //mole appears!
            }
        }
        else if(wamDisplay_moleInfo[i]->ticksUntilDormant > ZERO){ //if it still has ticks till dormant
            wamDisplay_moleInfo[i]->ticksUntilDormant--; //decrement
            if(wamDisplay_moleInfo[i]->ticksUntilDormant == ZERO){ //once it gets to zero
                drawMole(wamDisplay_moleInfo[i]->origin, true); //mole disappears
                missScore++; //you missed the mole
                wamDisplay_drawScoreScreen(); //update the score screen
                activeMoles--; //one less active mole
            }
        }

    }
}
// Returns the count of currently active moles.
// A mole is active if it is not dormant, if:
// ticksUntilAwake or ticksUntilDormant are non-zero (in the moleInfo_t struct).
uint16_t wamDisplay_getActiveMoleCount(){
    return activeMoles; //return active number of moles
}

// Sets the hit value in the score window.
void wamDisplay_setHitScore(uint16_t hits){
    hitScore = hits; //set the hitScore
}

// Gets the current hit value.
uint16_t wamDisplay_getHitScore(){
    return hitScore; //return the hitScore
}

// Sets the miss value in the score window.
void wamDisplay_setMissScore(uint16_t misses){
    misses = missScore; //sets number of misses
}

// Gets the miss value.
// Can be used for testing and other functions.
uint16_t wamDisplay_getMissScore(){
    return missScore; //return missScore
}

// Sets the level value on the score board.
void wamDisplay_incrementLevel(){
    level++; //increment level variable
    if (wamDisplay_getLevel() < moles){
        wamControl_setMaxActiveMoles(wamDisplay_getLevel()+ONE); //increase active number of moles based on level
    }
}

// Retrieves the current level value.
// Can be used for testing and other functions.
uint16_t wamDisplay_getLevel(){
    return level; //return level
}

// Completely draws the score screen.
// This function renders all fields, including the text fields for "Hits" and "Misses".
// Usually only called once when you are initializing the game.
void wamDisplay_drawScoreScreen(){
    //Hits
    display_setCursor(TOP_LEFT_EDGE,HITS_Y); //set the hits cursor
    display_setTextSize(SMALL_TEXT); //The score screen will have small text
    sprintf(str,"Hits:%d",hitScore); //hits and value saves to str
    //clears the space where the number is updating
    display_fillRect(HITS_BLACK_RECT_X,HITS_BLACK_RECT_Y,BLACK_RECT_WIDTH,BLACK_RECT_HEIGHT,DISPLAY_BLACK);
    display_println(str); // Print it to the LCD
    //Misses
    display_setCursor(MISSES_X,MISSES_Y); //set the misses cursor
    sprintf(str,"Misses:%d", missScore); //misses and value saves to str
    //clears the space where the number is updating
    display_fillRect(MISSES_BLACK_RECT_X,MISSES_BLACK_RECT_Y,BLACK_RECT_WIDTH,BLACK_RECT_HEIGHT,DISPLAY_BLACK);
    display_println(str);  // Print it to the LCD.
    //Level
    display_setCursor(LEVEL_X,LEVEL_Y); //sets the level cursor
    sprintf(str, "Level:%d", level); //level and value save to str
    //clears the space where the number is updating
    display_fillRect(LEVEL_BLACK_RECT_X,LEVEL_BLACK_RECT_Y,BLACK_RECT_WIDTH,BLACK_RECT_HEIGHT,DISPLAY_BLACK);
    display_println(str);  // Print it to the LCD.
}

// Make this function available for testing purposes.
void wamDisplay_incrementMissScore(){
    missScore++; //increment missScore
}

// Reset the scores and level to restart the game.
void wamDisplay_resetAllScoresAndLevel(){
    missScore = RESET; //reset missScore
    hitScore = RESET; //reset hitScore
    level = RESET; //reset level
    activeMoles = RESET; //reset activeMoles
    wamControl_setMaxActiveMoles(RESET_ACTIVE_MOLES); //set max active moles to 1
    for (uint16_t i = ZERO; i < moles; i++){ //parse through moles
        free(wamDisplay_moleInfo[i]); //deallocate the memory
        wamDisplay_moleInfo[i] = NULL; //prevents reusing deallocated memory
    }
    free(wamDisplay_moleInfo); // Deallocates the container of arrays.
    wamDisplay_moleInfo = NULL; // Also keeps you from reusing the deallocated memory.
}

// Test function that can be called from main() to demonstrate milestone 1.
// Invoking this function should provide the same behavior as shown in the Milestone 1 video.
void wamDisplay_runMilestone1_test(){
    wamDisplay_init(); // Init the display
    switches_init(); //init the switches
    buttons_init(); // Init the push buttons.
    wamDisplay_drawSplashScreen(); //draw the splash screen
    while (!buttons_read()) { //while no button is pushed
        while(!display_isTouched()){
            //infinite while loop till touch
        }
        if (display_isTouched()){ //once the display is touched
            display_fillScreen(DISPLAY_BLACK); //fill the screen black
            display_clearOldTouchData(); //clear the touch data
            wamDisplay_drawMoleBoard(); //draw the mole board
        }
        while(!display_isTouched()){
            //infinite while loop till touch
        }
        if (display_isTouched()){ //once the display is touched
            display_fillScreen(DISPLAY_BLACK); //fill the screen black
            display_clearOldTouchData(); //clear the touch data
            wamDisplay_drawGameOverScreen(); //draw game over screen
        }
        while(!display_isTouched()){
            //infinite while loop till touch
        }
        if (display_isTouched()){ //oonce the display is touched
            display_fillScreen(DISPLAY_BLACK); //fill the screen black
            display_clearOldTouchData(); //clear the touch data
            wamDisplay_drawSplashScreen(); //draw splash screen
        }
    }
    display_fillScreen(DISPLAY_BLACK); //clear the display black
}


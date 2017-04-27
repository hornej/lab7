/*
 * wamControl.c
 *
 *  Created on: Nov 28, 2016
 *      Author: hornej2
 */

#include "wamControl.h"
#include <stdio.h>
#include <stdlib.h>
#include "supportFiles/display.h"
#include "wamDisplay.h"

#define ADC_MAX 1 //adc settler
#define RAND_DIVISOR 21 //used to set rand number
#define GET_HIT_MULTIPLIER .5 //slowly speeds up the time moles are shown
#define RESET 0 //reset counters
#define MS_TICK_DIVISOR 5 //used to set ticks according to mspertick

uint16_t adc_cnt; //adc counter
uint16_t maxActiveMoles; //keeps track of how many active moles should be on the board
uint16_t maxMissCnt; //keeps track of maximum times I can miss
uint16_t Tick; //tick variable allows for moles to be controlled by ms instead of ticks
wamDisplay_point_t hitCoordinate; //saves coordinate that the mole touch was hit


// Call this to set how much time is consumed by each tick of the controlling state machine.
// This information makes it possible to set the awake and sleep time of moles in ms, not ticks.
void wamControl_setMsPerTick(uint16_t msPerTick){
    Tick = msPerTick; //set Tick variable
}

// This returns the time consumed by each tick of the controlling state machine.
uint16_t wamControl_getMsPerTick(){
    return Tick; //return Tick
}

enum wamControl_st_t {
    init_st, // init state
    wait_st, //wait state
    adc_st, //adc state
    touch_st, //touch state
} current_state = init_st; //start in the init_st

static wamControl_st_t previous_state;
static bool first_pass = true;

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void Debug_StatePrint() {
    // Only print the message if:
    // 1. This the first pass and the value for previousState is unknown.
    // 2. previousState != currentState - this prevents reprinting the same state name over and over.
    if (previous_state != current_state || first_pass) {
        first_pass = false;                // previousState will be defined, firstPass is false.
        previous_state = current_state;     // keep track of the last state that you were in.
        switch(current_state) {            // This prints messages based upon the state that you were in.
        case init_st:
            printf("init_st wamControl\n\r"); //print init_st to console
            break;
        case wait_st:
            printf("wait_st wamControl\n\r");//print wait_st to console
            break;
        case adc_st:
            printf("adc_st wamControl\n\r");//print adc_st to console
            break;
        case touch_st:
            printf("touch_st wamControl\n\r");//print touch_st to console
            break;
        }
    }
}

// Call this before using any wamControl_ functions.
//The function wamControl_init() initializes the state machine as if it was starting
//for the first time. Use this to initialize the controller state machine before starting
//the game and when restarting the game when the game is interrupted by pressing one of the push-buttons.
void wamControl_init(){
    current_state = init_st; //start in the init state
}

// Standard tick function.
void wamControl_tick(){
    int16_t x,y; //x and y coordinate touches
    uint8_t z; //pressure. will not be used

    Debug_StatePrint();

    // Perform state action first
    switch(current_state) {
    case init_st: //do nothing
        break;
    case wait_st:
        //if the active mole count is less than number of moles
        if(wamDisplay_getActiveMoleCount() < wamControl_getMaxActiveMoles()){
            wamDisplay_activateRandomMole(); //activate a random mole
        }
        wamDisplay_updateAllMoleTickCounts(); //update the tick counts for the moles
        break;
    case adc_st:
        adc_cnt++; //increment adc counter
        break;
    case touch_st:
        adc_cnt = RESET; //reset adc counter
        display_getTouchedPoint(&x,&y,&z); //get touch data coordinates
        hitCoordinate.x = x; //save x coordinate to hitcoordinate
        hitCoordinate.y = y; //save y coordinate to hitcoordinate
        wamDisplay_whackMole(&hitCoordinate); //pass in coordinates to whack mole
        break;
    default:
        printf("flashSequence_tick state action: hit default\n\r");
        break;
    }

    // Perform state update next
    switch(current_state){
    case init_st:
        current_state = wait_st; //go directly to wait state
        break;
    case wait_st:
        if (display_isTouched()){ //if the display is touched
            display_clearOldTouchData(); //clear the touch data
            current_state = adc_st; //go to adc state
        }
        else {
            current_state = wait_st; //state in wait state user touches
        }
        break;
    case adc_st:
        if(adc_cnt == ADC_MAX){ //if adc has settled
            current_state = touch_st; //go to touch state
        }
        else {
            current_state = adc_st; //wait in adc to adc has settled
        }
        break;
    case touch_st:
        current_state = wait_st; //go directly back to wait state
        break;
        printf("flashSequence_tick state action: hit default\n\r");
        break;
    }
}

// Returns a random value that indicates how long the mole should sleep before awaking.
wamDisplay_moleTickCount_t wamControl_getRandomMoleAsleepInterval(){
    return (rand() % RAND_DIVISOR) + wamControl_getMsPerTick()/MS_TICK_DIVISOR; //random number between .5 and 1.5 seconds
}

// Returns a random value that indicates how long the mole should stay awake before going dormant.
wamDisplay_moleTickCount_t wamControl_getRandomMoleAwakeInterval(){
    //random number between .5 and 1.5 seconds that slowly decreases to .5 as uses keeps hitting moles
    return (rand() % RAND_DIVISOR) + wamControl_getMsPerTick()/(wamDisplay_getHitScore()*GET_HIT_MULTIPLIER);
}

// Set the maximum number of active moles.
void wamControl_setMaxActiveMoles(uint16_t count){
    maxActiveMoles = count; //set max active moles
}

// Get the current allowable count of active moles.
uint16_t wamControl_getMaxActiveMoles(){
    return maxActiveMoles; //return max active moles
}

// Set the seed for the random-number generator.
void wamControl_setRandomSeed(uint32_t seed){
    srand(seed); //set seed for random number generator
}

// Set the maximum number of misses until the game is over.
void wamControl_setMaxMissCount(uint16_t missCount){
    maxMissCnt = missCount; //set max miss count
}

// Use this predicate to see if the game is finished.
bool wamControl_isGameOver(){
    if(maxMissCnt == wamDisplay_getMissScore()){ //if the max miss count equals the miss score
        return true; //game is over
    }
    else {
        return false; //game is not over yet
    }
}



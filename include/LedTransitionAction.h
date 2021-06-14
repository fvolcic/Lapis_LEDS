// This class is for transitioning actions.
// It contains a normal led action that acts as a transition.
// This is so normal led actions can be used as a transition if wanted.

#include "LedAction.h"

class TransitionAction{

    // The effect that is to be run.
    LED_Action * action; 

    //The constructor for the transition object.
    TransitionAction(){}

    // Update the transition effects trigger. 
    virtual void update_trigger();

};
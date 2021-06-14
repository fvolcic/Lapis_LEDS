// This file is for The ActionTrigger class.
// An action trigger is something that is used to determine when a trigger should be called.

class ActionTrigger{

    ActionTrigger(){}

    ~ActionTrigger(){}

    // Function that is called to update the trigger.
    virtual void update_trigger() = 0;  

    // return the current status of the trigger.
    virtual bool get_trigger_status() = 0; 

    // Reset the trigger for next use.
    virtual void reset_trigger() = 0; 

};
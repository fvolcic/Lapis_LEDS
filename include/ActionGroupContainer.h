//This class is highly similar to the ActiionExecuter.
//Rather than executing actions directly, this class executes groupes of actions that are
//Written to the led strip.

#ifndef ACTIONGROUPCONTAINER_H
#define ACTIONGROUPCONTAINER_H

#include "ActionExecuter.h"
#include "ActionTrigger.h"
#include "TransitionAction.h"

// This class encapsulates all the neccessary tools
// to run transition triggers along with the LED actions.
class ActionGroup{
public:

    // The default constructor.
    // Pins the alternate core transition if needed.
    ActionGroup(); 

    // Deletes the action group.
    // Will unpin all alternate core functions.
    ~ActionGroup(); 

    // Run the action group.
    // This will run the in order trigger,
    // then also run the led actions.
    // If the trigger is enabled, this will run the transition.
    void execute_group(); 

    //Check if the group has completed running.
    //This means that the transition has completed,
    //and now the secondary action is running.
    bool is_group_complete(); 

    // Resetting the group resets all internal triggers back to their
    // internal state. Also resets needed variables.
    void reset_group(); 

    // This function passes a message onto the primary executer to update and adjust actions on the led strip.
    void send_message(char * msg); 

private:

//These are the led actions that run before and after the transition.
//This action group is responsible for the creation and deletion of the primary_executer.
ActionExecuter * primary_executer;
ActionExecuter * secondary_executer; 

// These are the trigger pointers for the triggers that will run on the core.
ActionTrigger * inOrder_trigger;
ActionTrigger * alternate_trigger; 

// This indicates to the program if the transition is ready to be triggered.
bool transition_triggered = false; 

// Keep track of the action group and if its completed executing.
bool action_group_completed = false; 

};


#endif
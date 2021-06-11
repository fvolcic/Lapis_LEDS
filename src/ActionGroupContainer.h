//This class is highly similar to the ActiionExecuter.
//Rather than executing actions directly, this class executes groupes of actions that are
//Written to the led strip.

#ifndef ACTIONGROUPCONTAINER_H
#define ACTIONGROUPCONTAINER_H

#include <queue>

#include "ActionExecuter.h"



class ActionGrouper{

    // Will likely test with both pointers and without pointers.
    // Pointers will make adding actions quicker, but the downfall to that is that 
    // it will be another level of indirection, so could be slower. This code needs to 
    // be highly effient, so the less indirection the better. 
    std::queue<ActionExecuter> actionsQueue; 

    //Adds an action to the action group.
    void addActionGroup(ActionExecuter & executer); 
    
    //Remove an action from the actions queue
    void removeActionGroup(unsigned int actionIndex);



};

#endif


#ifndef updaterheadergaurd
#define updaterheadergaurd

namespace test{
extern char * SSid;
extern char * PAss; 
}


//The reset function
//This resets the esp controller
//and starts the program from the beginning.
extern void(* resetFunc) (void);

extern void update_firmware(void * p);


#endif

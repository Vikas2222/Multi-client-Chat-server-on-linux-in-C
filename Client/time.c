/* to print current time MAN-KI-BAAT ver6
   creator : Vikas Patel (patel2vikas@gmail.com) */


#include <stdio.h>
#include <time.h>

//prints current time
void timePrint(){
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf ( "%s", asctime (timeinfo) );

}


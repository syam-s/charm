#include <stdio.h>
#include "simple_reduction.decl.h"
#include <math.h>


/*readonly*/ CProxy_main mainProxy;
/*readonly*/ int units;
/*readonly*/ double dOne;
/*readonly*/ double dTwo;
#include "simple_reduction.h"

/*mainchare*/

main::main(CkArgMsg* m)
  {
    //Process command-line arguments
    //Start the computation

    mainProxy = thishandle;
    if(m->argc < 4)
      {
        CkPrintf("%s [numChares] [starting_index_one] [starting_index_two]\n", m->argv[0]);
	CkExit();
      }
    units=atoi(m->argv[1]);
    dOne=atof(m->argv[2]);
    dTwo=atof(m->argv[3]);

    double indexSum = (units-1)*units/2;
    expected[0] = dOne*units + indexSum;
    expected[1] = dTwo*units + indexSum;

    arr = CProxy_RedExample::ckNew(units);

    CkPrintf("simple_reduction for %d pes on %d units for %f and %f\n",
	     CkNumPes(),units,dOne, dTwo);

    arr.dowork();
  }



#include "simple_reduction.def.h"

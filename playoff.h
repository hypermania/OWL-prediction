#ifndef PLAYOFF_H
#define PLAYOFF_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <malloc.h>
#include <pthread.h>

#include "dSFMT/dSFMT.h"

//#include "distribution.h"
//#include "thread.h"
#include "code.h"
#include "profile.h"
#include "standing.h"

extern dsfmt_t dsfmt_playoff;

void simulate_playoff(Standing *standing, Profile *profile, int start, int end);

#endif

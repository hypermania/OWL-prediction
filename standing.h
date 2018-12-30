#ifndef STANDING_H
#define STANDING_H

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


typedef struct {
  struct {
    int w;
    int l;
    //int map_w;
    //int map_l;
    //int map_t;
    int diff;
  } team[TEAM_NUM];
} Standing;

void init_standing(Standing *standing);
void update_standing(Standing *standing, MatchResult *match_results, int start, int end);
void print_standing(Standing *standing);



#endif

#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <malloc.h>
#include <pthread.h>

#include "dSFMT/dSFMT.h"

#include "distribution.h"


#define PROB_NUM (3)
#define TEAM_NUM 12

typedef struct {
  double w_4_0;
  double w_3_1;
  double w_3_2;
  double w_2_3;
  double w_1_3;
  double w_0_4;
} MatchProb;

typedef struct {
  long int n;
  double norm;
  int home_team[3];
  int away_team[3];
  double skill[TEAM_NUM];
  double skill_sqr[TEAM_NUM];
  double prob[PROB_NUM];
  double prob_sqr[PROB_NUM];
  MatchProb match_prob[PROB_NUM];
  double diff_entropy;
} ThreadResult;

typedef struct {
  dsfmt_t dsfmt;
  long int num_points;
  int home_team[3];
  int away_team[3];
  double (*dist)(double, double, double, double, double, double, double, double, double, double, double, double);
} MonteCarloThread;


void *monte_carlo_thread(void *thread_struct);



#endif


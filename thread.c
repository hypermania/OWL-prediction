#include "thread.h"

void *monte_carlo_thread(void *thread_struct){

  MonteCarloThread *info = (MonteCarloThread *)thread_struct;
  dsfmt_t *dsfmt = &info->dsfmt;
  long int num_points = info->num_points;
  int home_team[3];
  int away_team[3];
  memcpy(home_team, info->home_team, sizeof(home_team));
  memcpy(away_team, info->away_team, sizeof(away_team));
  double (*dist)(double, double, double, double, double, double, double, double, double, double, double, double) = info->dist;

  
  ThreadResult result;
  memset(&result, 0, sizeof(ThreadResult));
  memcpy(result.home_team, info->home_team, sizeof(home_team));
  memcpy(result.away_team, info->away_team, sizeof(away_team));

  
  double r[TEAM_NUM];

  for(result.n = 0; result.n < num_points; result.n++){
    int team;
    for(team = 0; team < TEAM_NUM; team++){
      r[team] = dsfmt_genrand_open_close(dsfmt);
    }
    
    double dist_value = dist(r[0], r[1], r[2], r[3], r[4], r[5],
			     r[6], r[7], r[8], r[9], r[10], r[11]);
    result.norm += dist_value;
    
    for(team = 0; team < TEAM_NUM; team++){
      result.skill[team] += r[team] * dist_value;
      result.skill_sqr[team] += r[team] * r[team] * dist_value;
    }

    double winrate[PROB_NUM];
    
    winrate[0] = WINRATE(r[home_team[0]], r[away_team[0]]);
    winrate[1] = WINRATE(r[home_team[1]], r[away_team[1]]);
    winrate[2] = WINRATE(r[home_team[2]], r[away_team[2]]);

    result.prob[0] += winrate[0] * dist_value;
    result.prob[1] += winrate[1] * dist_value;
    result.prob[2] += winrate[2] * dist_value;

    result.prob_sqr[0] += winrate[0] * winrate[0] * dist_value;
    result.prob_sqr[1] += winrate[1] * winrate[1] * dist_value;
    result.prob_sqr[2] += winrate[2] * winrate[2] * dist_value;

    int pair;
    for(pair = 0; pair < PROB_NUM; pair++){
      result.match_prob[pair].w_4_0 +=
	winrate[pair] * winrate[pair] *
	winrate[pair] * winrate[pair] * dist_value;
      result.match_prob[pair].w_3_1 +=
	4 * winrate[pair] * winrate[pair] *
	winrate[pair] * (1 - winrate[pair]) * dist_value;
      result.match_prob[pair].w_3_2 +=
	6 * winrate[pair] * winrate[pair] *
	(1 - winrate[pair]) * (1 - winrate[pair]) * winrate[pair] * dist_value;
      result.match_prob[pair].w_2_3 +=
	6 * winrate[pair] * winrate[pair] *
	(1 - winrate[pair]) * (1 - winrate[pair]) * (1 - winrate[pair]) * dist_value;
      result.match_prob[pair].w_1_3 +=
	4 * (1 - winrate[pair]) * (1 - winrate[pair]) *
	(1 - winrate[pair]) * winrate[pair] * dist_value;
      result.match_prob[pair].w_0_4 +=
	(1 - winrate[pair]) * (1 - winrate[pair]) *
	(1 - winrate[pair]) * (1 - winrate[pair]) * dist_value;
    }
    
    result.diff_entropy -= log(dist_value) * dist_value;
    
  }

  ThreadResult *thread_result = malloc(sizeof(ThreadResult));
  memcpy(thread_result, &result, sizeof(ThreadResult));

  return (void *)thread_result;
}

#include "profile.h"

const char team_names[TEAM_NUM][4]
= {
  "SEO", "LDN", "NYE", "LAV", "LAG", "HOU",
  "BOS", "PHI", "SFS", "DAL", "FLA", "SHD"
};

const char team_div[TEAM_NUM][4]
= {
  "PAC", "ATL", "ATL", "PAC", "PAC", "ATL",
  "ATL", "ATL", "PAC", "PAC", "ATL", "PAC"
};

Profile *profile_create(void){
  Profile *profile = malloc(sizeof(Profile));
  return profile;
}

Profile *profile_from_thread_result(ThreadResult **thread_results, int n){

  Profile *profile = malloc(sizeof(Profile));
  memset(profile, 0, sizeof(Profile));
  
  int thread, team, pair;

  long int num_points = 0;
  double norm = 0;
  double sum_diff_entropy = 0;
  for(thread = 0; thread < n; thread++){
    num_points += thread_results[thread]->n;
    norm += thread_results[thread]->norm;
    sum_diff_entropy += thread_results[thread]->diff_entropy;
  }
  profile->diff_entropy = sum_diff_entropy / norm + log(norm / num_points);

  for(team = 0; team < TEAM_NUM; team++){
    double sum = 0;
    double sum_sqr = 0;
    for(thread = 0; thread < n; thread++){
      sum += thread_results[thread]->skill[team];
      sum_sqr += thread_results[thread]->skill_sqr[team];
    }
    profile->skill[team] = sum / norm;
    profile->skill_std[team] = sqrt((sum_sqr / norm) - (sum / norm) * (sum / norm));
  }

  for(pair = 0; pair < 3; pair++){
    double sum_prob = 0;
    double sum_prob_sqr = 0;
    MatchProb match_prob;
    memset(&match_prob, 0, sizeof(MatchProb));
    for(thread = 0; thread < n; thread++){
      sum_prob += thread_results[thread]->prob[pair];
      sum_prob_sqr += thread_results[thread]->prob_sqr[pair];

      match_prob.w_4_0 += thread_results[thread]->match_prob[pair].w_4_0;
      match_prob.w_3_1 += thread_results[thread]->match_prob[pair].w_3_1;
      match_prob.w_3_2 += thread_results[thread]->match_prob[pair].w_3_2;
      match_prob.w_2_3 += thread_results[thread]->match_prob[pair].w_2_3;
      match_prob.w_1_3 += thread_results[thread]->match_prob[pair].w_1_3;
      match_prob.w_0_4 += thread_results[thread]->match_prob[pair].w_0_4;
    }
    profile->prob[pair] = sum_prob / norm;
    profile->prob_std[pair] = sqrt((sum_prob_sqr / norm) - (sum_prob / norm) * (sum_prob / norm));

    profile->match_prob[pair].w_4_0 = match_prob.w_4_0 / norm;
    profile->match_prob[pair].w_3_1 = match_prob.w_3_1 / norm;
    profile->match_prob[pair].w_3_2 = match_prob.w_3_2 / norm;
    profile->match_prob[pair].w_2_3 = match_prob.w_2_3 / norm;
    profile->match_prob[pair].w_1_3 = match_prob.w_1_3 / norm;
    profile->match_prob[pair].w_0_4 = match_prob.w_0_4 / norm;
  }

  memcpy(profile->home_team, thread_results[0]->home_team, sizeof(profile->home_team));
  memcpy(profile->away_team, thread_results[0]->away_team, sizeof(profile->away_team));
  
  return profile;
}

void profile_print(Profile *profile){
  // Print skill ranking
  int compare(const void *p1, const void *p2){
    int r1 = *((int *)p1);
    int r2 = *((int *)p2);
    if(profile->skill[r1] > profile->skill[r2]) return -1;
    if(profile->skill[r1] == profile->skill[r2]) return 0;
    if(profile->skill[r1] < profile->skill[r2]) return 1;
    return 0;
  }

  printf("Skill Ranking\n");
  int ranking[TEAM_NUM];
  int i;
  for(i = 0; i < TEAM_NUM; i++){
    ranking[i] = i;
  }
  
  qsort(&ranking, TEAM_NUM, sizeof(int), compare);

  for(i = 0; i < TEAM_NUM; i++){
    printf("(Rank %02d) %s: %lf      std: %lf\n", i + 1, team_names[ranking[i]], profile->skill[ranking[i]], profile->skill_std[ranking[i]]);
  }

  printf("\n");
  
  // Print win probabilities
  /*
  void print_winrate_series(double p){
    double winrate_4_0 = p * p * p * p;
    double winrate_3_1 = 4 * p * p * p * (1 - p);
    double winrate_3_2 = 6 * p * p * (1 - p) * (1 - p) * p;
    double winrate_2_3 = 6 * p * p * (1 - p) * (1 - p) * (1 - p);
    double winrate_1_3 = 4 * (1 - p) * (1 - p) * (1 - p) * p;
    double winrate_0_4 = (1 - p) * (1 - p) * (1 - p) * (1 - p);

    printf("4:0 %lf    3:1 %lf    3:2 %lf    2:3 %lf    1:3 %lf    0:4 %lf\n", winrate_4_0, winrate_3_1, winrate_3_2, winrate_2_3, winrate_1_3, winrate_0_4);
  }
  */
  void print_winrate_series(MatchProb *match_prob){
    printf("4:0 %lf    3:1 %lf    3:2 %lf    2:3 %lf    1:3 %lf    0:4 %lf\n",
	   match_prob->w_4_0, match_prob->w_3_1, match_prob->w_3_2,
	   match_prob->w_2_3, match_prob->w_1_3, match_prob->w_0_4);
  }

  int pair;
  for(pair = 0; pair < 3; pair++){
    printf("%s-%s: map_prob = %lf   match_prob = %lf    std = %lf\n",
	   team_names[profile->home_team[pair]],
	   team_names[profile->away_team[pair]],
	   profile->prob[pair],
	   profile->match_prob[pair].w_4_0 +
	   profile->match_prob[pair].w_3_1 +
	   profile->match_prob[pair].w_3_2,
	   profile->prob_std[pair]
	   );
    print_winrate_series(&profile->match_prob[pair]);
    printf("\n");
  }

  // Print diff entropy
  printf("diff entropy = %lf\n\n\n", profile->diff_entropy);
}

void profile_save(Profile *profile, const char *filename){
  FILE *fp = fopen(filename, "w");
  fwrite(profile, sizeof(Profile), 1, fp);
  fclose(fp);
}

Profile *profile_load(const char *filename){
  FILE *fp = fopen(filename, "r");
  Profile *profile = malloc(sizeof(Profile));
  fread(profile, sizeof(Profile), 1, fp);
  fclose(fp);
  return profile;
}

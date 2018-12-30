
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <malloc.h>
#include <pthread.h>
#include <dlfcn.h>

#include "dSFMT/dSFMT.h"

#include "profile.h"
#include "code.h"
#include "thread.h"
#include "standing.h"
#include "playoff.h"

void prediction(double p, int *scores){
  if(p < 0.2){
    scores[0] = 0;
    scores[1] = 4;
  } else if(p < 0.5){
    scores[0] = 1;
    scores[1] = 3;
  } else if(p < 0.8){
    scores[0] = 3;
    scores[1] = 1;
  } else {
    scores[0] = 4;
    scores[1] = 0;
  }
}

  
  /*
  int num_3_1 = 0;
  int num_3_2 = 0;
  int num_4_0 = 0;
  int j;
  for(j = 0; j < num_matches; j++){
    if(match_results[j].home_score == 1 &&
       match_results[j].away_score == 3){
      num_3_1++;
    }
    if(match_results[j].home_score == 3 &&
       match_results[j].away_score == 1){
      num_3_1++;
    }
    if(match_results[j].home_score == 2 &&
       match_results[j].away_score == 3){
      num_3_2++;
    }
    if(match_results[j].home_score == 3 &&
       match_results[j].away_score == 2){
      num_3_2++;
    }
    if(match_results[j].home_score == 4 &&
       match_results[j].away_score == 0){
      num_4_0++;
    }
    if(match_results[j].home_score == 0 &&
       match_results[j].away_score == 4){
      num_4_0++;
    }
  }
  printf("num_3_1 = %d\n", num_3_1);
  printf("num_3_2 = %d\n", num_3_2);
  printf("num_4_0 = %d\n", num_4_0);
  
  exit(0);

  */


int main(int argc, char **argv){
  
  int seed = time(NULL);
  int num_matches = load_matches("match");
  int num_schedule = load_schedule("schedule");

  /*
  int start = (12 * 5 + 2) * 2 + (12 * 5 + 3);
  int end = num_matches - 1;
  */
  
  int start = 0;
  int end = 12 * 3 - 1;

  /*
  int start_schedule = (12 * 5) + 12 * 5;
  int end_schedule = num_schedule - 1;

  printf("start_sch = %d, end_sch = %d\n", start_schedule, end_schedule);


  Standing standing;
  init_standing(&standing);
  update_standing(&standing, match_results, (12 * 5 + 2) * 2 + (12 * 5 + 3), end);
  print_standing(&standing);

  dsfmt_init_gen_rand(&dsfmt_playoff, seed);
  */


  printf("%d matches used for generating the distribution\n", end - start + 1);

  write_code("temp.c", start, end);
  system("gcc temp.c -msse4.2 -msse2 -mpopcnt -mbmi -mbmi2 -mavx -mavx2 -lm -O3 -ffast-math -shared -fPIC -o dynamic.so");
  void *handle = dlopen("./dynamic.so", RTLD_NOW);
  system("rm temp.c");
  double (*dist)(double, double, double, double, double, double, double, double, double, double, double, double) = dlsym(handle, "p");
  
  MonteCarloThread thread_info[4];
  pthread_t tid[4];
  

  int i;
  for(i = 0; i < 4; i++){
    dsfmt_init_gen_rand(&thread_info[i].dsfmt, seed + i);
    thread_info[i].num_points = 1000000000 / 4;
    thread_info[i].home_team[0] = get_team_num("PHI");
    thread_info[i].home_team[1] = get_team_num("LDN");
    thread_info[i].home_team[2] = get_team_num("LAG");
    thread_info[i].away_team[0] = get_team_num("BOS");
    thread_info[i].away_team[1] = get_team_num("LAG");
    thread_info[i].away_team[2] = get_team_num("LAV");
    thread_info[i].dist = dist;
    pthread_create(&tid[i], NULL, monte_carlo_thread, &thread_info[i]);
  }
  
  ThreadResult *(thread_result[4]);
  
  for(i = 0; i < 4; i++){
    pthread_join(tid[i], (void **)&thread_result[i]);
  }

  Profile *profile = profile_from_thread_result(thread_result, 4);
  //profile_save(profile, "profile/stage4_week13");

  //Profile *profile = profile_load("profile/stage3_week13");

  profile_print(profile);
  
  /*
  long int team_playoff_count[TEAM_NUM];
  memset(team_playoff_count, 0, sizeof(team_playoff_count));
  */

  /*
  long int team_atl_playoff_count[TEAM_NUM];
  long int team_pac_playoff_count[TEAM_NUM];
  memset(team_atl_playoff_count, 0, sizeof(team_atl_playoff_count));
  memset(team_pac_playoff_count, 0, sizeof(team_pac_playoff_count));
  */

  /*
  long int total_trial = 10000000;
  long int trial;
  for(trial = 0; trial < total_trial; trial++){
    Standing try;
    memcpy(&try, &standing, sizeof(Standing));
    simulate_playoff(&try, profile, start_schedule, end_schedule);
    
    int compare(const void *p1, const void *p2){
      int r1 = *((int *)p1);
      int r2 = *((int *)p2);
      if((&try)->team[r1].w > (&try)->team[r2].w) return -1;
      if((&try)->team[r1].w == (&try)->team[r2].w){
	if((&try)->team[r1].diff > (&try)->team[r2].diff) return -1;
	if((&try)->team[r1].diff == (&try)->team[r2].diff) return 0;
	if((&try)->team[r1].diff < (&try)->team[r2].diff) return 1;
      }
      if((&try)->team[r1].w < (&try)->team[r2].w) return 1;
      return 0;
    }
  
    int ranking[TEAM_NUM];
    int r;
    for(r = 0; r < TEAM_NUM; r++){
      ranking[r] = r;
    }
  
    qsort(&ranking, TEAM_NUM, sizeof(int), compare);

    for(r = 0; r < 4; r++){
      team_playoff_count[ranking[r]]++;
    }

  }

  int team;
  for(team = 0; team < TEAM_NUM; team++){
    printf("%s: %lf\n", team_names[team], team_playoff_count[team] / (double)total_trial);
  }
}
  */
  
  /*
    int scores[2];
    int pair;
    for(pair = 0; pair < 3; pair++){
      prediction(profile->prob[pair], scores);
      if((scores[0] > scores[1]) ==
	 (match_results[end + pair + 1].home_score > match_results[end + pair + 1].away_score)){
	correct_win++;
	if(scores[0] == match_results[end + pair + 1].home_score &&
	   scores[1] == match_results[end + pair + 1].away_score){
	  correct_score++;
	}
      }
    }
    
    dlclose(handle);
  }

  printf("correct_win = %d,    correct_score = %d\n", correct_win, correct_score);
    */


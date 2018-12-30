#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <stdlib.h>
#include <math.h>

#define WINRATE(alpha_1, alpha_2) (1/(1+(alpha_2)/(alpha_1)))

#define LIKELIHOOD_4_0(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2))
#define LIKELIHOOD_3_1(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_2, alpha_1))
#define LIKELIHOOD_3_2(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_1, alpha_2))
#define LIKELIHOOD_2_3(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1))
#define LIKELIHOOD_1_3(alpha_1, alpha_2) (WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_1, alpha_2))
#define LIKELIHOOD_0_4(alpha_1, alpha_2) (WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1))
#define LIKELIHOOD_2_2(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1))
#define LIKELIHOOD_3_0(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2))
#define LIKELIHOOD_0_3(alpha_1, alpha_2) (WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1))
#define LIKELIHOOD_2_1(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2) * WINRATE(alpha_2, alpha_1))
#define LIKELIHOOD_1_2(alpha_1, alpha_2) (WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1) * WINRATE(alpha_1, alpha_2))
#define LIKELIHOOD_2_0(alpha_1, alpha_2) (WINRATE(alpha_1, alpha_2) * WINRATE(alpha_1, alpha_2))
#define LIKELIHOOD_0_2(alpha_1, alpha_2) (WINRATE(alpha_2, alpha_1) * WINRATE(alpha_2, alpha_1))


double p(double SEO, double LDN, double NYE, double LAV, double LAG, double HOU,
	 double BOS, double PHI, double SFS, double DAL, double FLA, double SHD);

#endif

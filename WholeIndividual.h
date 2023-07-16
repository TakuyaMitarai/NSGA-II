#pragma once

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

#define GENERATION_MAX	1000						// 世代交代数
#define WPOP_SIZE		200							// 全体解個体数
#define MUTATE_PROB		0.001						// 部分解突然変異確率
#define MUTATE_PROBI	MUTATE_PROB*RAND_MAX		// 部分解突然変異 * RAND_MAX
#define WCHROM_LEN		2000						// 全体解遺伝子数
#define TOURNAMENT_SIZE 10							// 混雑度トーナメント数
#define PI				3.141592
#define ENDTIME			10000000					//実験を止める時間(秒)

class WholeIndividual
{
public:
	WholeIndividual();
	~WholeIndividual();
	void newGeneration(WholeIndividual* p1, WholeIndividual* p2, int index1, int index2);
	void objective_evaluation();
	void mutate();

	double chrom[WCHROM_LEN];				// 染色体
	long double fitness1;					// 目的関数の適応度１
	long double fitness2;					// 目的関数の適応度２
	double fitness;							// 混雑度とパレートランキング
	double rankfit;							// パレートランキング
};

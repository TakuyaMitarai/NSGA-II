#include "WholePopulation.h"

// コンストラクタ
WholePopulation::WholePopulation()
{
	int i;

	for(i = 0; i < WPOP_SIZE * 2; i++)
		pop[i] = new WholeIndividual();
}

// デストラクタ
WholePopulation::~WholePopulation()
{
	int i;

	for(i = 0; i < WPOP_SIZE * 2; i++)
		delete pop[i];
}


// Qw(t+1), Rw(t+1)生成
void WholePopulation::newWholeGeneration()
{
	int i, j, k, a, fit1, tmp1, tmp2, index1, index2;
	int cnt = 0;

	for(k = WPOP_SIZE; k < WPOP_SIZE * 2; k++) {
		tmp1 = rand() % WPOP_SIZE;
		fit1 = pop[tmp1]->fitness;
		for(i = 0; i < TOURNAMENT_SIZE; i++) {
			a = rand() % WPOP_SIZE;
			if(fit1 < pop[a]->fitness) {
				fit1 = pop[a]->fitness;
				tmp1 = a;
			}
		}
		tmp2 = rand() % WPOP_SIZE;
		fit1 = pop[tmp2]->fitness;
		for(i = 0; i < TOURNAMENT_SIZE; i++) {
			a = rand() % WPOP_SIZE;
			if(fit1 < pop[a]->fitness) {
				fit1 = pop[a]->fitness;
				tmp2 = a;
			}
		}
		index1 = rand() % WCHROM_LEN;
		index2 = ((rand() % (WCHROM_LEN - 1)) + index1) % WCHROM_LEN;

		pop[k]->newGeneration(pop[tmp1], pop[tmp2], index1, index2);
		pop[k]->mutate();
	}

	for(i = 0; i < WPOP_SIZE * 2; i++) {
		pop[i]->fitness = 0;
		pop[i]->fitness1 = 0;
		pop[i]->fitness2 = 0;
		pop[i]->rankfit = DBL_MAX;
	}
}

// パレートランキングに関するクイックソート
void WholePopulation::sort(int lb, int ub)
{
	int i, j, k;
	double pivot;
	WholeIndividual* swap;

	if(lb < ub) {
		k = (lb + ub) / 2;
		pivot = pop[k]->rankfit;
		i = lb;
		j = ub;
		do {
			while(pop[i]->rankfit < pivot)
				i++;
			while(pop[j]->rankfit > pivot)
				j--;
			if(i <= j) {
				swap = pop[i];
				pop[i] = pop[j];
				pop[j] = swap;
				i++;
				j--;
			}
		} while(i <= j);
		sort(lb, j);
		sort(i, ub);
	}
}

// 全体解個体の評価
void WholePopulation::evaluation()
{
	int i, j, k;
	int cnt;
	int current_cnt = 0;
	int flag;
	int rank = 1;
	int num;
	vector<int> now_s;
	vector<int> next_s;
	vector<int> rank_s;

	//初期化
	for(i = 0; i < WPOP_SIZE * 2; i++) {
		next_s.push_back(i);
	}
	//目的関数の算出
	#pragma omp parallel for
	for(i = 0; i < WPOP_SIZE * 2; i++) {
		pop[i]->objective_evaluation();
	}
	//パレートランキングの算出
	while(current_cnt < WPOP_SIZE * 2) {
		cnt = current_cnt;
		now_s = next_s;
		next_s.clear();
		rank_s.clear();
		for(j = 0; j < WPOP_SIZE * 2 - cnt; j++) {
			flag = 1;
			for(k = 0; k < WPOP_SIZE * 2 - cnt; k++) {
				if(pop[now_s[j]]->fitness1 >= pop[now_s[k]]->fitness1 && pop[now_s[j]]->fitness2 >= pop[now_s[k]]->fitness2 && now_s[j] != now_s[k]) {
					if(pop[now_s[j]]->fitness1 != pop[now_s[k]]->fitness1 || pop[now_s[j]]->fitness2 != pop[now_s[k]]->fitness2) {
						num = now_s[j];
						flag = 0;
					}
				}
				if(flag == 0) {
					next_s.push_back(num);
					break;
				}
			}
			if(flag == 1) {
				pop[now_s[j]]->rankfit = rank;
				rank_s.push_back(now_s[j]);
				current_cnt++;
			}
		}
		//混雑度の算出
		congestion(rank, rank_s);
		rank++;
	}
	for(i = 0; i < WPOP_SIZE * 2; i++) {
		pop[i]->rankfit += 1 / (pop[i]->fitness * 10 + 1);
		pop[i]->fitness = pop[i]->rankfit;
	}
	//全体解個体のソート
	sort(0, WPOP_SIZE * 2 - 1);
}

//混雑度
void WholePopulation::congestion(int i, vector<int> rank_s)
{
	int j;
	
	for(j = 0; j < rank_s.size(); j++) {
		pop[rank_s[j]]->fitness = 0;
	}
	if(rank_s.size() >= 2) {
		sortobject(0, rank_s.size()-1, rank_s);
		pop[rank_s[0]]->fitness = 10000;
		pop[rank_s[rank_s.size()-1]]->fitness = 10000;
		for(j = 2; j < rank_s.size() - 1; j++) {
			if(pop[rank_s[rank_s.size()-1]]->fitness1 - pop[rank_s[0]]->fitness1 != 0) {
				pop[rank_s[j]]->fitness += (pop[rank_s[j+1]]->fitness1 - pop[rank_s[j-1]]->fitness1) / (pop[rank_s[rank_s.size()-1]]->fitness1 - pop[rank_s[0]]->fitness1);
			}
		}
		sortobject2(0, rank_s.size()-1, rank_s);
		pop[rank_s[0]]->fitness = 10000;
		pop[rank_s[rank_s.size()-1]]->fitness = 10000;
		for(j = 2; j < rank_s.size() - 1; j++) {
			if(pop[rank_s[rank_s.size()-1]]->fitness2 - pop[rank_s[0]]->fitness2 != 0) {
				pop[rank_s[j]]->fitness += (pop[rank_s[j+1]]->fitness2 - pop[rank_s[j-1]]->fitness2) / (pop[rank_s[rank_s.size()-1]]->fitness2 - pop[rank_s[0]]->fitness2);
			}
		}
	}
}

// 目的関数1に関するソート
void WholePopulation::sortobject(int lb, int ub, vector<int> rank_s)
{
	int i, j, k;
	double pivot;
	WholeIndividual* swap;
	if(lb < ub) {
		k = (lb + ub) / 2;
		pivot = pop[rank_s[k]]->fitness1;
		i = lb;
		j = ub;
		do {
			while(pop[rank_s[i]]->fitness1 < pivot)
				i++;
			while(pop[rank_s[j]]->fitness1 > pivot)
				j--;
			if(i <= j) {
				swap = pop[rank_s[i]];
				pop[rank_s[i]] = pop[rank_s[j]];
				pop[rank_s[j]] = swap;
				i++;
				j--;
			}
		} while(i <= j);
		sortobject(lb, j, rank_s);
		sortobject(i, ub, rank_s);
	}
}

//目的関数2に関するソート
void WholePopulation::sortobject2(int lb, int ub, vector<int> rank_s)
{
	int i, j, k;
	double pivot;
	WholeIndividual* swap;
	if(lb < ub) {
		k = (lb + ub) / 2;
		pivot = pop[rank_s[k]]->fitness2;
		i = lb;
		j = ub;
		do {
			while(pop[rank_s[i]]->fitness2 < pivot)
				i++;
			while(pop[rank_s[j]]->fitness2 > pivot)
				j--;
			if(i <= j) {
				swap = pop[rank_s[i]];
				pop[rank_s[i]] = pop[rank_s[j]];
				pop[rank_s[j]] = swap;
				i++;
				j--;
			}
		} while(i <= j);
		sortobject2(lb, j, rank_s);
		sortobject2(i, ub, rank_s);
	}
}

// 解表示
void WholePopulation::printsolution()
{
	int i, j, k;

	for(i = 0; i < WPOP_SIZE; i++) {
		for(j = 0; j < WCHROM_LEN; j++) {
				cout << pop[i]->chrom[j] << " ";
		}
		cout << endl;
	}
}

//目的関数表示
void WholePopulation::printfitness()
{
	int i, j, k;

	for(i = 0; i < WPOP_SIZE; i++) {
		//cout << pop[i]->rankfit << ": " << pop[i]->fitness1 << ", " << pop[i]->fitness2 << ", " << pop[i]->fitness << endl;
		cout << pop[i]->fitness1 << " " << pop[i]->fitness2 << endl;
	}
}
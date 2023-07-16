#include "nsga2.h"

// コンストラクタ
nsga2::nsga2()
{
	int i, j;

	// 全体解初期化
	wpop = new WholePopulation();

	//パレートランキングと混雑距離による評価
	wpop->evaluation();
}

// デストラクタ
nsga2::~nsga2()
{
	int i;

	delete wpop;
}

// 探索
void nsga2::solve(clock_t start, int runtimes)
{
	int gen, i, j, k, cnt;
	vector<double> x, y;
	string fn = "./result";
	string txt = ".txt";
	string runtimesstr = to_string(runtimes);
	string fn1 = fn + runtimesstr + txt;
	size_t size1 = fn1.size() + 1;
	char *fname = new char[size1];
	strcpy(fname, fn1.c_str());
	FILE *fp;
	fp = fopen(fname, "w");

	for(gen = 1; gen <=	GENERATION_MAX; gen++) {
		cout << "第" << gen << "世代" << endl;
		//全体解世代交代
		wpop->newWholeGeneration();

		//パレートランキングと混雑距離による評価
		wpop->evaluation();
		fileprint(fp, gen);
		if((float)(clock() - start) / CLOCKS_PER_SEC >= ENDTIME) {
			printf("終了時間：%f秒\n", (float)(clock() - start) / CLOCKS_PER_SEC);
			break;
		}
	}
	wpop->printfitness();
	//wpop->printsolution();
}

//適応度出力
void nsga2::fileprint(FILE *fp, int gen)
{
	int i;

	for(i = 0; i < WPOP_SIZE; i++) {
		fprintf(fp, "%Lf %Lf %d\n", wpop->pop[i]->fitness1, wpop->pop[i]->fitness2, gen);
	}
}
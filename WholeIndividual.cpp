#include "WholeIndividual.h"
#include <math.h>

// コンストラクタ
WholeIndividual::WholeIndividual()
{	
	int i;
	
	for(i = 0; i < WCHROM_LEN; i++) {
		//chrom[i] =  (double)(rand() % 60000) / 1000 - 30;
		//chrom[i] =  (double)(rand() % 80000) / 10000 - 4;
		//chrom[i] =  (double)(rand() % 100000) / 10000 - 5; //ZDT4・ KUR
		//chrom[i] =  (double)(rand() % 100000) / 100000; //Discontinuous Pareto-optimal front
		chrom[i] = rand() % 2; //ZDT4，多目的部分だまし問題
	}
	fitness = 0;
	rankfit = DBL_MAX;
}

// デストラクタ
WholeIndividual::~WholeIndividual()
{

}

void WholeIndividual::newGeneration(WholeIndividual* p1, WholeIndividual* p2, int index1, int index2)
{
	int i, min, max;

	if(index1 < index2) {
		min = index1;
		max = index2;
	} else {
		min = index2;
		max = index1;
	}
	for(i = 0; i < min; i++)
		chrom[i] = p1->chrom[i];
	for( ; i < max; i++)
		chrom[i] = p2->chrom[i];
	for( ; i < WCHROM_LEN; i++)
		chrom[i] = p1->chrom[i];
}

void WholeIndividual::mutate()
{
	int i;

	for(i = 0; i < WCHROM_LEN; i++) {
		if(rand() < MUTATE_PROBI)
			//chrom[i] =  (double)(rand() % 60000) / 1000 - 30;
			//chrom[i] =  (double)(rand() % 80000) / 10000 - 4;
			//chrom[i] =  (double)(rand() % 100000) / 10000 - 5; //KUR
			//chrom[i] =  (double)(rand() % 100000) / 100000; //Discontinuous Pareto-optimal front
			chrom[i] = rand() % 2; //ZDT4，多目的部分だまし問題
	}
}

// 目的関数
void WholeIndividual::objective_evaluation()
{
	int i, j;
	long double gx = 0;
	long double hx = 0;
	long double sum1 = 0;
	long double sum2 = 0;

	fitness = 0.0;
	fitness1 = 0.0;
	fitness2 = 0.0;
	rankfit = 0.0;
	
	/*
	//MOP
	fitness1 = (chrom[0]->chrom[0] + 30) / 60;
	for(i = 0; i < WCHROM_LEN; i++) {
		for(j = 0; j < PCHROM_LEN; j++){
			if(i != 0 || j != 0) {
				gx += chrom[i]->chrom[j] * chrom[i]->chrom[j] - 10 * cos(2 * PI * chrom[i]->chrom[j]);
			}
		}
	}
	gx = 1 + 10 * (PCHROM_LEN * WCHROM_LEN - 1) + gx;
	if(fitness1 <=gx) {
		hx = 1 - pow(fitness1/gx, 0.5);
	} else {
		hx = 0;
	}

	fitness2 = gx * hx;
	*/

	//Fonseca-fleming function
	/*
	fitness1 = chrom[0]->chrom[0];
	for(i = 0; i < WCHROM_LEN; i++) {
		for(j = 0; j < PCHROM_LEN; j++){
			if(i != 0 || j != 0) {
				gx += chrom[i]->chrom[j];
			}
		}
	}
	gx = 1 + 9 / 29 * gx;
	hx = 1 - sqrt(fitness1 / gx);
	fitness2 = gx * hx;
	*/

	/*
	for(i = 0; i < WCHROM_LEN; i++) {
		for(j = 0; j < PCHROM_LEN; j++){
			sum1 += pow((chrom[i]->chrom[j] -  1 / sqrt(i*PCHROM_LEN+j+1)),2);
			sum2 += pow((chrom[i]->chrom[j] +  1 / sqrt(i*PCHROM_LEN+j+1)),2);
		}
	}
	fitness1 = 1 - pow(2.71828, -sum1);
	fitness2 = 1 - pow(2.71828, -sum2);
	*/

	/*
	//ZDT1グレイコード
	std::vector<int> binaryCode(20, 0);
    binaryCode[0] = chrom[0]; // 最上位ビットはそのままコピー

    // グレイコードを2進数に変換
    for (int i = 1; i < 20; i++) {
        binaryCode[i] = binaryCode[i-1] ^ (int)chrom[i]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
    }

    // 2進数を10進数に変換
    int decimalValue = 0;
    for (int i = 0; i < 20; i++) {
        decimalValue += binaryCode[i] * std::pow(2, 19 - i);
    }

    // バイナリの最大値と最小値を求める
    int binaryMin = 0;
    int binaryMax = std::pow(2, 20) - 1;

    // バイナリを-kからkの実数にマッピングする
    double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);
	
	fitness1 = realValue;

	for(i = 1; i < 30; i++) {
		std::vector<int> binaryCode(20, 0);
		binaryCode[0] = chrom[i*20]; // 最上位ビットはそのままコピー

		// グレイコードを2進数に変換
		for (int j = 1; j < 20; j++) {
			binaryCode[j] = binaryCode[j-1] ^ (int)chrom[i*20+j]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
		}

		// 2進数を10進数に変換
		decimalValue = 0;
		for (int j = 0; j < 20; j++) {
			decimalValue += binaryCode[j] * std::pow(2, 19 - j);
		}

		// バイナリの最大値と最小値を求める
		binaryMin = 0;
		binaryMax = std::pow(2, 20) - 1;

		// バイナリを-kからkの実数にマッピングする
		double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);
		gx += realValue;
	}
	gx = 1 + 9 * gx / 29;
	hx = 1 - pow(fitness1 / gx, 0.5);
	fitness2 = gx * hx;
	*/

	/*
	//ZDT2グレイコード
	std::vector<int> binaryCode(20, 0);
    binaryCode[0] = chrom[0]; // 最上位ビットはそのままコピー

    // グレイコードを2進数に変換
    for (int i = 1; i < 20; i++) {
        binaryCode[i] = binaryCode[i-1] ^ (int)chrom[i]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
    }

    // 2進数を10進数に変換
    int decimalValue = 0;
    for (int i = 0; i < 20; i++) {
        decimalValue += binaryCode[i] * std::pow(2, 19 - i);
    }

    // バイナリの最大値と最小値を求める
    int binaryMin = 0;
    int binaryMax = std::pow(2, 20) - 1;

    // バイナリを-kからkの実数にマッピングする
    double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);
	
	fitness1 = realValue;

	for(i = 1; i < 30; i++) {
		std::vector<int> binaryCode(20, 0);
		binaryCode[0] = chrom[i*20]; // 最上位ビットはそのままコピー

		// グレイコードを2進数に変換
		for (int j = 1; j < 20; j++) {
			binaryCode[j] = binaryCode[j-1] ^ (int)chrom[i*20+j]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
		}

		// 2進数を10進数に変換
		decimalValue = 0;
		for (int j = 0; j < 20; j++) {
			decimalValue += binaryCode[j] * std::pow(2, 19 - j);
		}

		// バイナリの最大値と最小値を求める
		binaryMin = 0;
		binaryMax = std::pow(2, 20) - 1;

		// バイナリを-kからkの実数にマッピングする
		double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);
		gx += realValue;
	}
	gx = 1 + 9 * gx / 29;
	hx = 1 - pow(fitness1 / gx, 2);
	fitness2 = gx * hx;
	*/

	/*
	//ZDT3グレイコード
	std::vector<int> binaryCode(20, 0);
    binaryCode[0] = chrom[0]; // 最上位ビットはそのままコピー

    // グレイコードを2進数に変換
    for (int i = 1; i < 20; i++) {
        binaryCode[i] = binaryCode[i-1] ^ (int)chrom[i]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
    }

    // 2進数を10進数に変換
    int decimalValue = 0;
    for (int i = 0; i < 20; i++) {
        decimalValue += binaryCode[i] * std::pow(2, 19 - i);
    }

    // バイナリの最大値と最小値を求める
    int binaryMin = 0;
    int binaryMax = std::pow(2, 20) - 1;

    // バイナリを-kからkの実数にマッピングする
    double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);
	
	fitness1 = realValue;

	for(i = 1; i < 30; i++) {
		std::vector<int> binaryCode(20, 0);
		binaryCode[0] = chrom[i*20]; // 最上位ビットはそのままコピー

		// グレイコードを2進数に変換
		for (int j = 1; j < 20; j++) {
			binaryCode[j] = binaryCode[j-1] ^ (int)chrom[i*20+j]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
		}

		// 2進数を10進数に変換
		decimalValue = 0;
		for (int j = 0; j < 20; j++) {
			decimalValue += binaryCode[j] * std::pow(2, 19 - j);
		}

		// バイナリの最大値と最小値を求める
		binaryMin = 0;
		binaryMax = std::pow(2, 20) - 1;

		// バイナリを-kからkの実数にマッピングする
		double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);
		gx += realValue;
	}
	gx = 1 + 9 * gx / 29;
	hx = 1 - pow(fitness1 / gx, 0.5) - fitness1 / gx * sin(10 * PI * fitness1);
	fitness2 = gx * hx;
	*/

	/*
	//ZDT4
	std::vector<int> binaryCode(20, 0);
    binaryCode[0] = chrom[0]; // 最上位ビットはそのままコピー

    // グレイコードを2進数に変換
    for (int i = 1; i < 20; i++) {
        binaryCode[i] = binaryCode[i-1] ^ (int)chrom[i]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
    }

    // 2進数を10進数に変換
    int decimalValue = 0;
    for (int i = 0; i < 20; i++) {
        decimalValue += binaryCode[i] * std::pow(2, 19 - i);
    }

    // バイナリの最大値と最小値を求める
    int binaryMin = 0;
    int binaryMax = std::pow(2, 20) - 1;

    // バイナリを-kからkの実数にマッピングする
    double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);

	fitness1 = realValue;
	
	for(i = 1; i < 10; i++) {
		std::vector<int> binaryCode(20, 0);
		binaryCode[0] = chrom[i * 20]; 

		// グレイコードを2進数に変換
		for (int j = 1; j < 20; j++) {
			binaryCode[j] = binaryCode[j-1] ^ (int)chrom[i * 20 + j];
		}

		// 2進数を10進数に変換
		decimalValue = 0;
		for (int j = 0; j < 20; j++) {
			decimalValue += binaryCode[j] * std::pow(2, 19 - j);
		}

		// バイナリの最大値と最小値を求める
		binaryMin = 0;
		binaryMax = std::pow(2, 20) - 1;

		// バイナリを-kからkの実数にマッピングする
		realValue = -5 + (double)decimalValue * (2.0 * 5) / (double)(binaryMax - binaryMin);
		gx += realValue * realValue - 10 * cos(4 * PI * realValue);
	}
	gx = 91 + gx;
	hx = 1 - pow(fitness1/gx, 0.5);

	fitness2 = gx * hx;
	*/

	/*
	//Discontinuous Pareto-optimal front
	fitness1 = chrom[0]->chrom[0];
	for(i = 0; i < WCHROM_LEN; i++) {
		for(j = 0; j < PCHROM_LEN; j++){
			if(i != 0 || j != 0) {
				gx += chrom[i]->chrom[j];
			}
		}
	}
	gx = 1 + 10 * gx / (WCHROM_LEN * PCHROM_LEN - 1);
	hx = 1 - pow(fitness1 / gx, 0.25) - fitness1 / gx * sin(10 * PI * fitness1);
	fitness2 = gx * hx;
	*/

	/*
	//ZDT6
	std::vector<int> binaryCode(20, 0);
    binaryCode[0] = chrom[0]; // 最上位ビットはそのままコピー

    // グレイコードを2進数に変換
    for (int i = 1; i < 20; i++) {
        binaryCode[i] = binaryCode[i-1] ^ (int)chrom[i]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
    }

    // 2進数を10進数に変換
    int decimalValue = 0;
    for (int i = 0; i < 20; i++) {
        decimalValue += binaryCode[i] * std::pow(2, 19 - i);
    }

    // バイナリの最大値と最小値を求める
    int binaryMin = 0;
    int binaryMax = std::pow(2, 20) - 1;

    // バイナリを-kからkの実数にマッピングする
    double realValue = (double)decimalValue / (double)(binaryMax - binaryMin);

	fitness1 = 1 - exp(-4 * realValue) * pow(sin(6 * PI * realValue), 6);
	
	for(i = 1; i < 10; i++) {
		std::vector<int> binaryCode(20, 0);
		binaryCode[0] = chrom[i * 20]; 

		// グレイコードを2進数に変換
		for (int j = 1; j < 20; j++) {
			binaryCode[j] = binaryCode[j-1] ^ (int)chrom[i * 20 + j];
		}

		// 2進数を10進数に変換
		decimalValue = 0;
		for (int j = 0; j < 20; j++) {
			decimalValue += binaryCode[j] * std::pow(2, 19 - j);
		}

		// バイナリの最大値と最小値を求める
		binaryMin = 0;
		binaryMax = std::pow(2, 20) - 1;

		// バイナリを-kからkの実数にマッピングする
		realValue = (double)decimalValue / (double)(binaryMax - binaryMin);
		gx += realValue;
	}
	gx = 1 + 9 * pow(gx / 9, 0.25);
	hx = 1 - pow(fitness1/gx, 2);

	fitness2 = gx * hx;
	*/

	double realValue2;
	std::vector<int> binaryCode(20, 0);
		binaryCode[0] = chrom[0]; // 最上位ビットはそのままコピー

		// グレイコードを2進数に変換
		for (int j = 1; j < 20; j++) {
			binaryCode[j] = binaryCode[j-1] ^ (int)chrom[j]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
		}

		// 2進数を10進数に変換
		int decimalValue = 0;
		for (int j = 0; j < 20; j++) {
			decimalValue += binaryCode[j] * std::pow(2, 19 - j);
		}

		// バイナリの最大値と最小値を求める
		int binaryMin = 0;
		int binaryMax = std::pow(2, 20) - 1;

		// バイナリを-kからkの実数にマッピングする
		double realValue = -5 + (double)decimalValue * (2.0 * 5) / (double)(binaryMax - binaryMin);

	for(i = 1; i <= 100; i++) {
		if(i != 100) {
			std::vector<int> binaryCode(20, 0);
			binaryCode[0] = chrom[20 * i]; // 最上位ビットはそのままコピー

			// グレイコードを2進数に変換
			for (int j = 1; j < 20; j++) {
				binaryCode[j] = binaryCode[j-1] ^ (int)chrom[20 * i + j]; // i番目のビットは、i-1番目のビットとi番目のグレイコードをXORしたもの
			}

			// 2進数を10進数に変換
			int decimalValue = 0;
			for (int j = 0; j < 20; j++) {
				decimalValue += binaryCode[j] * std::pow(2, 19 - j);
			}

			// バイナリの最大値と最小値を求める
			int binaryMin = 0;
			int binaryMax = std::pow(2, 20) - 1;

			// バイナリを-kからkの実数にマッピングする
			realValue2 = -5 + (double)decimalValue * (2.0 * 5) / (double)(binaryMax - binaryMin);

			fitness1 += -10 * exp(-0.2 * sqrt(realValue * realValue + realValue2 * realValue2));
		}
		fitness2 += pow(abs(realValue), 0.8) + 5 * sin(realValue * realValue * realValue);
		
		realValue = realValue2;
	}
	/*
	//KUR
	fitness1 = 0;
	for(i = 0; i < WCHROM_LEN - 1; i++) {
		fitness1 += -10 * pow(2.71828, -0.2 * sqrt(chrom[i]* chrom[i] + chrom[i+1] * chrom[i+1]));
	}
	fitness2 = 0;
	for(i = 0; i < WCHROM_LEN; i++) {
			fitness2 += pow(abs(chrom[i]), 0.8) + 5 * sin(chrom[i] * chrom[i] * chrom[i]);
	}
	*/

	/*
	//多目的部分だまし問題 (PCHROM_LEN = 4)
	int zero_cnt = 0;
	int one_cnt = 0;
	for(i = 0; i < WCHROM_LEN / 4; i++) {
		for(j = 0; j < 4; j++){
			if(chrom[i * 4 + j]== 0) zero_cnt++;
			else one_cnt++;
		}
		if(one_cnt == 4) fitness1 += 5;
		else fitness1 += zero_cnt;
		if(zero_cnt == 4) fitness2 += 5;
		else fitness2 += one_cnt;
		zero_cnt = 0;
		one_cnt = 0;
	}
	fitness1 = -fitness1;
	fitness2 = -fitness2;
	*/
	//cout << fitness1 << " " << fitness2 << endl;
}
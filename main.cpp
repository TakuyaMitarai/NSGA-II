#include "nsga2.h"

// メインルーチン
int main()
{
	int i;
	float comptime;
	clock_t start;
	nsga2 *se;

	// 時間計測スタート
	start = clock();

	// 乱数のタネの設定
	srand((unsigned int)time(NULL));

	for(i = 0; i < 1; i++) {
		// 解探索
		se = new nsga2();
		se->solve(start, i);
		// 後処理
		delete se;
	}

	return 0;
}

// human1.c（このプログラムの名前）
#include <stdio.h>     // 標準入出力ヘッダのインクルード
#include "ppmlib2.h"   // 画面表示部をカットした ppmlib.h
#include "makefname.c" // 連番ファイル名の作成

// 関数のプロトタイプ宣言（makefname.c中の関数）
void make_filename(char head[], int keta, int num, char fname[]);

// main関数の始まり
int main(void)
{
    int i;           // i:入力ファイル番号
    char fname[256]; // ファイル名用配列（入出力兼用）
    int x, y, col;   // 制御変数

    // 背景画像を読み込む
    load_color_image(1, "../data/back.ppm"); // 背景画像をNo.1に読み込み

    // 各フレームを処理
    for (i = 1; i <= 200; i++)
    {
        // 連番のファイル名を生成（org00001.ppm〜org000200.ppm）
        make_filename("../data/org/org", 5, i, fname); // 5:数字の桁数
        // 画像をNo.0に読み込む
        load_color_image(0, fname);

        // 出力画像を初期化（全て白）
        width[2] = width[0];
        height[2] = height[0];
        init_color_image(2, 255, 255, 255);

        // 差分処理
        for (y = 0; y < height[0]; y++)
        {
            for (x = 0; x < width[0]; x++)
            {
                int diff_detected = 0; // 差分検出のフラグ。初期値は0（差分なし）
                for (col = 0; col < 3; col++)
                {
                    // 現在のフレーム（image[0]）と背景画像（image[1]）の色成分の差の絶対値を計算
                    int diff = abs(image[0][x][y][col] - image[1][x][y][col]);
                    // 30 以上の階調差ならば、
                    if (diff >= 30)
                    {
                        diff_detected = 1; // 差分検出フラグを立てる
                        break;
                    }
                }
                // 差分が検出されたピクセルについての処理
                if (diff_detected)
                {
                    for (col = 0; col < 3; col++)
                    {
                        image[2][x][y][col] = image[0][x][y][col]; // 元画像の値を維持
                    }
                }
                // 差分が検出されなかった場合（diff_detected == 0）は何も行わず、背景のままとなる
            }
        }
        // 出力ファイル名（out00001.ppm〜out000200.ppm）を作る
        make_filename("../data/results/out", 5, i, fname);
        // 画像の保存
        save_color_image(2, fname); // 処理後の画像を保存
    }

    printf("結果画像を out00001.ppm〜out000200.ppm で出力しました\n");
    return 0; // プログラムの正常終了
}

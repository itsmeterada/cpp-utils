#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define C (10)
#define H (10)
#define W (10)
#define K (3)

int
main(int argc, char **argv)
{
  float (*input)[H][W];
  float (*filter)[C][K][K];
  float (*estimated)[H][W];

  float (*v)[4][C][H / 2][W / 2];
  float (*u)[4][C][C];
  float (*uv)[4][C][H / 2][W / 2];
  float (*actual)[H][W];

  actual = malloc(C * H * W * sizeof(float));

  /* 初期化 */
  srand(0);
  input = malloc(C * H * W * sizeof(float));
  for (ptrdiff_t c = 0; c < C; ++c) {
    for (ptrdiff_t h = 0; h < H; ++h) {
      for (ptrdiff_t w = 0; w < W; ++w) {
        input[c][h][w] = (float) rand() / (float) RAND_MAX;
      }
    }
  }
  filter = malloc(C * C * K * K * sizeof(float));
  for (ptrdiff_t oc = 0; oc < C; ++oc) {
    for (ptrdiff_t ic = 0; ic < C; ++ic) {
      for (ptrdiff_t kh = 0; kh < K; ++kh) {
        for (ptrdiff_t kw = 0; kw < K; ++kw) {
          filter[oc][ic][kh][kw] = (float) rand() / (float) RAND_MAX;
        }
      }
    }
  }

  /* ベタな Convolution 計算 */
  estimated = malloc(C * H * W * sizeof(float));
  for (ptrdiff_t oc = 0; oc < C; ++oc) {
    for (ptrdiff_t h = 0; h < H; ++h) {
      for (ptrdiff_t w = 0; w < W; ++w) {
        float value = 0.0f;
        for (ptrdiff_t ic = 0; ic < C; ++ic) {
          for (ptrdiff_t kh = 0; kh < K; ++kh) {
            ptrdiff_t ih = h + kh - 1;
            if (ih < 0 || ih >= H) {
              continue;
            }
            for (ptrdiff_t kw = 0; kw < K; ++kw) {
              ptrdiff_t iw = w + kw - 1;
              if (iw < 0 || iw >= W) {
                continue;
              }
              value += input[ic][ih][iw] * filter[oc][ic][kh][kw];
            }
          }
        }
        estimated[oc][h][w] = value;
      }
    }
  }

  /* Winograd 入力変換 */
  /* B =
   * |  1  0  0  0 |
   * |  0  1 -1 -1 |
   * | -1  1  1  0 |
   * |  0  0  0  1 |
   * V = Bt d B
   */
  v = malloc(4 * 4 * C * (H / 2) * (W / 2) * sizeof(float));
  for (ptrdiff_t c = 0; c < C; ++c) {
    for (ptrdiff_t h = 0; h < H / 2; ++h) {
      for (ptrdiff_t w = 0; w < W / 2; ++w) {
        float d[4][4];
        for (ptrdiff_t i = 0; i < 4; ++i) {
          ptrdiff_t ih = h * 2 + i - 1;
          if (ih < 0 || ih >= H) {
            for (ptrdiff_t j = 0; j < 4; ++j) {
              d[i][j] = 0.0f;
            }
            continue;
          }
          for (ptrdiff_t j = 0; j < 4; ++j) {
            ptrdiff_t iw = w * 2 + j - 1;
            if (iw < 0 || iw >= H) {
              d[i][j] = 0.0f;
              continue;
            }
            d[i][j] = input[c][ih][iw];
          }
        }
        for (ptrdiff_t i = 0; i < 4; ++i) {
          for (ptrdiff_t j = 0; j < 4; ++j) {
            float value = 0.0f;
            for (ptrdiff_t k = 0; k < 4; ++k) {
              for (ptrdiff_t l = 0; l < 4; ++l) {
                static float B[4][4] = { {  1.0f,  0.0f,  0.0f,  0.0f },
                                         {  0.0f,  1.0f, -1.0f, -1.0f },
                                         { -1.0f,  1.0f,  1.0f,  0.0f },
                                         {  0.0f,  0.0f,  0.0f,  1.0f } };
                value += B[k][i] * B[l][j] * d[k][l];
              }
            }
            v[i][j][c][h][w] = value;
          }
        }
      }
    }
  }

  /* Winograd フィルタ変換 */
  /* G =
   * |   1    0    0  |
   * |  1/2  1/2  1/2 |
   * |  1/2 -1/2  1/2 |
   * |   0    0    1  |
   * U = G g Gt
   */
  u = malloc(4 * 4 * C * C * sizeof(float));
  for (ptrdiff_t oc = 0; oc < C; ++oc) {
    for (ptrdiff_t ic = 0; ic < C; ++ic) {
      float g[3][3];
      for (ptrdiff_t i = 0; i < 3; ++i) {
        for (ptrdiff_t j = 0; j < 3; ++j) {
          g[i][j] = filter[oc][ic][i][j];
        }
      }
      for (ptrdiff_t i = 0; i < 4; ++i) {
        for (ptrdiff_t j = 0; j < 4; ++j) {
          float value = 0.0f;
          for (ptrdiff_t k = 0; k < 3; ++k) {
            for (ptrdiff_t l = 0; l < 3; ++l) {
              static float G[4][3] = { {  1.0f,  0.0f,  0.0f },
                                       {  0.5f,  0.5f,  0.5f },
                                       {  0.5f, -0.5f,  0.5f },
                                       {  0.0f,  0.0f,  1.0f } };
              value += G[i][k] * G[j][l] * g[k][l];
            }
          }
          u[i][j][oc][ic] = value;
        }
      }
    }
  }

  /* Winograd 行列積 */
  uv = malloc(4 * 4 * C * (H / 2) * (W / 2) * sizeof(float));
  for (ptrdiff_t i = 0; i < 4; ++i) {
    for (ptrdiff_t j = 0; j < 4; ++j) {
      for (ptrdiff_t oc = 0; oc < C; ++oc) {
        for (ptrdiff_t ih = 0; ih < (H / 2); ++ih) {
          for (ptrdiff_t iw = 0; iw < (W / 2); ++iw) {
            float value = 0.0f;
            for (ptrdiff_t ic = 0; ic < C; ++ic) {
              value += u[i][j][oc][ic] * v[i][j][ic][ih][iw];
            }
            uv[i][j][oc][ih][iw] = value;
          }
        }
      }
    }
  }

  /* Winograd 出力変換 */
  /* A =
   * |  1  0 |
   * |  1  1 |
   * |  1 -1 |
   * |  0  1 |
   * Y = At m A
   */
  actual = malloc(C * H * W * sizeof(float));
  for (ptrdiff_t c = 0; c < C; ++c) {
    for (ptrdiff_t h = 0; h < H / 2; ++h) {
      for (ptrdiff_t w = 0; w < W / 2; ++w) {
        float m[4][4];
        for (ptrdiff_t i = 0; i < 4; ++i) {
          for (ptrdiff_t j = 0; j < 4; ++j) {
            m[i][j] = uv[i][j][c][h][w];
          }
        }
        for (ptrdiff_t i = 0; i < 2; ++i) {
          for (ptrdiff_t j = 0; j < 2; ++j) {
            float value = 0.0f;
            for (ptrdiff_t k = 0; k < 4; ++k) {
              for (ptrdiff_t l = 0; l < 4; ++l) {
                static float A[4][2] = { {  1.0f,  0.0f },
                                         {  1.0f,  1.0f },
                                         {  1.0f, -1.0f },
                                         {  0.0f,  1.0f } };
                value += A[k][i] * A[l][j] * m[k][l];
              }
            }
            actual[c][h * 2 + i][w * 2 + j] = value;
          }
        }
      }
    }
  }

  /* 出力比較 */
  printf("Estimated\tActual\t\tDiff\n");
  double maxdiff = 0.0f;
  for (ptrdiff_t c = 0; c < C; ++c) {
    for (ptrdiff_t h = 0; h < H; ++h) {
      for (ptrdiff_t w = 0; w < W; ++w) {
        float estimatedi = estimated[c][h][w];
        float actuali = actual[c][h][w];
        double diffi = fabs((double) estimatedi - (double) actuali);
        printf("%e\t%e\t%e\n", estimatedi, actuali, diffi);
        maxdiff = fmax(diffi, maxdiff);
      }
    }
  }
  printf("Max difference:\t%e\n", maxdiff);
  
  return 0;
}

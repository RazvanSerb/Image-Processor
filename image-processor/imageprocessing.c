#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#define MAX_RGB 255

int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M / 2; j++) {
            int *aux = image[i][j];
            image[i][j] = image[i][(M - 1) - j];
            image[i][(M - 1) - j] = aux;
        }
    return image;
}

int ***rotate_left(int ***image, int N, int M) {
    int ***new_image = (int ***)malloc(M * sizeof(int **));
    for (int new_i = 0; new_i < M; new_i++) {
        new_image[new_i] = (int **)malloc(N * sizeof(int *));
        for (int new_j = 0; new_j < N; new_j++)
            new_image[new_i][new_j] = image[new_j][(M - 1) - new_i];
    }
    for (int i = 0; i < N; i++)
        free(image[i]);
    free(image);
    return new_image;
}

int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int ***new_image = (int ***)malloc(h * sizeof(int **));
    for (int new_i = 0; new_i < h; new_i++) {
        new_image[new_i] = (int **)malloc(w * sizeof(int *));
        for (int new_j = 0; new_j < w; new_j++) {
            new_image[new_i][new_j] = image[y + new_i][x + new_j];
            image[y + new_i][x + new_j] = NULL;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            if (image[i][j])
                free(image[i][j]);
        free(image[i]);
    }
    free(image);
    return new_image;
}

int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int ***new_image = (int ***)malloc((rows + N + rows) * sizeof(int **));
    for (int new_i = 0; new_i < rows; new_i++) {
        new_image[new_i] = (int **)malloc((cols + M + cols) * sizeof(int *));
        for (int new_j = 0; new_j < cols + M + cols; new_j++) {
            new_image[new_i][new_j] = (int *)malloc(3 * sizeof(int));
            new_image[new_i][new_j][0] = new_R;
            new_image[new_i][new_j][1] = new_G;
            new_image[new_i][new_j][2] = new_B;
        }
    }
    for (int new_i = rows; new_i < rows + N; new_i++) {
        new_image[new_i] = (int **)malloc((cols + M + cols) * sizeof(int *));
        for (int new_j = 0; new_j < cols; new_j++) {
            new_image[new_i][new_j] = (int *)malloc(3 * sizeof(int));
            new_image[new_i][new_j][0] = new_R;
            new_image[new_i][new_j][1] = new_G;
            new_image[new_i][new_j][2] = new_B;
        }
        for (int new_j = cols; new_j < cols + M; new_j++)
            new_image[new_i][new_j] = image[new_i - rows][new_j - cols];
        for (int new_j = cols + M; new_j < cols + M + cols; new_j++) {
            new_image[new_i][new_j] = (int *)malloc(3 * sizeof(int));
            new_image[new_i][new_j][0] = new_R;
            new_image[new_i][new_j][1] = new_G;
            new_image[new_i][new_j][2] = new_B;
        }
    }
    for (int new_i = rows + N; new_i < rows + N + rows; new_i++) {
        new_image[new_i] = (int **)malloc((cols + M + cols) * sizeof(int *));
        for (int new_j = 0; new_j < cols + M + cols; new_j++) {
            new_image[new_i][new_j] = (int *)malloc(3 * sizeof(int));
            new_image[new_i][new_j][0] = new_R;
            new_image[new_i][new_j][1] = new_G;
            new_image[new_i][new_j][2] = new_B;
        }
    }
    for (int i = 0; i < N; i++)
        free(image[i]);
    free(image);
    return new_image;
}

int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
    for (int i = y; i < N_dst; i++) {
        if (i - y >= N_src)
            break;
        for (int j = x; j < M_dst; j++) {
            if (j - x >= M_src)
                break;
            image_dst[i][j][0] = image_src[i - y][j - x][0];
            image_dst[i][j][1] = image_src[i - y][j - x][1];
            image_dst[i][j][2] = image_src[i - y][j - x][2];
        }
    }
    return image_dst;
}

int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int ***new_image = (int ***)malloc(N * sizeof(int **));
    for (int new_i = 0; new_i < N; new_i++) {
        new_image[new_i] = (int **)malloc(M * sizeof(int *));
        for (int new_j = 0; new_j < M; new_j++) {
            new_image[new_i][new_j] = (int *)malloc(3 * sizeof(int));
            float new_R = 0, new_G = 0, new_B = 0;
            for (int filter_i = 0; filter_i < filter_size; filter_i++)
                for (int filter_j = 0; filter_j < filter_size; filter_j++) {
                    int i = new_i - filter_size / 2 + filter_i;
                    int j = new_j - filter_size / 2 + filter_j;
                    if (i >= 0 && i < N && j >= 0 && j < M) {
                        new_R += (float)image[i][j][0] * filter[filter_i][filter_j];
                        new_G += (float)image[i][j][1] * filter[filter_i][filter_j];
                        new_B += (float)image[i][j][2] * filter[filter_i][filter_j];
                    }
                }
            new_R = new_R < 0 ? 0 : new_R;
            new_R = new_R > MAX_RGB ? MAX_RGB : new_R;
            new_image[new_i][new_j][0] = (int)new_R;
            new_G = new_G < 0 ? 0 : new_G;
            new_G = new_G > MAX_RGB ? MAX_RGB : new_G;
            new_image[new_i][new_j][1] = (int)new_G;
            new_B = new_B < 0 ? 0 : new_B;
            new_B = new_B > MAX_RGB ? MAX_RGB : new_B;
            new_image[new_i][new_j][2] = (int)new_B;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            free(image[i][j]);
        free(image[i]);
    }
    free(image);
    return new_image;
}

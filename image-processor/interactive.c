#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"
#define BUF_SIZE 100

typedef struct {
    int ***image;
    int N;
    int M;
} TImage;

typedef struct {
    float **filter;
    int filter_size;
} TFilter;

int main() {
    TImage *images = NULL;
    int images_length = 0;
    TFilter *filters = NULL;
    int filters_length = 0;
    while (1) {
        char command[3]; scanf("%s", command);
        if (!strcmp(command, "e")) {
            for (int index = 0; index < filters_length; index++) {
                for (int i = 0; i < filters[index].filter_size; i++)
                    free(filters[index].filter[i]);
                free(filters[index].filter);
            }
            free(filters);
            for (int index = 0; index < images_length; index++) {
                for (int i = 0; i < images[index].N; i++) {
                    for (int j = 0; j < images[index].M; j++)
                        free(images[index].image[i][j]);
                    free(images[index].image[i]);
                }
                free(images[index].image);
            }
            free(images);
            break;
        } else if (!strcmp(command, "l")) {
            int N = 0; scanf("%d", &N);
            int M = 0; scanf("%d", &M);
            int ***image = (int ***)malloc(N * sizeof(int **));
            for (int i = 0; i < N; i++) {
                image[i] = (int **)malloc(M * sizeof(int *));
                for (int j = 0; j < M; j++)
                  image[i][j] = (int *)malloc(3 * sizeof(int));
            }
            char path[BUF_SIZE]; scanf("%s", path);
            read_from_bmp(image, N, M, path);
            images_length++;
            images = (TImage *)realloc(images, images_length * sizeof(TImage));
            images[images_length - 1].image = image;
            images[images_length - 1].N = N;
            images[images_length - 1].M = M;
        } else if (!strcmp(command, "s")) {
            int index = 0; scanf("%d", &index);
            char path[BUF_SIZE]; scanf("%s", path);
            write_to_bmp(images[index].image, images[index].N, images[index].M, path);
        } else if (!strcmp(command, "ah")) {
            int index = 0; scanf("%d", &index);
            images[index].image = flip_horizontal(images[index].image, images[index].N, images[index].M);
        } else if (!strcmp(command, "ar")) {
            int index = 0; scanf("%d", &index);
            images[index].image = rotate_left(images[index].image, images[index].N, images[index].M);
            int aux = images[index].N;
            images[index].N = images[index].M;
            images[index].M = aux;
        } else if (!strcmp(command, "ac")) {
            int index = 0; scanf("%d", &index);
            int x = 0; scanf("%d", &x);
            int y = 0; scanf("%d", &y);
            int w = 0; scanf("%d", &w);
            int h = 0; scanf("%d", &h);
            images[index].image = crop(images[index].image, images[index].N, images[index].M, x, y, h, w);
            images[index].N = h;
            images[index].M = w;
        } else if (!strcmp(command, "ae")) {
            int index = 0; scanf("%d", &index);
            int rows = 0; scanf("%d", &rows);
            int cols = 0; scanf("%d", &cols);
            int R = 0; scanf("%d", &R);
            int G = 0; scanf("%d", &G);
            int B = 0; scanf("%d", &B);
            images[index].image = extend(images[index].image, images[index].N, images[index].M, rows, cols, R, G, B);
            images[index].N = rows + images[index].N + rows;
            images[index].M = cols + images[index].M + cols;
        } else if (!strcmp(command, "ap")) {
            int index_dst = 0; scanf("%d", &index_dst);
            int index_src = 0; scanf("%d", &index_src);
            int x = 0; scanf("%d", &x);
            int y = 0; scanf("%d", &y);
            images[index_dst].image = paste(images[index_dst].image, images[index_dst].N, images[index_dst].M, \
                                            images[index_src].image, images[index_src].N, images[index_src].M, x, y);
        } else if (!strcmp(command, "cf")) {
            int filter_size = 0; scanf("%d", &filter_size);
            float **filter = (float **)malloc(filter_size * sizeof(float *));
            for (int i = 0; i < filter_size; i++) {
                filter[i] = (float *)malloc(filter_size * sizeof(float));
                for (int j = 0; j < filter_size; j++)
                    scanf("%f", &filter[i][j]);
            }
            filters_length++;
            filters = (TFilter *)realloc(filters, filters_length * sizeof(TFilter));
            filters[filters_length - 1].filter = filter;
            filters[filters_length - 1].filter_size = filter_size;
        } else if (!strcmp(command, "af")) {
            int index_img = 0; scanf("%d", &index_img);
            int index_filter = 0; scanf("%d", &index_filter);
            images[index_img].image = apply_filter(images[index_img].image, images[index_img].N, images[index_img].M, \
                                                   filters[index_filter].filter, filters[index_filter].filter_size);
        } else if (!strcmp(command, "df")) {
            int index = 0; scanf("%d", &index);
            for (int i = 0; i < filters[index].filter_size; i++)
                free(filters[index].filter[i]);
            free(filters[index].filter);
            for (int k = index; k < filters_length - 1; k++)
                filters[k] = filters[k + 1];
            filters_length--;
            filters = (TFilter *)realloc(filters, filters_length * sizeof(TFilter));
        } else if (!strcmp(command, "di")) {
            int index = 0; scanf("%d", &index);
            for (int i = 0; i < images[index].N; i++) {
                for (int j = 0; j < images[index].M; j++)
                    free(images[index].image[i][j]);
                free(images[index].image[i]);
            }
            free(images[index].image);
            for (int k = index; k < images_length - 1; k++)
                images[k] = images[k + 1];
            images_length--;
            images = (TImage *)realloc(images, images_length * sizeof(TImage));
        }
    }
    return 0;
}

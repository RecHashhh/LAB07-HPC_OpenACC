#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <openacc.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080
#define MAX_ITER 1000

void mandelbrot(float *image) {
    #pragma acc parallel loop gang vector
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float complex c = (x - WIDTH / 2.0) * 4.0 / WIDTH +
                              ((y - HEIGHT / 2.0) * 4.0 / HEIGHT) * I;
            float complex z = 0;
            int iter = 0;
            while (cabs(z) < 2.0 && iter < MAX_ITER) {
                z = z * z + c;
                iter++;
            }
            image[y * WIDTH + x] = (float)iter / MAX_ITER;
        }
    }
}

void save_image(const char *filename, float *image) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    fprintf(f, "P3\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int color = (int)(image[y * WIDTH + x] * 255);
            fprintf(f, "%d %d %d ", color, color, color); // Escala de grises
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main() {
    // Measure time
    clock_t start, end;
    start = clock();

    float *image = (float *)malloc(WIDTH * HEIGHT * sizeof(float));

    // Mandelbrot generation
    mandelbrot(image);

    // Save the image to file
    save_image("mandelbrot_acc.ppm", image);

    // Free memory
    free(image);

    // Measure time
    end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Calculation Time: %f seconds\n", time_taken);
    printf("Memory used: %lu bytes\n", WIDTH * HEIGHT * sizeof(float));

    return 0;
}


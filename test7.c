#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Error: file argument needed!\n");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Error: could not open file \"%s\"\n", argv[1]);
        exit(1);
    }

    const int BUFSIZE = 65536;
    char buf[BUFSIZE], ch;
    // Initialize histogram array
    int hist[256] = {0}, most = -1;
    size_t total_mem = 0;

    // Start timing
    clock_t start_time = clock();

    // Compute histogram
    while (1) {
        size_t size = fread(buf, 1, BUFSIZE, f);
        if (size <= 0) break;
        
        total_mem += size; // Actualizar memoria utilizada

        #pragma acc parallel loop copyin(buf[:size])
        for (int i = 0; i < size; i++) {
            int v = buf[i];
            // Solo cuenta si el carácter es un dígito (número entre 0-9)
            if (v >= '0' && v <= '9') {
                #pragma acc atomic
                hist[v]++;
            }
        }
    }

    // Stop timing
    clock_t end_time = clock();
    double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the first highest peak
    for (int i = 0; i < 256; i++) {
        if (hist[i] > most) {
            most = hist[i];
            ch = i;
        }
    }

    printf("Highest count of %d for character code %d\n", most, ch);
    printf("Calculation Time: %f seconds\n", cpu_time_used);
    printf("Memory used: %zu bytes (file size)\n", total_mem);
    
    fclose(f); // Cerrar el archivo al final
    return 0;
}

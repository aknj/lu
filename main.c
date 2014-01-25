#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>

void print_matrix(gsl_matrix *m) {
    int i;
    int j;
    for(i = 0; i < m->size1; i++) {
        for(j = 0; j < m->size2; j++) {
            printf("\t[ %g ] ", gsl_matrix_get(m, i, j));
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int rn, cn;
    FILE *in = argc > 1 ? fopen(argv[1], "r") : NULL;

    if(in == NULL) {
        if(argc > 1)
            fprintf(stderr, "%s: problem z otwarciem pliku %s.\n", argv[0], argv[1]);
        else
            fprintf(stderr, "%s: prosze podac plik z danymi.\n", argv[0]);
        return 1;
    }

    fscanf(in, "%d %d", &rn, &cn);
    gsl_matrix matrix = *gsl_matrix_alloc(rn, cn);
    gsl_matrix_fscanf(in, &matrix);

    gsl_matrix_view A = gsl_matrix_submatrix(&matrix, 0, 0, rn, cn-1);
    gsl_matrix_view b_tmp = gsl_matrix_submatrix(&matrix, 0, cn-1, rn, 1);

#ifdef DEBUG
    printf("A = ");
    print_matrix(&A.matrix);                                        // wyswietlanie macierzy
    printf("b = ");
    print_matrix(&b_tmp.matrix);
#endif

    gsl_vector b = *gsl_vector_alloc(rn);
    for(int i = 0; i < rn; i++)                                // przepisanie macierzy b_tmp do wektora b
        gsl_vector_set(&b, i, gsl_matrix_get(&b_tmp.matrix, i, 0));

    gsl_vector x = *gsl_vector_alloc(rn);
    int s;
    gsl_permutation p = *gsl_permutation_alloc(rn);

    gsl_linalg_LU_decomp(&A.matrix, &p, &s);
    gsl_linalg_LU_solve(&A.matrix, &p, &b, &x);

    printf("x = ");
    gsl_vector_fprintf(stdout, &x, "\t[ %g ]");
    
    return 0;
}

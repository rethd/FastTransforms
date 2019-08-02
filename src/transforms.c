// Computational routines for one-dimensional orthogonal polynomial transforms.

#include "fasttransforms.h"
#include "ftinternal.h"

double * plan_legendre_to_chebyshev(const int normleg, const int normcheb, const int n) {
    ft_triangular_bandedl * A = ft_create_A_legendre_to_chebyshevl(n);
    ft_triangular_bandedl * B = ft_create_B_legendre_to_chebyshevl(n);
    long double * Vl = calloc(n*n, sizeof(long double));
    if (n > 0)
        Vl[0] = 1;
    if (n > 1)
        Vl[1+n] = 1;
    for (int i = 2; i < n; i++)
        Vl[i+i*n] = (2*i-1)*Vl[i-1+(i-1)*n]/(2*i);
    ft_triangular_banded_eigenvectorsl(A, B, Vl);
    double * V = calloc(n*n, sizeof(double));
    long double * sclrow = calloc(n, sizeof(long double));
    long double * sclcol = calloc(n, sizeof(long double));
    for (int i = 0; i < n; i++) {
        sclrow[i] = normcheb ? i ? sqrtl(M_PI_2l) : sqrtl(M_PIl) : 1.0L;
        sclcol[i] = normleg ? sqrtl(i+0.5L) : 1.0L;
    }
    for (int j = 0; j < n; j++)
        for (int i = j; i >= 0; i -= 2)
            V[i+j*n] = sclrow[i]*Vl[i+j*n]*sclcol[j];
    ft_destroy_triangular_bandedl(A);
    ft_destroy_triangular_bandedl(B);
    free(Vl);
    free(sclrow);
    free(sclcol);
    return V;
}

double * plan_chebyshev_to_legendre(const int normcheb, const int normleg, const int n) {
    ft_triangular_bandedl * A = ft_create_A_chebyshev_to_legendrel(n);
    ft_triangular_bandedl * B = ft_create_B_chebyshev_to_legendrel(n);
    long double * Vl = calloc(n*n, sizeof(long double));
    if (n > 0)
        Vl[0] = 1;
    if (n > 1)
        Vl[1+n] = 1;
    for (int i = 2; i < n; i++)
        Vl[i+i*n] = (2*i)*Vl[i-1+(i-1)*n]/(2*i-1);
    ft_triangular_banded_eigenvectorsl(A, B, Vl);
    double * V = calloc(n*n, sizeof(double));
    long double * sclrow = calloc(n, sizeof(long double));
    long double * sclcol = calloc(n, sizeof(long double));
    for (int i = 0; i < n; i++) {
        sclrow[i] = normleg ? 1.0L/sqrtl(i+0.5L) : 1.0L;
        sclcol[i] = normcheb ? i ? sqrtl(M_2_PIl) : sqrtl(M_1_PIl) : 1.0L;
    }
    for (int j = 0; j < n; j++)
        for (int i = j; i >= 0; i -= 2)
            V[i+j*n] = sclrow[i]*Vl[i+j*n]*sclcol[j];
    ft_destroy_triangular_bandedl(A);
    ft_destroy_triangular_bandedl(B);
    free(Vl);
    free(sclrow);
    free(sclcol);
    return V;
}

double * plan_ultraspherical_to_ultraspherical(const int norm1, const int norm2, const int n, const double lambda, const double mu) {
    ft_triangular_bandedl * A = ft_create_A_ultraspherical_to_ultrasphericall(n, lambda, mu);
    ft_triangular_bandedl * B = ft_create_B_ultraspherical_to_ultrasphericall(n, mu);
    long double * Vl = calloc(n*n, sizeof(long double));
    long double lambdal = lambda, mul = mu;
    if (n > 0)
        Vl[0] = 1;
    if (n > 1)
        Vl[1+n] = lambdal/mul;
    for (int i = 2; i < n; i++)
        Vl[i+i*n] = (i-1+lambdal)*Vl[i-1+(i-1)*n]/(i-1+mul);
    ft_triangular_banded_eigenvectorsl(A, B, Vl);
    double * V = calloc(n*n, sizeof(double));
    long double * sclrow = calloc(n, sizeof(long double));
    long double * sclcol = calloc(n, sizeof(long double));
    if (n > 0) {
        sclrow[0] = norm2 ? sqrtl(sqrtl(M_PIl)*tgammal(mul+0.5L)/tgammal(mul+1)) : 1.0L;
        sclcol[0] = norm1 ? sqrtl(tgammal(lambdal+1)/(sqrtl(M_PIl)*tgammal(lambdal+0.5L))) : 1.0L;
    }
    for (int i = 1; i < n; i++) {
        sclrow[i] = norm2 ? sqrtl((i-1+mul)/i*(i-1+2*mul)/(i+mul))*sclrow[i-1] : 1.0L;
        sclcol[i] = norm1 ? sqrtl(i/(i-1+lambdal)*(i+lambdal)/(i-1+2*lambdal))*sclcol[i-1] : 1.0L;
    }
    for (int j = 0; j < n; j++)
        for (int i = j; i >= 0; i -= 2)
            V[i+j*n] = sclrow[i]*Vl[i+j*n]*sclcol[j];
    ft_destroy_triangular_bandedl(A);
    ft_destroy_triangular_bandedl(B);
    free(Vl);
    free(sclrow);
    free(sclcol);
    return V;
}

double * plan_jacobi_to_jacobi(const int norm1, const int norm2, const int n, const double alpha, const double beta, const double gamma, const double delta) {
    ft_triangular_bandedl * A = ft_create_A_jacobi_to_jacobil(n, alpha, beta, gamma, delta);
    ft_triangular_bandedl * B = ft_create_B_jacobi_to_jacobil(n, gamma, delta);
    long double alphal = alpha, betal = beta, gammal = gamma, deltal = delta;
    long double * Vl = calloc(n*n, sizeof(long double));
    if (n > 0)
        Vl[0] = 1;
    if (n > 1)
        Vl[1+n] = (alphal+betal+2)/(gammal+deltal+2);
    for (int i = 2; i < n; i++)
        Vl[i+i*n] = (2*i+alphal+betal-1)/(i+alphal+betal)*(2*i+alphal+betal)/(2*i+gammal+deltal-1)*(i+gammal+deltal)/(2*i+gammal+deltal)*Vl[i-1+(i-1)*n];
    ft_triangular_banded_eigenvectorsl(A, B, Vl);
    double * V = calloc(n*n, sizeof(double));
    long double * sclrow = calloc(n, sizeof(long double));
    long double * sclcol = calloc(n, sizeof(long double));
    if (n > 0) {
        sclrow[0] = norm2 ? sqrtl(powl(2.0L, gammal+deltal+1)*tgammal(gammal+1)*tgammal(deltal+1)/tgammal(gammal+deltal+2)) : 1.0L;
        sclcol[0] = norm1 ? sqrtl(tgammal(alphal+betal+2)/(powl(2.0L, alphal+betal+1)*tgammal(alphal+1)*tgammal(betal+1))) : 1.0L;
    }
    if (n > 1) {
        sclrow[1] = norm2 ? sqrtl((gammal+1)*(deltal+1)/(gammal+deltal+3))*sclrow[0] : 1.0L;
        sclcol[1] = norm1 ? sqrtl((alphal+betal+3)/(alphal+1)/(betal+1))*sclcol[0] : 1.0L;
    }
    for (int i = 2; i < n; i++) {
        sclrow[i] = norm2 ? sqrtl((i+gammal)/i*(i+deltal)/(i+gammal+deltal)*(2*i+gammal+deltal-1)/(2*i+gammal+deltal+1))*sclrow[i-1] : 1.0L;
        sclcol[i] = norm1 ? sqrtl(i/(i+alphal)*(i+alphal+betal)/(i+betal)*(2*i+alphal+betal+1)/(2*i+alphal+betal-1))*sclcol[i-1] : 1.0L;
    }
    for (int j = 0; j < n; j++)
        for (int i = 0; i <= j; i++)
            V[i+j*n] = sclrow[i]*Vl[i+j*n]*sclcol[j];
    ft_destroy_triangular_bandedl(A);
    ft_destroy_triangular_bandedl(B);
    free(Vl);
    free(sclrow);
    free(sclcol);
    return V;
}

double * plan_associated_jacobi_to_jacobi(const int norm2, const int n, const int c, const double alpha, const double beta, const double gamma, const double delta) {
    ft_triangular_bandedl * A = ft_create_A_associated_jacobi_to_jacobil(n, c, alpha, beta, gamma, delta);
    ft_triangular_bandedl * B = ft_create_B_associated_jacobi_to_jacobil(n, gamma, delta);
    long double alphal = alpha, betal = beta, gammal = gamma, deltal = delta;
    long double * Vl = calloc(n*n, sizeof(long double));
    if (n > 0)
        Vl[0] = 1;
    if (n > 1)
        Vl[1+n] = (alphal+betal+2*c+1)/(alphal+betal+c+1)*(alphal+betal+2)/(gammal+deltal+2);
    for (int i = 2; i < n; i++)
        Vl[i+i*n] = (2*i+alphal+betal+2*c-1)/(i+alphal+betal+c)*(2*i+alphal+betal+2*c)/(2*i+gammal+deltal-1)*(i+gammal+deltal)/(2*i+gammal+deltal)*Vl[i-1+(i-1)*n];
    ft_triangular_banded_eigenvectorsl(A, B, Vl);
    double * V = calloc(n*n, sizeof(double));
    long double * sclrow = calloc(n, sizeof(long double));
    if (n > 0)
        sclrow[0] = norm2 ? sqrtl(powl(2.0L, gammal+deltal+1)*tgammal(gammal+1)*tgammal(deltal+1)/tgammal(gammal+deltal+2)) : 1.0L;
    if (n > 1)
        sclrow[1] = norm2 ? sqrtl((gammal+1)*(deltal+1)/(gammal+deltal+3))*sclrow[0] : 1.0L;
    for (int i = 2; i < n; i++)
        sclrow[i] = norm2 ? sqrtl((i+gammal)/i*(i+deltal)/(i+gammal+deltal)*(2*i+gammal+deltal-1)/(2*i+gammal+deltal+1))*sclrow[i-1] : 1.0L;
    for (int j = 0; j < n; j++)
        for (int i = 0; i <= j; i++)
            V[i+j*n] = sclrow[i]*Vl[i+j*n];
    ft_destroy_triangular_bandedl(A);
    ft_destroy_triangular_bandedl(B);
    free(Vl);
    free(sclrow);
    return V;
}

double * plan_konoplev_to_jacobi(const int n, const double alpha, const double beta) {
    ft_triangular_bandedl * A = ft_create_A_konoplev_to_jacobil(n, alpha, beta);
    ft_triangular_bandedl * B = ft_create_B_konoplev_to_jacobil(n, alpha);
    long double alphal = alpha, betal = beta;
    long double * Vl = calloc(n*n, sizeof(long double));
    for (int i = 0; i < n; i++)
        Vl[i+i*n] = 1;
    //if (n > 0)
    //    Vl[0] = 1;
    //if (n > 1)
    //    Vl[1+n] = (alphal+betal+2)/(gammal+deltal+2);
    //for (int i = 2; i < n; i++)
    //    Vl[i+i*n] = (2*i+alphal+betal-1)/(i+alphal+betal)*(2*i+alphal+betal)/(2*i+gammal+deltal-1)*(i+gammal+deltal)/(2*i+gammal+deltal)*Vl[i-1+(i-1)*n];
    ft_triangular_banded_eigenvectorsl(A, B, Vl);
    double * V = calloc(n*n, sizeof(double));
    for (int j = 0; j < n; j++)
        for (int i = j; i >= 0; i -= 2)
            V[i+j*n] = Vl[i+j*n];
    /*
    double * sclrow = calloc(n, sizeof(double));
    double * sclcol = calloc(n, sizeof(double));
    for (int i = 0; i < n; i++) {
        sclrow[i] = norm2 ? sqrt(Analphabeta(i, gamma, delta)) : 1.0;
        sclcol[i] = norm1 ? 1.0/sqrt(Analphabeta(i, alpha, beta)) : 1.0;
    }
    for (int j = 0; j < n; j++)
        for (int i = 0; i <= j; i++)
            V[i+j*n] = sclrow[i]*Vl[i+j*n]*sclcol[j];
    */
    ft_destroy_triangular_bandedl(A);
    ft_destroy_triangular_bandedl(B);
    free(Vl);
    //free(sclrow);
    //free(sclcol);
    return V;
}

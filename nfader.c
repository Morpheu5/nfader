#include <stdlib.h>
#include <math.h>
#include "m_pd.h"

static t_class *nfader_tilde_class;

typedef struct _nfader_tilde {
    t_object x_obj;
    t_float x_f;
    t_int n;
    t_float **signals;
    t_inlet **inlets;
    t_outlet *out;
} t_nfader_tilde;

void *nfader_new(t_floatarg n) {
    t_nfader_tilde *x = (t_nfader_tilde *)pd_new(nfader_tilde_class);

    x->n = (t_int)floor(n < 2 ? 2 : n);
    x->signals = (t_float **)getbytes((x->n + 1) * sizeof(t_float *));
    x->inlets = (t_inlet **)getbytes((x->n + 1) * sizeof(t_inlet *));
    for (int i = 0; i < x->n+1; ++i) {
        x->inlets[i] = signalinlet_new((t_object *)x, 0.0f);
    }
    x->out = outlet_new((t_object *)x, &s_signal);

    return (void *)x;
}

void nfader_free(t_nfader_tilde *x) {
    for (int i = 0; i < x->n+1; ++i) {
        inlet_free((t_inlet *)x->inlets[i]);
    }
    freebytes(x->inlets, (x->n+1) * sizeof(t_inlet *));
    freebytes(x->signals, (x->n+1) * sizeof(t_float *));
    outlet_free(x->out);
}

t_int *nfader_tilde_perform(t_int *w) {
    t_nfader_tilde *x = (t_nfader_tilde *)w[1];
    t_sample *out = (t_sample *)w[2];
    int bs = (int)w[3];

    long n = x->n;
    for (int i = 0; i < bs; ++i) {
#if PD_FLOATSIZE == 32
        t_float t = fmaxf(0, fminf(x->signals[0][i], 1));
        t_float nt = (n-1)*t;
        t_float ft = fmodf(nt, 1);
        long a = (long)floor(fminf(nt, n-1)) + 1;
#elif PD_FLOATSIZE == 64
        t_float t = fmax(0, fmin(x->signals[0][i], 1));
        t_float nt = (n-1)*t;
        t_float ft = fmod(nt, 1);
        long a = (long)floor(fmin(nt, n-1)) + 1;
#else
#error "PD_FLOATSIZE is neither 32 nor 64 bits."
#endif
        if (a < n) {
            out[i] = (1-ft)*x->signals[a][i] + ft*x->signals[a+1][i];
        } else {
            out[i] = x->signals[a][i];
        }
    }

    return w+4;
}

void nfader_tilde_dsp(t_nfader_tilde *x, t_signal **sp) {
    for (int i = 0; i < x->n+1; ++i) {
        x->signals[i] = sp[i]->s_vec;
    }
    dsp_add(nfader_tilde_perform, 3, x, sp[x->n+1]->s_vec, (t_int)sp[0]->s_n);
}

void nfader_tilde_setup(void) {
    nfader_tilde_class = class_new(gensym("nfader~"),
                             (t_newmethod)nfader_new,
                             (t_method)nfader_free,
                             sizeof(t_nfader_tilde),
                             CLASS_PATCHABLE | CLASS_NOINLET,
                             A_DEFFLOAT,
                             0);
    CLASS_MAINSIGNALIN(nfader_tilde_class, t_nfader_tilde, x_f);
    class_addmethod(nfader_tilde_class,
                    (t_method)nfader_tilde_dsp,
                    gensym("dsp"), A_CANT, 0);

    class_sethelpsymbol(nfader_tilde_class, gensym("nfader~"));
}

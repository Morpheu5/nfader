#include <stdlib.h>
#include <math.h>
#include "m_pd.h"

static t_class *nfader_tilde_class;

typedef struct _nfader_tilde {
    t_object x_obj;
    t_float x_f;
    t_int n;
    t_float t;
    t_inlet *in_t;
    t_inlet **in_vec;
    t_outlet *out;
} t_nfader_tilde;

void *nfader_new(t_floatarg n) {
    t_nfader_tilde *x = (t_nfader_tilde *)pd_new(nfader_tilde_class);

    x->n = (t_int)floor(n < 2 ? 2 : n);
    x->t = 0.0f;
    x->in_t = floatinlet_new(&x->x_obj, &x->t);
    x->in_vec = (t_inlet **)getbytes(n * sizeof(t_inlet *));
    for (int i = 0; i < x->n; ++i) {
        x->in_vec[i] = signalinlet_new(&x->x_obj, 0.0f);
    }
    x->out = outlet_new(&x->x_obj, &s_signal);

    return (void *)x;
}

void nfader_free(t_nfader_tilde *x) {
    for (int i = 0; i < x->n; ++i) {
        inlet_free(x->in_vec[i]);
    }
    freebytes(x->in_vec, x->n * sizeof(t_inlet *));
    inlet_free(x->in_t);
    outlet_free(x->out);
}

t_int *nfader_tilde_perform(t_int *w) {
    t_nfader_tilde *x = (t_nfader_tilde *)w[1];
    t_inlet **in_vec = (t_inlet **)w[2];
    t_sample *out = (t_sample *)w[3];

    post("t = %f\nn = %d\n", x->t, x->n);

    return w+5;
}

void nfader_tilde_dsp(t_nfader_tilde *x, t_signal **sp) {
    dsp_add(nfader_tilde_perform, 4, x, sp, sp[x->n]->s_vec, (t_int)sp[0]->s_n);
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
}

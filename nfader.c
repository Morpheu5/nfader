#include <stdlib.h>
#include <math.h>
#include "m_pd.h"

static t_class *nfader_tilde_class;

typedef struct _nfader_tilde {
    t_object x_obj;
    t_int n;
    t_float t;
    t_atom *in_vec;
    t_outlet *out;
} t_nfader_tilde;

void nfader_setN(t_nfader_tilde *x, t_floatarg n) {
    x->n = n < 2 ? 2 : n;
}

void *nfader_new(t_floatarg n) {
    t_nfader_tilde *x = (t_nfader_tilde *)pd_new(nfader_tilde_class);
    t_atom *vec;

    nfader_setN(x, n);
    x->t = 0.0f;
    vec = x->in_vec = (t_atom *)getbytes(n * sizeof(*x->in_vec));
    for (int i = 0; i < n; ++i) {
        signalinlet_new(&x->x_obj, 0.0f);
    }
    x->out = outlet_new(&x->x_obj, &s_signal);

    return (void *)x;
}

void nfader_free(t_nfader_tilde *x) {
    outlet_free(x->out);
}

void nfader_tilde_setup(void) {
    nfader_tilde_class = class_new(gensym("nfader~"),
                             (t_newmethod)nfader_new,
                             (t_method)nfader_free,
                             sizeof(t_nfader_tilde),
                             CLASS_DEFAULT,
                             A_DEFFLOAT,
                             0);
}

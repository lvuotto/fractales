
#define RANDOM01()  (rand() / (double) RAND_MAX)
#define RANDOM_11() (2.L*rand() / (double) RAND_MAX - 1.L)


enum LLAMA_VARIACION {
  LLAMA_LINEAR,
  LLAMA_SINUSOIDAL,
  LLAMA_SPHERICAL,
  LLAMA_SWIRL,
  LLAMA_HORSESHOE,
  LLAMA_POLAR,
  LLAMA_HANDKERCHIEF,
  LLAMA_HEART,
  LLAMA_DISC,
  LLAMA_SPIRAL,
  LLAMA_HYPERBOLIC,
  LLAMA_DIAMOND,
  LLAMA_EX,
  LLAMA_JULIA,
  LLAMA_BENT,
  
  /* sizeof */
  LLAMA_TOTAL_VARIACIONES
};
typedef enum LLAMA_VARIACION LLAMA_VARIACION_T;



struct __punto {
  double x;
  double y;
};

typedef struct __punto punto;

struct __transformacion_afin {
  double              a;
  double              b;
  double              c;
  double              d;
  double              e;
  double              f;
};
typedef struct __transformacion_afin trans_afin;

struct __funcion {
  double              a;
  double              b;
  double              c;
  double              d;
  double              e;
  double              f;
  
  LLAMA_VARIACION_T   t;
  double              w;
  trans_afin          P;
};
typedef struct __funcion funcion;

typedef void (*f_ptr)(punto *, funcion *);


void ta_id (trans_afin *p) {
  p->a = 1;
  p->b = 0;
  p->c = 0;
  p->d = 0;
  p->e = 1;
  p->f = 0;
}

void ta_id_afin (trans_afin *p) {
  p->a = 1;
  p->b = 0;
  p->d = 0;
  p->e = 1;
}

void ta_og_rot (trans_afin *p) {
  double x;
  
  x = sqrt(p->a*p->a + p->d*p->d);
  p->a /= x;
  p->d /= x;
  p->b = -p->d;
  p->e = p->a;
  p->c = 0;
  p->f = 0;
}

void ta_og_sim (trans_afin *p) {
  double x;
  
  x = sqrt(p->a*p->a + p->d*p->d);
  p->a /= x;
  p->d /= x;
  p->b = p->d;
  p->e = -p->a;
  p->c = 0;
  p->f = 0;
}

void ta_og_rot_afin (trans_afin *p) {
  double x;
  
  x = sqrt(p->a*p->a + p->d*p->d);
  p->a /= x;
  p->d /= x;
  p->b = -p->d;
  p->e = p->a;
}

void ta_og_sim_afin (trans_afin *p) {
  double x;
  
  x = sqrt(p->a*p->a + p->d*p->d);
  p->a /= x;
  p->d /= x;
  p->b = p->d;
  p->e = -p->a;
  p->c = 0;
  p->f = 0;
}

void ta_tl (trans_afin *p) {
  p->c = 0;
  p->f = 0;
}


/*

void llama_ (punto *p, funcion *v) {
  double x, y;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
}

*/


void llama_linear (punto *p, funcion *v) {
  double x, y;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  
  p->x = x;
  p->y = y;
}

void llama_sinusoidal (punto *p, funcion *v) {
  double x, y;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  
  p->x = sin(x);
  p->y = sin(y);
}

void llama_spherical (punto *p, funcion *v) {
  double x, y, r2;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r2 = x*x + y*y;
  
  p->x = x/r2;
  p->y = y/r2;
}

void llama_swirl (punto *p, funcion *v) {
  double x, y, r2;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r2 = x*x + y*y;
  
  p->x = x*sin(r2) - y*cos(r2);
  p->y = x*cos(r2) + y*sin(r2);
}

void llama_horseshoe (punto *p, funcion *v) {
  double x, y, r;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  
  p->x = (x*x - y*y)/r;
  p->y = 2.0*x*y/r;
}

void llama_polar (punto *p, funcion *v) {
  double x, y, r, tita;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  
  p->x = tita/M_PI;
  p->y = r - 1.0;
}

void llama_handkerchief (punto *p, funcion *v) {
  double x, y, r, tita;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  
  p->x = r*sin(tita + r);
  p->y = r*cos(tita - r);
}

void llama_heart (punto *p, funcion *v) {
  double x, y, r, tita;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  
  p->x = r*sin(tita*r);
  p->y = -r*cos(tita*r);
}

void llama_disc (punto *p, funcion *v) {
  double x, y, r, tita;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  
  p->x = tita*sin(M_PI*r)/r;
  p->y = tita*cos(M_PI*r)/r;
}

void llama_spiral (punto *p, funcion *v) {
  double x, y, r, tita;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  
  p->x = (cos(tita) + sin(r))/r;
  p->y = (sin(tita) - cos(r))/r;
}

void llama_hyperbolic (punto *p, funcion *v) {
  double x, y, r, tita;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  
  p->x = sin(tita)/r;
  p->y = r*cos(tita);
}

void llama_diamond (punto *p, funcion *v) {
  double x, y, r, tita;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  
  p->x = sin(tita)*cos(r);
  p->y = cos(tita)*sin(r);
}

void llama_ex (punto *p, funcion *v) {
  double x, y, r, tita, p0, p1;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r = sqrt(x*x + y*y);
  tita = atan(x/y);
  p0 = sin(tita + r);
  p1 = cos(tita - r);
  
  p->x = r*(pow(p0, 3) + pow(p1, 3));
  p->y = r*(pow(p0, 3) - pow(p1, 3));
}

void llama_julia (punto *p, funcion *v) {
  double x, y, r_2, tita, omega;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  r_2 = sqrt(sqrt(x*x + y*y));
  tita = atan(x/y);
  omega = RANDOM01() > 0.5 ? 0 : M_PI;
  
  p->x = r_2*cos(0.5*tita + omega);
  p->y = r_2*sin(0.5*tita + omega);
}

void llama_bent (punto *p, funcion *v) {
  double x, y;
  
  x = v->a*p->x + v->b*p->y + v->c;
  y = v->d*p->x + v->e*p->y + v->f;
  
  p->x = x >= 0 ? x : 2.0*x;
  p->y = y >= 0 ? y : 0.5*y;
}



static f_ptr variaciones[LLAMA_TOTAL_VARIACIONES + 1] = {
  llama_linear,
  llama_sinusoidal,
  llama_spherical,
  llama_swirl,
  llama_horseshoe,
  llama_polar,
  llama_handkerchief,
  llama_heart,
  llama_disc,
  llama_spiral,
  llama_hyperbolic,
  llama_diamond,
  llama_ex,
  llama_julia,
  llama_bent,
  
  NULL
};

void aplicar (funcion *f, punto *p) {
  double x, y;
  
  variaciones[f->t](p, f);
  x = p->x*f->w;
  y = p->y*f->w;
  
  p->x = f->P.a*x + f->P.b*y + f->P.c;
  p->y = f->P.d*x + f->P.e*y + f->P.f;
}


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <lv_pnm.h>
#include <paleta.h>
#include <variaciones.h>


#ifndef SEED
# define SEED time(NULL)
  /* 1389058682 */
  /* 1389062786 */
  /* 1389131238 */
  /* 1389131702 */
  /* 1389132335 */
#endif

#ifndef GAMMA
# define GAMMA 2.2
#endif

#ifndef POINTS
# define POINTS 1e7
#endif

#ifndef FUNCS
# define FUNCS 13
#endif

#ifndef T_PALETA
# define T_PALETA FUNCS
#endif


#define RANDOM01()  (rand() / (double) RAND_MAX)
#define RANDOM_11() (2.L*rand() / (double) RAND_MAX - 1.L)


void muestra_paleta (unsigned int seed);
void llama (unsigned int ancho, unsigned int alto, unsigned int seed);


int main (int argc, char **argv) {
  
  int w, h;
  unsigned int seed;
  
  seed = SEED;
  
  if (argc < 3) {
    w = 640;
    h = 512;
  } else {
    w = atoi(argv[1]);
    h = atoi(argv[2]);
  }
  
  muestra_paleta(seed);
  llama(w, h, seed);
  
  return EXIT_SUCCESS;
  
}


void muestra_paleta (unsigned int seed) {
  
  pnm_context_t *ctx;
  pnm_pixel_t *paleta;
  unsigned int i, j, f;
  char c[160];
  
  f = 100;
  while (f*FUNCS > 1000) {
    f--;
  }
  
  ctx = pnm_create(PNM_BIN_RGB, f*FUNCS, 100, 255);
  
  srand(seed);
  paleta = crear_paleta(rand() % 256,
                        rand() % 256,
                        rand() % 256,
                        256*RANDOM01(),
                        256*RANDOM01(),
                        RANDOM_11(),
                        RANDOM_11(),
                        RANDOM_11(),
                        RANDOM_11(),
                        T_PALETA);
  
  for (j = 0; j < 100; j++) {
    for (i = 0; i < f*FUNCS; i++) {
      pnm_pixel_set(ctx, i, j, paleta[i/f]);
    }
  }
  
  sprintf(c, "SEED = %d", (int) seed);
  pnm_add_comment(ctx, c);
  pnm_write_image(ctx, "muestra_paleta.pnm");
  
  pnm_destroy(ctx);
  destruir_paleta(paleta);
  
}



void llama (unsigned int ancho, unsigned int alto, unsigned int seed) {
  
  pnm_context_t *pnm;
  pnm_pixel_t bg_color, pixel, *paleta;
  unsigned int i, t, *hit_ctr, hit_max, x, y;
  int px, py, m;
  double tx, ty, cenX, cenY, zoom, ws, s, sum, d, a, gamma_c, r, g, b;
  punto p;
  funcion F[FUNCS];
  trans_afin final;
  char comentarios[160];
  
  pnm = pnm_create(PNM_BIN_RGB, ancho, alto, PNM_MAXVAL_MAX_VALUE);
  hit_ctr = (unsigned int *) calloc(ancho * alto, sizeof(unsigned int));
  
  
  bg_color.r = 0x00;
  bg_color.g = 0x00;
  bg_color.b = 0x00;
  pnm_set_color(pnm, bg_color);
  
  cenX = 0.0;
  cenY = 0.0;
  zoom = 1.0;
  
  ws = 2.0/zoom;
  s = ws/(double)ancho;
  
  srand(seed);
  paleta = crear_paleta(rand() % 256,
                        rand() % 256,
                        rand() % 256,
                        256*RANDOM01(),
                        256*RANDOM01(),
                        RANDOM_11(),
                        RANDOM_11(),
                        RANDOM_11(),
                        RANDOM_11(),
                        FUNCS);
  
  sum = 0;
  m = 1;
  for (t = 0; t < FUNCS; t++) {
    F[t].a = ws*RANDOM_11();
    F[t].b = ws*RANDOM_11();
    F[t].c = ws*RANDOM_11();
    F[t].d = ws*RANDOM_11();
    F[t].e = ws*RANDOM_11();
    F[t].f = ws*RANDOM_11();
    
    F[t].t = rand() % LLAMA_TOTAL_VARIACIONES;
    
    F[t].w = m*RANDOM01();
    if (t == FUNCS - 1)
      F[t].w = m > 0 ? 1.0 - sum : sum - 1.0;
    sum = sum + F[t].w;
    m = sum > 1.0 ? -1 : 1;
    
    F[t].P.a = ws*RANDOM_11();
    F[t].P.b = ws*RANDOM_11();
    F[t].P.c = ws*RANDOM_11();
    F[t].P.d = ws*RANDOM_11();
    F[t].P.e = ws*RANDOM_11();
    F[t].P.f = ws*RANDOM_11();
    /*ta_og_sim(&F[t].P);*/
    /*ta_id(&F[t].P);*/
  }
  
  final.a = ws*RANDOM_11();
  final.b = ws*RANDOM_11();
  final.c = ws*RANDOM_11();
  final.d = ws*RANDOM_11();
  final.e = ws*RANDOM_11();
  final.f = ws*RANDOM_11();
  
  ta_id(&final);
  
  p.x = ws*RANDOM_11();
  p.y = (s*alto)*RANDOM_11();
  
  hit_max = 0;
  for (t = 0; t < POINTS; t++) {
    i = rand() % FUNCS;
    
    aplicar(&F[i], &p);
    tx = p.x;
    ty = p.y;
    p.x = final.a*tx + final.b*ty + final.c;
    p.y = final.d*tx + final.e*ty + final.f;
    
    px = (0.5*(p.x + ws - cenX)/ws) * ancho;
    py = (0.5*(p.y + s*alto - cenY)/(s*alto)) * alto;
    
    if (0 <= px && px < ancho && 0 <= py && py < alto && t > 20) {
      pixel = pnm_pixel_get(pnm, px, py);
      pixel.r = (pixel.r + paleta[i].r) / 2;
      pixel.g = (pixel.g + paleta[i].g) / 2;
      pixel.b = (pixel.b + paleta[i].b) / 2;
      pnm_pixel_set(pnm, px, py, pixel);
      
      hit_ctr[py*ancho + px]++;
      hit_max = hit_max < hit_ctr[py*ancho + px] ?
                hit_ctr[py*ancho + px] :
                hit_max;
    }
  }
  
  if (hit_max > 1) {
    d = 1.0 / log(hit_max);
    gamma_c = 1.0 / GAMMA;
    /*pnm_set_color(pnm, bg_color);*/
    for (y = 0; y < alto; y++) {
      for (x = 0; x < ancho; x++) {
        pixel = pnm_pixel_get(pnm, x, y);
        
        if (hit_ctr[ancho*y + x] > 1) {
          a = fabs(log(hit_ctr[ancho*y + x]) * d);
          /*pixel = paleta[(int)(a*T_PALETA) % T_PALETA];*/
          r = pixel.r / 255.0;
          g = pixel.g / 255.0;
          b = pixel.b / 255.0;
          r = pow(r*a, gamma_c);
          g = pow(g*a, gamma_c);
          b = pow(b*a, gamma_c);
          
          pixel.r = (int)(r * 255) % 256;
          pixel.g = (int)(g * 255) % 256;
          pixel.b = (int)(b * 255) % 256;
        }
        
        pnm_pixel_set(pnm, x, y, pixel);
      }
    }
  }
  
  sprintf(comentarios,
          "SEED = %u\nFUNCS = %u\n",
          (unsigned int) seed,
          (unsigned int) FUNCS);
  pnm_add_comment(pnm, comentarios);
  pnm_write_image(pnm, "llama.pnm");
  
  destruir_paleta(paleta);
  free(hit_ctr);
  pnm_destroy(pnm);
  
}

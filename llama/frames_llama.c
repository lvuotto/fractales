
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
# define FUNCS 7
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
  while (f * FUNCS > 1000) {
    f--;
  }
  
  ctx = pnm_create(PNM_BIN_RGB, f * FUNCS, 100, 255);
  
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
  
  for (j = 0; j < 100; j++) {
    for (i = 0; i < f * FUNCS; i++) {
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
  
  pnm_context_t *pnm, *histograma;
  pnm_pixel_t bg_color, pixel, *paleta;
  unsigned int i, t, *freq_ctr, freq_max, x, y;
  int px, py, m, cant;
  double /*x, y,*/ cenX, cenY, zoom, ws, s, sum, d, a, gamma_c, r, g, b;
  punto p;
  funcion F[FUNCS];
  char comentarios[160], nombre[160];
  
  pnm = pnm_create(PNM_BIN_RGB, ancho, alto, PNM_MAXVAL_MAX_VALUE);
  histograma = pnm_create(PNM_BIN_RGB, ancho, alto, PNM_MAXVAL_MAX_VALUE);
  freq_ctr = (unsigned int *) calloc(ancho * alto, sizeof(unsigned int));
  
  
  bg_color.r = 0x00;
  bg_color.g = 0x00;
  bg_color.b = 0x00;
  pnm_set_color(histograma, bg_color);
  
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
    
    /*if (t % 2) {
      F[t].a =  F[t].a;
      F[t].b =  F[t].d;
      F[t].d =  F[t].d;
      F[t].e = -F[t].a;
    }*/
    
    F[t].t = rand() % LLAMA_TOTAL_VARIACIONES;
    
    F[t].w = m*RANDOM01();
    if (t == FUNCS - 1)
      F[t].w = m > 0 ? 1.0 - sum : sum - 1.0;
    sum = sum + F[t].w;
    m = sum > 1.0 ? -1 : 1;
    /*F[t].w = 1.;*/
  }
  
  p.x = ws*RANDOM_11();
  p.y = (s*alto)*RANDOM_11();
  
  freq_max = 0;
  cant = 0;
  for (t = 0; t < POINTS; t++) {
    i = rand() % FUNCS;
    
    aplicar(&F[i], &p);
    
    px = (0.5*(p.x + ws - cenX)/ws) * ancho;
    py = (0.5*(p.y + s*alto - cenY)/(s*alto)) * alto;
    /*if (t % 10000 == 0) printf("(%f, %f)\n", p.x + .5*ws - cenX, p.y + .5*ws - cenY);*/
    /*if (t % 10000 == 0) printf("(%d, %d)\n", px, py);*/
    /*px *= ancho;
    py *= alto;*/
    
    if (t % 100000 == 0) {
      sprintf(nombre, "frames/llama.%d.pnm", cant++);
      pnm_write_image(histograma, nombre);
    }
    
    
    if (0 <= px && px < ancho && 0 <= py && py < alto && t > 20) {
      pixel = pnm_pixel_get(histograma, px, py);
      pixel.r = (pixel.r + paleta[i].r) / 2;
      pixel.g = (pixel.g + paleta[i].g) / 2;
      pixel.b = (pixel.b + paleta[i].b) / 2;
      pnm_pixel_set(histograma, px, py, pixel);
      freq_ctr[py*ancho + px]++;
      freq_max = freq_max < freq_ctr[py*ancho + px] ?
                 freq_ctr[py*ancho + px] :
                 freq_max;
    }
  }
  
  if (freq_max > 1) {
    d = 1.0 / log(freq_max);
    gamma_c = 1.0 / GAMMA;
    for (y = 0; y < alto; y++) {
      for (x = 0; x < ancho; x++) {
        pixel = pnm_pixel_get(histograma, x, y);
        
        if (freq_ctr[ancho*y + x] > 0) {
          a = fabs(log(freq_ctr[ancho*y + x]) * d);
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
  sprintf(nombre, "frames/llama.%d.pnm", cant++);
  pnm_write_image(pnm, nombre);
  
  destruir_paleta(paleta);
  free(freq_ctr);
  pnm_destroy(pnm);
  pnm_destroy(histograma);
  
}

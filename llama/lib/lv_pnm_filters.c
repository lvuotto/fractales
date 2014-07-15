
#include "lv_pnm_types.h"
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
# define M_PI      3.14159265359
# define M_PI_2    1.57079632679
# define M_PI_4    0.78539816339
#endif
#define SQRT_2PI   2.50662827463

/**
 * Data for kernels from:
 *  - http://en.wikipedia.org/wiki/Kernel_%28statistics%29#Kernel_functions_in_common_use
 * 
 * Influenced by:
 *  - http://code.google.com/p/cairocks/source/browse/trunk/src/gaussian-blur.c
 * 
 **/

#define set_in_range(X, INF, SUP) ((X) < (INF) ? (INF) : (X) > (SUP) ? (SUP) : (X))


double * create_kernel (pnm_blur_kernel_t kernel, double r) {
  int kernel_width, i;
  double v, u, s, j, *K;
  
  kernel_width = 2 * (int) r + 1;
  K = (double *) malloc(sizeof(double) * (kernel_width + 1));
  
  if (K == NULL) return 0;
  
  K[0] = (double) kernel_width;
  s = 0;
  j = 2.*fabs(r)/K[0];
  v = j/2. - fabs(r);
  
  if (kernel_width == 1) {
    K[1] = 1;
  } else {
    switch (kernel) {
      case PNM_BLUR_KERNEL_UNIFORM:
        for (i = 1; i <= kernel_width; i++) {
          K[i] = .5;
          s += K[i];
        }
        break;
      case PNM_BLUR_KERNEL_TRIANGULAR:
        for (i = 1; i <= kernel_width; i++) {
          u = fabs(v/r);
          K[i] = 1 - u;
          s += K[i];
          v += j;
        }
        break;
      case PNM_BLUR_KERNEL_EPANECHNIKOV:
        for (i = 1; i <= kernel_width; i++) {
          u = v/r;
          K[i] = .75 * (1 - u*u);
          s += K[i];
          v += j;
        }
        break;
      case PNM_BLUR_KERNEL_BIWEIGHT:
        for (i = 1; i <= kernel_width; i++) {
          u = v/r;
          u = 1 - u*u;
          K[i] = .9375 * u*u;
          s += K[i];
          v += j;
        }
        break;
      case PNM_BLUR_KERNEL_TRIWEIGHT:
        for (i = 1; i <= kernel_width; i++) {
          u = v/r;
          u = 1 - u*u;
          K[i] = 1.09375 * u*u*u;
          s += K[i];
          v += j;
        }
        break;
      case PNM_BLUR_KERNEL_TRICUBE:
        for (i = 1; i <= kernel_width; i++) {
          u = fabs(v/r);
          u = 1 - u*u*u;
          K[i] = 70. * u*u*u / 81.;
          s += K[i];
          v += j;
        }
        break;
      case PNM_BLUR_KERNEL_GAUSSIAN:
        for (i = 1; i <= kernel_width; i++) {
          u = v/r;
          K[i] = exp(-.5*u*u) / SQRT_2PI;
          s += K[i];
          v += j;
        }
        break;
      case PNM_BLUR_KERNEL_COSINE:
        for (i = 1; i <= kernel_width; i++) {
          u = v/r;
          K[i] = M_PI_4 * cos(M_PI_2 * u);
          s += K[i];
          v += j;
        }
        break;
      default:
        return NULL;
    }
    
    for (i = 1; i <= kernel_width; i++) {
      K[i] /= s;
    }
  }
  
  return K;
  
}

pnm_bool_t pnm_apply_blur_filter (pnm_context_t *ctx,
                                  pnm_blur_kernel_t kernel,
                                  double kernel_width,
                                  double kernel_height)
{
  
  int i, j, offset, f, x, y;
  double r, g, b, *kernel_m;
  pnm_pixel_t *hor_blur;
  
  /* Horizontal */
  kernel_m = create_kernel(kernel, kernel_width);
  hor_blur = (pnm_pixel_t *) malloc(sizeof(pnm_pixel_t) * ctx->width * ctx->height);
  
  if (kernel_m == NULL || hor_blur == NULL)
    return PNM_FALSE;
  
  offset = -kernel_m[0] / 2;
  for (i = 0; i < ctx->height; i++) {
    for (j = 0; j < ctx->width; j++) {
      r = 0;
      g = 0;
      b = 0;
      y = i * ctx->width;
      
      for (f = 1; f <= (int) kernel_m[0]; f++) {
        x = set_in_range(j + offset + f - 1, 0, ctx->width - 1);
        
        r += ctx->canvas[y + x].r * kernel_m[f];
        g += ctx->canvas[y + x].g * kernel_m[f];
        b += ctx->canvas[y + x].b * kernel_m[f];
      }
      
      hor_blur[y + j].r = (unsigned char) r;
      hor_blur[y + j].g = (unsigned char) g;
      hor_blur[y + j].b = (unsigned char) b;
    }
  }
  free(kernel_m);
  kernel_m = NULL;
  
  /* Vertical */
  kernel_m = create_kernel(kernel, kernel_height);
  
  if (kernel_m == NULL)
    return PNM_FALSE;
  
  offset = -kernel_m[0] / 2;
  for (i = 0; i < ctx->height; i++) {
    for (j = 0; j < ctx->width; j++) {
      r = 0;
      g = 0;
      b = 0;
      
      for (f = 1; f <= (int) kernel_m[0]; f++) {
        y = ctx->width * set_in_range(i + offset + f - 1, 0, ctx->height - 1);
        
        r += hor_blur[y + j].r * kernel_m[f];
        g += hor_blur[y + j].g * kernel_m[f];
        b += hor_blur[y + j].b * kernel_m[f];
      }
      
      y = i*ctx->width;
      ctx->canvas[y + j].r = (unsigned char) r;
      ctx->canvas[y + j].g = (unsigned char) g;
      ctx->canvas[y + j].b = (unsigned char) b;
    }
  }
  free(hor_blur);
  free(kernel_m);
  hor_blur = NULL;
  kernel_m = NULL;
    
  return PNM_TRUE;
  
}

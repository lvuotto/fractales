
/* pnm_filters.h - Compile with `gcc -lm' */


#ifndef __LV_PNM_FILTERS_H__

#define __LV_PNM_FILTERS_H__


#include "lv_pnm.h"

double * create_kernel           (pnm_blur_kernel_t kernel, double r);
pnm_bool_t pnm_apply_blur_filter (pnm_context_t *ctx,
                                  pnm_blur_kernel_t kernel,
                                  double kernel_width,
                                  double kernel_height);

#endif      /* __LV_PNM_FILTERS_H__ */

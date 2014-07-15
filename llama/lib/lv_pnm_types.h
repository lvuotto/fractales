
/* pnm_types.h */


#ifndef __LV_PNM_TYPES_H__

#define __PNM_TYPES_H__
#define PNM_MAXVAL_MAX_VALUE 255 /* 65535 */
#define PNM_WIDTH_MAX_VALUE 65535
#define PNM_HEIGHT_MAX_VALUE 65535


enum __PNM_BOOL_T {
  PNM_UNDEFINED           = -1,
  PNM_FALSE               =  0,
  PNM_TRUE                =  1
};

typedef enum __PNM_BOOL_T pnm_bool_t;

enum __PNM_IMAGE_TYPE {
  PNM_ASCII_BIT           = 1,
  PNM_ASCII_GREY          = 2,
  PNM_ASCII_RGB           = 3,
  PNM_BIN_BIT             = 4,
  PNM_BIN_GREY            = 5,
  PNM_BIN_RGB             = 6
};

typedef enum __PNM_IMAGE_TYPE pnm_image_t;

enum __PNM_BLUR_KERNEL {
  PNM_BLUR_KERNEL_UNIFORM,
  PNM_BLUR_KERNEL_TRIANGULAR,
  PNM_BLUR_KERNEL_EPANECHNIKOV,
  PNM_BLUR_KERNEL_BIWEIGHT,
  PNM_BLUR_KERNEL_TRIWEIGHT,
  PNM_BLUR_KERNEL_TRICUBE,
  PNM_BLUR_KERNEL_GAUSSIAN,
  PNM_BLUR_KERNEL_COSINE,

  /* sizeof :D */
  PNM_BLUR_AVAILABLE_KERNELS
};

typedef enum __PNM_BLUR_KERNEL pnm_blur_kernel_t;

enum __PNM_ERROR {
  PNMERR_LIMIT_OVERFLOW,
  PNMERR_VALUE_OUT_OF_RANGE,
  PNMERR_MEMORY_FAILURE,
  PNMERR_OUTPUT_FILE_FAILURE,
  PNMERR_WRONG_HEADER,
  PNMERR_READ_FAILURE
};

struct __PNM_COLOR {
  unsigned char             r;
  unsigned char             g;
  unsigned char             b;
};

struct __PNM_HSL_PIXEL {
  double                    h;
  double                    s;
  double                    l;
};

typedef struct __PNM_COLOR pnm_pixel_t;
typedef struct __PNM_HSL_PIXEL pnm_hsl_pixel_t;

struct __PNM_CONTEXT {
  pnm_image_t               type;

  unsigned int              width;
  unsigned int              height;
  unsigned int              maxval;

  pnm_pixel_t *             canvas;

  char **                   comments;
  int                       __comm_q; /* "private" */
};

typedef struct __PNM_CONTEXT pnm_context_t;

#endif  /*  __LV_PNM_TYPES_H__  */

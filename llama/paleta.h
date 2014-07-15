
#include <lv_pnm.h>

#define ACOTAR(X, I, S) ((X) > (S) ? (S) : (X) < (I) ? (I) : (X))

pnm_pixel_t * crear_paleta    (int cx,
                               int cy,
                               int cz,
                               double Rx,
                               double Ry,
                               double r,
                               double nx,
                               double ny,
                               double nz,
                               unsigned int t);
void          destruir_paleta (pnm_pixel_t *p);



pnm_pixel_t * crear_paleta (int cx,
                            int cy,
                            int cz,
                            double Rx,
                            double Ry,
                            double r,
                            double nx,
                            double ny,
                            double nz,
                            unsigned int t)
{
  
  unsigned int i;
  pnm_pixel_t *colores;
  double n, m, cr, cg, cb, tr, tg, ax, ay, az, M[6];
  
  colores = (pnm_pixel_t *) malloc(sizeof(pnm_pixel_t) * t);
  
  n = sqrt(nx*nx + ny*ny + nz*nz);
  ax = acos(nx/n);
  ay = acos(ny/n);
  az = acos(nz/n);
  
  /**
   * [ 1   0    0  ] [ cay 0 -say ] [ caz -saz 0 ]
   * [ 0  cax -sax ] [  0  1   0  ] [ saz  caz 0 ]
   * [ 0  sax  cax ] [ say 0  cay ] [  0    0  1 ]
   * 
   * [   cay     0    -say   ] [ caz -saz 0 ]
   * [ -sax*say cax -sax*cay ] [ saz  caz 0 ]
   * [  cax*say sax  cax*cay ] [  0    0  1 ]
   * 
   * [        cay*caz         |        -cay*saz        |   -say   ]
   * [ -sax*say*caz + cax*saz |  sax*say*saz + cax*caz | -sax*cay ]
   * [  cax*say*caz + sax*saz | -cax*say*saz + sax*caz |  cax*cay ]
   **/
  
  M[0] =  cos(ay)*cos(az);
  M[1] = -cos(ay)*sin(az);
  M[2] = -sin(ax)*sin(ay)*cos(az) + cos(ax)*sin(az);
  M[3] =  sin(ax)*sin(ay)*sin(az) + cos(ax)*sin(az);
  M[4] =  cos(ax)*sin(ay)*cos(az) + sin(ax)*sin(az);
  M[5] = -cos(ax)*sin(ay)*sin(az) + sin(ax)*cos(az);
  
  /*printf("cx = %d\ncy = %d\ncz = %d\nR = %.16f\n"
         "r = %.16f\nnx = %.16f\nny = %.16f\nnz = %.16f\n",
         cx, cy, cz, R, r, nx, ny, nz);*/
  
  i = 0;
  while (i < t/2) {
    m = 2.0*M_PI * i / (double) t;
    
    tr = Rx*cos(m);
    tg = Ry*sin(m);
    /* tb = 0; */
    
    cr = M[0]*tr + M[1]*tg;
    cg = M[2]*tr + M[3]*tg;
    cb = M[4]*tr + M[5]*tg;
    
    colores[i].r = ACOTAR(cx + cr, 0, 255);
    colores[i].g = ACOTAR(cy + cg, 0, 255);
    colores[i].b = ACOTAR(cz + cb, 0, 255);
    
    i++;
  }
  while (i < t) {
    m = 2.L * M_PI * i / (double) t;
    
    tr =    Rx*cos(m);
    tg = -r*Ry*sin(m);
    /* tb = 0; */
    
    cr = M[0]*tr + M[1]*tg;
    cg = M[2]*tr + M[3]*tg;
    cb = M[4]*tr + M[5]*tg;
    
    colores[i].r = ACOTAR(cx + cr, 0, 255);
    colores[i].g = ACOTAR(cy + cg, 0, 255);
    colores[i].b = ACOTAR(cz + cb, 0, 255);
    
    i++;
  }
  
  return colores;
  
}

void destruir_paleta (pnm_pixel_t *p) {
  free(p);
  p = NULL;
}

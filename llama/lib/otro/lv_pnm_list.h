
/* pnm_list.h */

#ifndef __PNM_LIST_H__

#define __PNM_LIST_H__

/* ==========================================================================
   STRUCTS
   ========================================================================== */

struct __PNM_COMMENT_NODE {
    char                      *comment;
    struct __PNM_COMMENT_NODE *next;
};

typedef struct __PNM_COMMENT_NODE pnm_comment_t;

struct __PNM_COMMENT_LIST {
    pnm_comment_t             *first;
    pnm_comment_t             *last;
    int                        size;
};

typedef struct __PNM_COMMENT_LIST pnm_list_t;


/* ==========================================================================
   PROTOTYPES
   ========================================================================== */

pnm_list_t * create_list ();
void append_node         (pnm_list_t *list, const char *s);
void free_list           (pnm_list_t *list);

#endif      /* __PNM_LIST_H__ */

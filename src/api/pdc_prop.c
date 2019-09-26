/*
 * Copyright Notice for 
 * Proactive Data Containers (PDC) Software Library and Utilities
 * -----------------------------------------------------------------------------

 *** Copyright Notice ***
 
 * Proactive Data Containers (PDC) Copyright (c) 2017, The Regents of the
 * University of California, through Lawrence Berkeley National Laboratory,
 * UChicago Argonne, LLC, operator of Argonne National Laboratory, and The HDF
 * Group (subject to receipt of any required approvals from the U.S. Dept. of
 * Energy).  All rights reserved.
 
 * If you have questions about your rights to use or distribute this software,
 * please contact Berkeley Lab's Innovation & Partnerships Office at  IPO@lbl.gov.
 
 * NOTICE.  This Software was developed under funding from the U.S. Department of
 * Energy and the U.S. Government consequently retains certain rights. As such, the
 * U.S. Government has been granted for itself and others acting on its behalf a
 * paid-up, nonexclusive, irrevocable, worldwide license in the Software to
 * reproduce, distribute copies to the public, prepare derivative works, and
 * perform publicly and display publicly, and to permit other to do so.
 */

#include <string.h>
#include "pdc_pkg.h"
#include "pdc_prop.h"
#include "pdc_prop_private.h"
#include "pdc_interface.h"
#include "pdc_malloc.h"
#include "pdc_prop_pkg.h"
#include "pdc_atomic.h"

static perr_t pdc_prop_cont_close(struct PDC_cont_prop *cp);

static perr_t pdc_prop_obj_close(struct PDC_obj_prop *cp);

perr_t PDC_prop_init()
{
    perr_t ret_value = SUCCEED;

    FUNC_ENTER(NULL);
    /* Initialize the atom group for the container property IDs */
    if(PDC_register_type(PDC_CONT_PROP, (PDC_free_t)pdc_prop_cont_close) < 0)
        PGOTO_ERROR(FAIL, "unable to initialize container property interface");

    /* Initialize the atom group for the object property IDs */
    if(PDC_register_type(PDC_OBJ_PROP, (PDC_free_t)pdc_prop_obj_close) < 0)
        PGOTO_ERROR(FAIL, "unable to initialize object property interface");

done:
    FUNC_LEAVE(ret_value);
} 

pdcid_t PDCprop_create(PDC_prop_type type, pdcid_t pdcid)
{
    pdcid_t ret_value = 0;
    struct PDC_cont_prop *p = NULL;
    struct PDC_obj_prop *q = NULL;
    struct PDC_id_info *id_info = NULL;
    struct PDC_class *pdc_class;
    pdcid_t new_id_c;
    pdcid_t new_id_o;
 
    FUNC_ENTER(NULL);

    if (type == PDC_CONT_CREATE) {
        p = PDC_MALLOC(struct PDC_cont_prop);
        if(!p)
            PGOTO_ERROR(0, "PDC container property memory allocation failed\n");
        p->cont_life = PDC_PERSIST;
        new_id_c = PDC_id_register(PDC_CONT_PROP, p);
        p->cont_prop_id = new_id_c;
        id_info = PDC_find_id(pdcid);
        pdc_class = (struct PDC_class *)(id_info->obj_ptr);
        p->pdc = PDC_CALLOC(struct PDC_class);
        if(p->pdc == NULL)
            PGOTO_ERROR(0, "PDC class allocation failed");
        if(pdc_class->name)
            p->pdc->name = strdup(pdc_class->name);
        p->pdc->local_id = pdc_class->local_id;
        
        ret_value = new_id_c;
    }
    if(type == PDC_OBJ_CREATE) {
        q = PDC_MALLOC(struct PDC_obj_prop);
        if(!q)
            PGOTO_ERROR(ret_value, "PDC object property memory allocation failed\n");
        q->obj_life = PDC_TRANSIENT;
        q->ndim = 0;
        q->dims = NULL;
        q->data_loc = NULL;
        q->type = PDC_UNKNOWN; 
	    q->app_name = NULL;
        q->time_step = 0;
	    q->tags = NULL;
        q->buf = NULL;
        new_id_o = PDC_id_register(PDC_OBJ_PROP, q);
        q->obj_prop_id = new_id_o;
        id_info = PDC_find_id(pdcid);
        pdc_class = (struct PDC_class *)(id_info->obj_ptr);
        q->pdc = PDC_CALLOC(struct PDC_class);
        if(q->pdc == NULL)
            PGOTO_ERROR(0, "PDC class allocation failed");
        if(pdc_class->name)
            q->pdc->name = strdup(pdc_class->name);
        q->pdc->local_id = pdc_class->local_id;
        q->type_extent = 0;
        q->data_state = 0;
        q->locus = CLIENT_MEMORY;
        memset(&q->transform_prop, 0, sizeof(PDC_transform_state_t));
        
        ret_value = new_id_o;
    }
    
done:
    FUNC_LEAVE(ret_value);
} 

pdcid_t PDCprop_obj_dup(pdcid_t prop_id)
{
    pdcid_t ret_value = 0;
    struct  PDC_obj_prop *q = NULL;
    struct  PDC_obj_prop *info = NULL;
    struct  PDC_id_info *prop = NULL;
    pdcid_t new_id;
    size_t  i;

    FUNC_ENTER(NULL);

    prop = PDC_find_id(prop_id);
    if(prop == NULL)
        PGOTO_ERROR(0, "cannot locate object property");
    info = (struct PDC_obj_prop *)(prop->obj_ptr);

    q = PDC_CALLOC(struct PDC_obj_prop);
    if(!q)
        PGOTO_ERROR(0, "PDC object property memory allocation failed\n");
    q->obj_life = info->obj_life;
    q->ndim = info->ndim;
    q->dims = (uint64_t *)malloc(info->ndim * sizeof(uint64_t));
    for(i=0; i<info->ndim; i++)
        (q->dims)[i] = (info->dims)[i];
    if(info->app_name)
        q->app_name = strdup(info->app_name);
    q->time_step = info->time_step;
    if(info->tags)
        q->tags = strdup(info->tags);
    new_id = PDC_id_register(PDC_OBJ_PROP, q);
    q->obj_prop_id = new_id;
    q->pdc = PDC_CALLOC(struct PDC_class);
    if(!q->pdc)
        PGOTO_ERROR(0, "PDC class memory allocation failed\n");
    if(info->pdc->name)
        q->pdc->name = strdup(info->pdc->name);
    q->pdc->local_id = info->pdc->local_id;
    q->data_loc = NULL;
    q->type = PDC_UNKNOWN;
    q->buf = NULL;
    ret_value = new_id;

done:
    FUNC_LEAVE(ret_value);
} 

perr_t PDC_prop_cont_list_null()
{
    perr_t ret_value = SUCCEED;
    int nelemts;

    FUNC_ENTER(NULL);
    
    // list is not empty
    nelemts = PDC_id_list_null(PDC_CONT_PROP);
    if(nelemts > 0) {
        if(PDC_id_list_clear(PDC_CONT_PROP) < 0)
            PGOTO_ERROR(FAIL, "fail to clear container property list");
    }

done:
    FUNC_LEAVE(ret_value);
}

perr_t PDC_prop_obj_list_null()
{
    perr_t ret_value = SUCCEED; 
    int nelemts;

    FUNC_ENTER(NULL);

    // list is not empty
    nelemts = PDC_id_list_null(PDC_OBJ_PROP);
    if(nelemts > 0) {
        if(PDC_id_list_clear(PDC_OBJ_PROP) < 0)
            PGOTO_ERROR(FAIL, "fail to clear obj property list");
    }
    
done:
    FUNC_LEAVE(ret_value);
}

static perr_t pdc_prop_cont_close(struct PDC_cont_prop *cp)
{
    perr_t ret_value = SUCCEED; 

    FUNC_ENTER(NULL);

    free(cp->pdc->name);
    cp->pdc = PDC_FREE(struct PDC_class, cp->pdc);
    cp = PDC_FREE(struct PDC_cont_prop, cp);
    
    FUNC_LEAVE(ret_value);
} 

static perr_t pdc_prop_obj_close(struct PDC_obj_prop *cp)
{
    perr_t ret_value = SUCCEED; 

    FUNC_ENTER(NULL);

    free(cp->pdc->name);
    cp->pdc = PDC_FREE(struct PDC_class, cp->pdc);
    if(cp->dims != NULL) {
        free(cp->dims);
        cp->dims = NULL;
    }
    free(cp->app_name);
    free(cp->tags);
    free(cp->data_loc);
    cp = PDC_FREE(struct PDC_obj_prop, cp);
    
    FUNC_LEAVE(ret_value);
} 

perr_t PDCprop_close(pdcid_t id)
{
    perr_t ret_value = SUCCEED; 

    FUNC_ENTER(NULL);

    /* When the reference count reaches zero the resources are freed */
    if(PDC_dec_ref(id) < 0)
        PGOTO_ERROR(FAIL, "property: problem of freeing id");

done:
    FUNC_LEAVE(ret_value);
} 

perr_t PDC_prop_end()
{
    perr_t ret_value = SUCCEED; 

    FUNC_ENTER(NULL);

    if(PDC_destroy_type(PDC_CONT_PROP) < 0)
        PGOTO_ERROR(FAIL, "unable to destroy container property interface");

    if(PDC_destroy_type(PDC_OBJ_PROP) < 0)
        PGOTO_ERROR(FAIL, "unable to destroy object property interface");

done:
    FUNC_LEAVE(ret_value);
} 

struct PDC_cont_prop *PDCcont_prop_get_info(pdcid_t cont_prop)
{
    struct PDC_cont_prop *ret_value = NULL;
    struct PDC_cont_prop *info =  NULL;
    struct PDC_id_info *prop;
    
    FUNC_ENTER(NULL);
    
    prop = PDC_find_id(cont_prop);
    if(prop == NULL)
        PGOTO_ERROR(NULL, "cannot allocate container property");
    info = (struct PDC_cont_prop *)(prop->obj_ptr);
    
    ret_value = PDC_CALLOC(struct PDC_cont_prop);
    if(!ret_value)
        PGOTO_ERROR(NULL, "PDC container property memory allocation failed\n");
    ret_value->cont_life = info->cont_life;
    ret_value->cont_prop_id = info->cont_prop_id;
    ret_value->pdc = PDC_CALLOC(struct PDC_class);
    if(!ret_value->pdc)
        PGOTO_ERROR(NULL, "cannot allocate ret_value->pdc");
    if(info->pdc->name)
        ret_value->pdc->name = strdup(info->pdc->name);
    ret_value->pdc->local_id = info->pdc->local_id;
    
done:
    FUNC_LEAVE(ret_value);
} 

struct PDC_obj_prop *PDCobj_prop_get_info(pdcid_t obj_prop)
{
    struct PDC_obj_prop *ret_value = NULL;
    struct PDC_obj_prop *info =  NULL;
    struct PDC_id_info *prop;
    size_t i;
    
    FUNC_ENTER(NULL);
    
    prop = PDC_find_id(obj_prop);
    if(prop == NULL)
        PGOTO_ERROR(NULL, "cannot locate object property");
    info = (struct PDC_obj_prop *)(prop->obj_ptr);

    ret_value = PDC_CALLOC(struct PDC_obj_prop);
    if(ret_value == NULL)
        PGOTO_ERROR(NULL, "PDC object property memory allocation failed\n");
    memcpy(ret_value, info, sizeof(struct PDC_obj_prop));

    if(info->app_name)
        ret_value->app_name = strdup(info->app_name);

    ret_value->pdc = PDC_CALLOC(struct PDC_class);
    if(ret_value->pdc == NULL)
        PGOTO_ERROR(NULL, "cannot allocate ret_value->pdc");
    if(info->pdc->name)
        ret_value->pdc->name = strdup(info->pdc->name);
    ret_value->pdc->local_id = info->pdc->local_id;

    ret_value->dims = malloc(info->ndim*sizeof(uint64_t));
    if(ret_value->dims == NULL)
        PGOTO_ERROR(NULL, "cannot allocate ret_value->dims");
    for(i=0; i<info->ndim; i++)
        ret_value->dims[i] = info->dims[i];
  
    if(info->app_name)
        ret_value->app_name = strdup(info->app_name);
    if(info->data_loc)
        ret_value->data_loc = strdup(info->data_loc);
    if(info->tags)
        ret_value->tags = strdup(info->tags);

done:
    FUNC_LEAVE(ret_value);
} 


// Utility function for internal use.
perr_t
PDC_obj_prop_free(struct PDC_obj_prop *cp)
{
    return pdc_prop_obj_close(cp);
}

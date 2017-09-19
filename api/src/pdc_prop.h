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

#ifndef _pdc_prop_H
#define _pdc_prop_H

#include "pdc_error.h"
#include "pdc_interface.h"
#include "pdc_prop_pkg.h"


/**
 * PDC container and object property initialization
 *
 * \return Non-negative on success/Negative on failure
 */
perr_t PDCprop_init();

/**
 * Create PDC property 
 *
 * \param type [IN]             PDC property creation type (enum type), 
 *                              PDC_CONT_CREATE or PDC_OBJ_CREATE
 * \param id [IN]               Id of the PDC
 *
 * \return PDC property id on success (0 for container and 1 for object)/Negative on failure
 */
pdcid_t PDCprop_create(PDC_prop_type type, pdcid_t pdc_id);

/**
 * Close property
 *
 * \param id [IN]               Id of the property
 *
 * \return Non-negative on success/Negative on failure
 */
perr_t PDCprop_close(pdcid_t id);

/**
 * PDC container and object property finalize
 *
 * \return Non-negative on success/Negative on failure
 */
perr_t PDCprop_end();

/* private functions */

/**
 * Check if object property list is empty
 *
 * \return SUCCEED if empty/FAIL if not empty
 */
perr_t PDC_prop_obj_list_null();

/**
 * Check if container property list is empty
 *
 * \return SUCCEED if empty/FAIL if not empty
 */
perr_t PDC_prop_cont_list_null();

/**
 * Get container property infomation
 *
 * \param prop_id [IN]          Id of the property
 *
 * \return Pointer to PDC_cont_prop struct/Null on failure
 */
struct PDC_cont_prop *PDCcont_prop_get_info(pdcid_t prop_id);

/**
 * Get object property infomation
 *
 * \param prop_id [IN]          Id of the object property
 *
 * \return Pointer to PDC_obj_prop struct/Null on failure
 */
struct PDC_obj_prop *PDCobj_prop_get_info(pdcid_t prop_id);

#endif

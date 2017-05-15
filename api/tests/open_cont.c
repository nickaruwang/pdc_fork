#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdc.h"

int main() {
    struct PDC_prop p;
    pdcid_t pdc, create_prop, cont1, cont2, cont1_cp, cont2_cp;
    // create a pdc
    pdc = PDC_init(p);
    printf("create a new pdc, pdc id is: %lld\n", pdc);

    // create a container property
    create_prop = PDCprop_create(PDC_CONT_CREATE, pdc);
    if(create_prop > 0)
        printf("Create a container property, id is %lld\n", create_prop);
    else
        printf("Fail to create container property @ line  %d!\n", __LINE__);

    // create a container
    cont1 = PDCcont_create(pdc, "c1", create_prop);
    if(cont1 > 0)
        printf("Create a container, id is %lld\n", cont1);
    else
        printf("Fail to create container @ line  %d!\n", __LINE__);
       
    // create second container
    cont2 = PDCcont_create(pdc, "c2", create_prop);
    if(cont2 > 0)
        printf("Create a container, id is %lld\n", cont2);
    else
        printf("Fail to create container @ line  %d!\n", __LINE__);

    // open 1st container
    cont1_cp = PDCcont_open(pdc, "c1");
    if(cont1_cp < 0)
        printf("Fail to open container c1\n");
    else
        printf("Open container c1, id is %lld\n", cont1_cp);

    // open 2nd container
    cont2_cp = PDCcont_open(pdc, "c2");
    if(cont2_cp < 0)
        printf("Fail to open container c2\n");
    else
        printf("Open container c2, id is %lld\n", cont2_cp);

    // close cont1_cp
    if(PDCcont_close(cont1_cp, pdc) < 0)
        printf("fail to close container %lld\n", cont1_cp);
    else
        printf("successfully close container # %lld\n", cont1_cp);

    // close cont2_cp
    if(PDCcont_close(cont2_cp, pdc) < 0)
        printf("fail to close container %lld\n", cont2_cp);
    else
        printf("successfully close container # %lld\n", cont2_cp);

    // close cont1
    if(PDCcont_close(cont1, pdc) < 0)
        printf("fail to close container %lld\n", cont1);
    else
        printf("successfully close container # %lld\n", cont1);

    // close cont2
    if(PDCcont_close(cont2, pdc) < 0)
        printf("fail to close container %lld\n", cont2);
    else
        printf("successfully close container # %lld\n", cont2);

    // close a container property
    if(PDCprop_close(create_prop, pdc) < 0)
        printf("Fail to close property @ line %d\n", __LINE__);
    else
        printf("successfully close container property # %lld\n", create_prop);

    // close pdc
    if(PDC_close(pdc) < 0)
       printf("fail to close PDC\n");
    else
       printf("PDC is closed\n");
}

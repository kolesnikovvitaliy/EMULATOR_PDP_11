#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/utils.h"



register_t* register_new()
{
        return (register_t*)malloc(sizeof(register_t));
}

void reg_create(pdp_11_t* pdp, register_t* regist)
{
        pdp->R0 = &regist->R0;
        pdp->R1 = &regist->R1;
        pdp->R2 = &regist->R2;
        pdp->R3 = &regist->R3;
        pdp->R4 = &regist->R4;
        pdp->R5 = &regist->R5;
        pdp->R6 = &regist->R6;
        pdp->R7 = &regist->R7;
        return;

}

void reg_destroy(pdp_11_t* pdp)
{
        pdp->R0 = NULL;
        pdp->R1 = NULL;
        pdp->R2 = NULL;
        pdp->R3 = NULL;
        pdp->R4 = NULL;
        pdp->R5 = NULL;
        pdp->R6 = NULL;
        pdp->R7 = NULL;
        return;

}


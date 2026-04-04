#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "utils/utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

reg_t *
register_new()
{
    return (reg_t *) malloc(sizeof(reg_t));
}

void
reg_create(pdp_11_t *pdp, reg_t *regist)
{
    regist->R0 = 0;
    regist->R1 = 0;
    regist->R2 = 0;
    regist->R3 = 0;
    regist->R4 = 0;
    regist->R5 = 0;
    regist->SP = 0;
    regist->PC = 0;

    pdp->R0 = &regist->R0;
    pdp->R1 = &regist->R1;
    pdp->R2 = &regist->R2;
    pdp->R3 = &regist->R3;
    pdp->R4 = &regist->R4;
    pdp->R5 = &regist->R5;
    pdp->SP = &regist->SP;
    pdp->PC = &regist->PC;
    return;
}

void
reg_destroy(pdp_11_t *pdp)
{
    *pdp->R0 = 0;
    *pdp->R1 = 0;
    *pdp->R2 = 0;
    *pdp->R3 = 0;
    *pdp->R4 = 0;
    *pdp->R5 = 0;
    *pdp->SP = 0;
    *pdp->PC = 0;
    pdp->R0  = NULL;
    pdp->R1  = NULL;
    pdp->R2  = NULL;
    pdp->R3  = NULL;
    pdp->R4  = NULL;
    pdp->R5  = NULL;
    pdp->SP  = NULL;
    pdp->PC  = NULL;
    return;
}

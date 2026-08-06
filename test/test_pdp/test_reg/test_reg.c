#include "pdp_11/pdp_11.h"
#include "pdp_11/pdp_11_p.h"
#include "pdp_11/register/register_p.h"
#include "tests/test.h"
#include "tests/test_pdp/test_pdp.h"
#include "utils/logger/logger.h"
#include "utils/utils.h"

#include <assert.h>
#include <stdio.h>

int
test_reg(struct pdp_11_t *pdp)
{
    assert(pdp);
    *(((pdp_11_t *) pdp)->R5) = 034;
    // DEBUG("\n\n*(pdp->R5) = 0;\n", "");
    pdp_11_t *ptr_pdp    = (pdp_11_t *) pdp;
    reg_t *   ptr_regist = (reg_t *) ptr_pdp->regist;
    TRACE_LOG("TEST REGISTERS PDP_11", "");
    assert(*(ptr_pdp->R5) == ptr_regist->R5);
    // DEBUG("\n\npdp->regist->R5 = %o;\n\n", ptr_regist->R5);
    // DEBUG("\n\n(*(pdp->R5) == pdp->regist->R5)\n", "SUCCES");
    PRINT_RESULT("%s", " ... OK");
    INFO_LOG("THE REGISTER TEST WAS SUCCESSFUL\n", "");
    return 0;
}

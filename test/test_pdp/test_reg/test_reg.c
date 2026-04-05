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
    *(((pdp_11_t *) pdp)->R0) = 01000;
    DEBUG("\n\n*(pdp->R0) = 1000;\n", "");
    pdp_11_t *ptr_pdp    = (pdp_11_t *) pdp;
    reg_t *   ptr_regist = (reg_t *) ptr_pdp->regist;
    assert(*(ptr_pdp->R0) == ptr_regist->R0);
    DEBUG("\n\npdp->regist->R0 = %o;\n\n", ptr_regist->R0);
    DEBUG("\n\n(*(pdp->R0) == pdp->regist->R0)\n", "SUCCES");
    INFO("\n\nTHE REGISTER TEST WAS SUCCESSFUL\n", "");
    return 0;
}

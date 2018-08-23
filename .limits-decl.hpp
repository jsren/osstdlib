/* .limits-decl.hpp - (c) 2018 James Renwick */
#pragma once

extern "C++" {
namespace std
{
    enum float_round_style
    {
        round_indeterminate       = -1,
        round_toward_zero         = 0,
        round_to_nearest          = 1,
        round_toward_infinity     = 2,
        round_toward_neg_infinity = 3
    };

    enum float_denorm_style
    {
        denorm_indeterminate = -1,
        denorm_absent        = 0,
        denorm_present       = 1
    };
}
}
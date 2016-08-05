#pragma once

#define COIN_STRINGIZE_DETAIL(A) #A
#define COIN_STRINGIZE(A)    COIN_STRINGIZE_DETAIL(A)
#define COIN_PPCAT_NX(A, B)  A ## B
#define COIN_PPCAT(A, B)     COIN_PPCAT_NX(A, B)



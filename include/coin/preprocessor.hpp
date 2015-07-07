#pragma once

// turn A into a string literal 
#define COIN_STRINGIZE_NX(A) #A

// after macro expansion
#define COIN_STRINGIZE(A) STRINGIZE_NX(A)

// concatenate preprocessor tokens A and B
#define COIN_PPCAT_NX(A, B) A ## B

// after macro expansion
#define COIN_PPCAT(A, B) PPCAT_NX(A, B)


// (dev_lib)
// helper macros

//#pragma once

// needed in macros with templates
#define COMMA ,

// variadic code generation

#define FOR_0(what, x) what(x)
#define FOR_1(what, x, ...) what(x) FOR_0(what, __VA_ARGS__)
#define FOR_2(what, x, ...) what(x) FOR_1(what, __VA_ARGS__)
#define FOR_3(what, x, ...) what(x) FOR_2(what, __VA_ARGS__)
#define FOR_4(what, x, ...) what(x) FOR_3(what, __VA_ARGS__)
#define FOR_5(what, x, ...) what(x) FOR_4(what, __VA_ARGS__)
#define FOR_6(what, x, ...) what(x) FOR_5(what, __VA_ARGS__)
#define FOR_7(what, x, ...) what(x) FOR_6(what, __VA_ARGS__)
#define FOR_8(what, x, ...) what(x) FOR_7(what, __VA_ARGS__)
#define FOR_9(what, x, ...) what(x) FOR_8(what, __VA_ARGS__)

#define VA_NN_(_, _0,_1,_2,_3,_4,_5,_6,_7,_8, N, ...) N
#define VA_N_(...) VA_NN_(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, _)

#define CCONCAT_(a, b) a ## b
#define CONCAT_(a, b)  CCONCAT_(a, b)

#define FOR_(what, ...) CONCAT_(FOR_, VA_N_(__VA_ARGS__))(what, __VA_ARGS__)

// eof

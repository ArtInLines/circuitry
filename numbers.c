// #define BENCHMARK

#ifdef  BENCHMARK
#   define AIL_BENCH_PROFILE
#   ifndef BENCHMARK_COUNT
#       define BENCHMARK_COUNT 10
#   endif
#endif

#include "./ail/src/base/ail_base.h"
#include "./ail/src/bench/ail_bench.h"
#include <stdio.h>

#define NOT(a) (~a)
#define BNOT(a) (!(a))

#define SHIFTL(data, n) ((data)<<(n))
#define SHIFTR(data, n) ((data)>>(n))
#define SWITCH(activate_bit, data) ((activate_bit)*(data))
#define SELECT(activate_bit, data1, data2) ((activate_bit)*(data1) + BNOT(activate_bit)*(data2))
#define GET_BIT(data, bit_idx) (((data)>>(bit_idx))&1)
#define SET_BIT(data, bit_idx, value) ((data)&(~(1<<(bit_idx)))|((value)<<(bit_idx)))

#define _OR_2(a, b) ((a)|(b))
#define _OR_3(a, b, c) ((a)|(b)|(c))
#define _OR_4(a, b, c, d) ((a)|(b)|(c)|(d))
#define _OR_5(a, b, c, d, e) ((a)|(b)|(c)|(d)|(e))
#define _OR_6(a, b, c, d, e, f) ((a)|(b)|(c)|(d)|(e)|(f))
#define _OR_7(a, b, c, d, e, f, g) ((a)|(b)|(c)|(d)|(e)|(f)|(g))
#define _OR_8(a, b, c, d, e, f, g, h) ((a)|(b)|(c)|(d)|(e)|(f)|(g)|(h))
#define OR(...) AIL_VFUNC(_OR_, __VA_ARGS__)
#define BOR(...) GET_BIT(OR(__VA_ARGS__), 0)

#define _AND_2(a, b) ((a)&(b))
#define _AND_3(a, b, c) ((a)&(b)&(c))
#define _AND_4(a, b, c, d) ((a)&(b)&(c)&(d))
#define _AND_5(a, b, c, d, e) ((a)&(b)&(c)&(d)&(e))
#define _AND_6(a, b, c, d, e, f) ((a)&(b)&(c)&(d)&(e)&(f))
#define _AND_7(a, b, c, d, e, f, g) ((a)&(b)&(c)&(d)&(e)&(f)&(g))
#define _AND_8(a, b, c, d, e, f, g, h) ((a)&(b)&(c)&(d)&(e)&(f)&(g)&(h))
#define AND(...) AIL_VFUNC(_AND_, __VA_ARGS__)
#define BAND(...) GET_BIT(AND(__VA_ARGS__), 0)

#define _XOR_2(a, b) ((a)^(b))
#define _XOR_3(a, b, c) ((a)^(b)^(c))
#define _XOR_4(a, b, c, d) ((a)^(b)^(c)^(d))
#define _XOR_5(a, b, c, d, e) ((a)^(b)^(c)^(d)^(e))
#define _XOR_6(a, b, c, d, e, f) ((a)^(b)^(c)^(d)^(e)^(f))
#define _XOR_7(a, b, c, d, e, f, g) ((a)^(b)^(c)^(d)^(e)^(f)^(g))
#define _XOR_8(a, b, c, d, e, f, g, h) ((a)^(b)^(c)^(d)^(e)^(f)^(g)^(h))
#define XOR(...) AIL_VFUNC(_XOR_, __VA_ARGS__)
#define BXOR(...) GET_BIT(XOR(__VA_ARGS__), 0)

#define _NAND_2(a, b) (~((a)&(b)))
#define _NAND_3(a, b, c) (~((a)&(b)&(c)))
#define _NAND_4(a, b, c, d) (~((a)&(b)&(c)&(d)))
#define _NAND_5(a, b, c, d, e) (~((a)&(b)&(c)&(d)&(e)))
#define _NAND_6(a, b, c, d, e, f) (~((a)&(b)&(c)&(d)&(e)&(f)))
#define _NAND_7(a, b, c, d, e, f, g) (~((a)&(b)&(c)&(d)&(e)&(f)&(g)))
#define _NAND_8(a, b, c, d, e, f, g, h) (~((a)&(b)&(c)&(d)&(e)&(f)&(g)&(h)))
#define NAND(...) AIL_VFUNC(_NAND_, __VA_ARGS__)
#define BNAND(...) GET_BIT(NAND(__VA_ARGS__), 0)

#define _NOR_2(a, b) (~((a)|(b)))
#define _NOR_3(a, b, c) (~((a)|(b)|(c)))
#define _NOR_4(a, b, c, d) (~((a)|(b)|(c)|(d)))
#define _NOR_5(a, b, c, d, e) (~((a)|(b)|(c)|(d)|(e)))
#define _NOR_6(a, b, c, d, e, f) (~((a)|(b)|(c)|(d)|(e)|(f)))
#define _NOR_7(a, b, c, d, e, f, g) (~((a)|(b)|(c)|(d)|(e)|(f)|(g)))
#define _NOR_8(a, b, c, d, e, f, g, h) (~((a)|(b)|(c)|(d)|(e)|(f)|(g)|(h)))
#define NOR(...) AIL_VFUNC(_NOR_, __VA_ARGS__)
#define BNOR(...) GET_BIT(NOR(__VA_ARGS__), 0)

#ifdef BENCHMARK
#   define ASSERT(expr, ...) AIL_UNUSED(expr)
#else
#   define ASSERT(expr, goto_label, test_type, fmt, ...) do { \
        if (!(expr)) { \
            printf(test_type " failed at " __FILE__ ":" AIL_STR_LINE " '" AIL_STRINGIFY(expr) "':\n"); \
            printf("  " fmt "\n", __VA_ARGS__); \
            goto goto_label; \
        } \
    } while(0)
#endif


typedef struct {
    i16 q, r;
} DivResult;

///////////////////
// Two's Complement
///////////////////

i8 intTrunc(i16 x) {
    return OR(AND(x, 0x7f), SHIFTL(GET_BIT(x, 15), 7));
}
i16 negInt16(i16 x) {
    return XOR(x, NOT(0)) + 1;
}
i8 negInt8(i8 x) {
    return XOR(x, NOT(0)) + 1;
}

i16 addInt16(i16 a, i16 b) {
    AIL_BENCH_PROFILE_START(addInt16);
    // Ripple Carry Adder
    i16 res = 0;
    u8 c = 0;
    for (int i = 0; i < 16; i++) {
        u8 ab = GET_BIT(a, i);
        u8 bb = GET_BIT(b, i);
        res = SET_BIT(res, i, BXOR(ab, bb, c));
        c   = OR(AND(ab, bb), AND(c, BXOR(ab, bb)));
    }
    AIL_BENCH_PROFILE_END(addInt16);
    return res;
}
i8 addInt8(i8 a, i8 b) {
    return intTrunc(addInt16((i16)a, (i16)b));
}

i16 subInt16(i16 a, i16 b) {
    // Subtraction is just addition in two's complement ;)
    return addInt16(a, negInt16(b));
}

i16 mulInt16(i16 a, i16 b) {
    AIL_BENCH_PROFILE_START(mulInt16);
    // Long Multiplication
    i16 res = 0;
    for (int i = 0; i < 16; i++) {
        res = addInt16(res, SWITCH(GET_BIT(b, i), SHIFTL(a, i)));
    }
    AIL_BENCH_PROFILE_END(mulInt16);
    return res;
}

DivResult fullDivInt16(i16 a, i16 b) {
    AIL_BENCH_PROFILE_START(divInt16);
    // Euclidean Division
    u8  an   = BAND(GET_BIT(a, 15), 1);
    u8  bn   = BAND(GET_BIT(b, 15), 1);
    i16 num  = SELECT(an, negInt16(a), a);
    i16 den  = SELECT(bn, negInt16(b), b);
    i16 quot = 0;
    i16 rem  = num;
    while (rem >= den) {
        quot = addInt16(quot, 1);
        rem  = subInt16(rem, den);
    }
    rem  = SELECT(bn, rem, negInt16(rem));
    quot = SELECT(BNOT(bn), quot, negInt16(quot));
    quot = SELECT(BNOT(an), quot, SELECT(rem == 0, negInt16(quot), addInt16(negInt16(quot), -1)));
    AIL_BENCH_PROFILE_END(divInt16);
    return (DivResult) { .q = quot, .r = rem };
}
i16 divInt16(i16 a, i16 b) {
    return fullDivInt16(a, b).q;
}
i16 remInt16(i16 a, i16 b) {
    return fullDivInt16(a, b).r;
}


///////////////////
// Sign Magnitude
///////////////////

typedef u8 SignMag8;
typedef u16 SignMag16;
typedef struct { SignMag16 q, r; } SignMagDivResult;

i8 intFromSignMag8(SignMag8 num) {
    u8 pos = SET_BIT(num, 7, 0);
    return SELECT(GET_BIT(num, 7), negInt8(pos), pos);
}
i16 intFromSignMag16(SignMag16 num) {
    u16 pos = SET_BIT(num, 15, 0);
    return SELECT(GET_BIT(num, 15), negInt16(pos), pos);
}
SignMag8 signMagFromInt8(i8 num) {
    return SELECT(BNOT(GET_BIT(num, 7)), (num), (SET_BIT(addInt16(NOT(num), 1), 7, 1)));
}
SignMag16 signMagFromInt16(i16 num) {
    return SELECT(BNOT(GET_BIT(num, 15)), (num), (SET_BIT(addInt16(NOT(num), 1), 15, 1)));
}
SignMag16 signMagExt(SignMag8 num) {
    return SET_BIT(SET_BIT(num, 7, 0), 15, GET_BIT(num, 7));
}
SignMag8 signMagTrunc(SignMag16 num) {
    return SET_BIT(AND(num, 0x7f), 7, GET_BIT(num, 15));
}

SignMag16 negSignMag16(SignMag16 x) {
    return SET_BIT(x, 15, BNOT(GET_BIT(x, 15)));
}
SignMag8 negSignMag8(SignMag8 x) {
    return SET_BIT(x, 8, BNOT(GET_BIT(x, 8)));
}

SignMag16 addSignMag16(SignMag16 a, SignMag16 b) {
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    u16 res   = OR(
        SWITCH(AND(asign, bsign),  SET_BIT(addInt16(abits, bbits), 15, 1)),
        SWITCH(XOR(asign, bsign),  SELECT(abits >= bbits, SET_BIT(subInt16(abits, bbits), 15, asign), SET_BIT(subInt16(bbits, abits), 15, bsign))),
        SWITCH(BNOR(asign, bsign), addInt16(abits, bbits))
    );
    return res;
}

SignMag16 subSignMag16(SignMag16 a, SignMag16 b) {
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    u16 res   =  OR(
        SWITCH(AND(asign, bsign),  SELECT(bbits >= abits, (subInt16(bbits, abits)), SET_BIT(subInt16(abits, bbits), 15, 1))),
        SWITCH(XOR(asign, bsign),  SET_BIT(addInt16(abits, bbits), 15, asign)),
        SWITCH(BNOR(asign, bsign), SELECT(abits >= bbits, subInt16(abits, bbits), SET_BIT(subInt16(bbits, abits), 15, 1)))
    );
    return res;
}

SignMag16 mulSignMag16(SignMag16 a, SignMag16 b) {
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    u16 res   =  OR(
        SWITCH(AND(asign, bsign),  mulInt16(abits, bbits)),
        SWITCH(BNOR(asign, bsign), mulInt16(abits, bbits)),
        SWITCH(XOR(asign, bsign),  SET_BIT(mulInt16(abits, bbits), 15, 1))
    );
    return res;
}

SignMagDivResult fullDivSignMag16(SignMag16 a, SignMag16 b) {
    // Euclidean Division
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    SignMag16 quot = 0;
    SignMag16 rem  = abits;
    while (rem >= bbits) {
        quot = addSignMag16(quot, 1);
        rem  = subSignMag16(rem, bbits);
    }
    rem  = SELECT(asign, negSignMag16(rem),  rem);
    quot = SELECT(BXOR(asign, bsign), negSignMag16(quot), quot);
    return (SignMagDivResult) { .q = quot, .r = rem };
}
SignMag16 divSignMag16(SignMag16 a, SignMag16 b) {
    return fullDivInt16(a, b).q;
}
SignMag16 remSignMag16(SignMag16 a, SignMag16 b) {
    return fullDivInt16(a, b).r;
}


///////////////////
// One's Complement
///////////////////

typedef u8 OnesComp8;
typedef u16 OnesComp16;
typedef struct { OnesComp16 q, r; } OnesCompDivResult;

i8 intFromOnesComp8(OnesComp8 num) {
    return SELECT(GET_BIT(num, 7), addInt8(num, 1), num);
}
i16 intFromOnesComp16(OnesComp16 num) {
    return SELECT(GET_BIT(num, 15), addInt16(num, 1), num);
}
OnesComp8 onesCompFromInt8(i8 num) {
    return SELECT(GET_BIT(num, 7), addInt8(num, -1), num);
}
OnesComp16 onesCompFromInt16(i16 num) {
    return SELECT(GET_BIT(num, 15), addInt16(num, -1), num);
}
OnesComp16 onesCompExt(OnesComp8 num) {
    return SELECT(GET_BIT(num, 7), OR(num, 0xff00), num);
}
OnesComp8 onesCompTrunc(OnesComp16 num) {
    return OR(AND(num, 0x7f), SHIFTL(GET_BIT(num, 15), 7));
}

OnesComp16 negOnesComp16(OnesComp16 x) {
    return XOR(x, NOT(0));
}
OnesComp8 negOnesComp8(OnesComp8 x) {
    return XOR(x, NOT(0));
}

OnesComp16 addOnesComp16(OnesComp16 a, OnesComp16 b) {
    // Ripple Carry Adder
    OnesComp16 res = 0;
    u8 c = 0;
    for (int i = 0; i < 16; i++) {
        u8 ab = GET_BIT(a, i);
        u8 bb = GET_BIT(b, i);
        res = SET_BIT(res, i, BXOR(ab, bb, c));
        c   = OR(AND(ab, bb), AND(c, OR(ab, bb)));
    }
    return res;
}

OnesComp16 subOnesComp16(OnesComp16 a, OnesComp16 b) {
    return addOnesComp16(a, negOnesComp16(b));
}

OnesComp16 mulOnesComp16(OnesComp16 a, OnesComp16 b) {
    return mulInt16(a, b);
}

OnesCompDivResult fullDivOnesComp16(OnesComp16 a, OnesComp16 b) {
    DivResult res = fullDivInt16(a, b);
    return (OnesCompDivResult) { .q = res.q, .r = res.r };
}
OnesComp16 divOnesComp16(OnesComp16 a, OnesComp16 b) {
    return fullDivOnesComp16(a, b).q;
}
OnesComp16 remOnesComp16(OnesComp16 a, OnesComp16 b) {
    return fullDivOnesComp16(a, b).r;
}


///////////////////
// Binary Offset
///////////////////

typedef u8 BinOffset8; typedef u16 BinOffset16;


///////////////////
// Base -2
///////////////////

typedef u8 BaseNeg28;  typedef u16 BaseNeg216;


///////////////////
// Tests
///////////////////

#define TEST_CONV(tu, tl) do {                                                                                                                               \
        AIL_BENCH_PROFILE_START(AIL_CONCAT(test, tu, Conv));                                                                                                 \
        for (int i = 0; i <= 0xffff; i++) {                                                                                                                  \
            i16 x = (i16)i;                                                                                                                                  \
            if (x != (i16)0x8000) {                                                                                                                          \
                AIL_CONCAT(tu, 16) val16 = AIL_CONCAT(tl, FromInt16)(x);                                                                                     \
                ASSERT(x == AIL_CONCAT(intFrom, tu, 16)(val16), AIL_CONCAT(tl, ConvFail), AIL_STRINGIFY(tu) "-Conversion",                                   \
                    "Conversion: %d -> 0x%04x -> %d", x, val16,AIL_CONCAT(intFrom, tu, 16)(val16));                                                          \
                if (intTrunc(x) != (i8)0x80) {                                                                                                               \
                    AIL_CONCAT(tu, 8) val8 = AIL_CONCAT(tl, FromInt8)(intTrunc(x));                                                                          \
                    ASSERT(intTrunc(x) == (i8)0x80 || AIL_CONCAT(tl, Trunc)(val16) == val8, AIL_CONCAT(tl, ConvFail), AIL_STRINGIFY(tu) "-Conversion",       \
                        "Truncation: %d -> %d (expected %d) | 0x%04x -> 0x%02x (expected 0x%02x)",                                                           \
                        x, AIL_CONCAT(intFrom, tu, 8)(AIL_CONCAT(tl, Trunc)(val16)), intTrunc(x), val16, AIL_CONCAT(tl, Trunc)(val16), val8);                \
                    ASSERT(intTrunc(x) == (i8)0x80 || AIL_CONCAT(tl, Ext)(val8) == AIL_CONCAT(tl, FromInt16)(intTrunc(x)),                                   \
                        AIL_CONCAT(tl, ConvFail), AIL_STRINGIFY(tu) "-Conversion", "Extension: %d -> %d (expected %d) | 0x%04x -> 0x%04x (expected 0x%04x)", \
                        intTrunc(x), AIL_CONCAT(intFrom, tu, 16)(AIL_CONCAT(tl, Ext)(val8)), intTrunc(x),                                                    \
                        val8, AIL_CONCAT(tl, Ext)(val8), AIL_CONCAT(tl, FromInt16)(intTrunc(x)));                                                            \
                }                                                                                                                                            \
            }                                                                                                                                                \
        }                                                                                                                                                    \
        printf(AIL_STRINGIFY(tu) "-Conversion tested successfully\n");                                                                                       \
    AIL_CONCAT(tl, ConvFail):                                                                                                                                \
        AIL_BENCH_PROFILE_END(AIL_CONCAT(test, tu, Conv));                                                                                                   \
    } while(0)

#define TEST_NEG(tu, tl) do { \
        AIL_BENCH_PROFILE_START(AIL_CONCAT(test, tu, Neg)); \
        for (int i = 0; i <= 0xffff; i++) { \
            i16 x = (i16)i; \
            AIL_CONCAT(tu, 16) val = AIL_CONCAT(tl, FromInt16)(x); \
            ASSERT(val == AIL_CONCAT(neg, tu, 16)(AIL_CONCAT(neg, tu, 16)(val)), AIL_CONCAT(tl, NegFail), AIL_STRINGIFY(tu) "-Negation", \
                "Negation: 0x%02x -> 0x%02x -> 0x%02x", \
                val, AIL_CONCAT(neg, tu, 16)(val), AIL_CONCAT(neg, tu, 16)(AIL_CONCAT(neg, tu, 16)(val))); \
            if (x != 0 && x != (i16)0x8000) { \
                ASSERT(AIL_CONCAT(tl, FromInt16)(-x) == AIL_CONCAT(neg, tu, 16)(val), AIL_CONCAT(tl, NegFail), AIL_STRINGIFY(tu) "-Negation", \
                    "%d -> 0x%02x != 0x%02x -> 0x%02x", -x, AIL_CONCAT(tl, FromInt16)(-x), val, AIL_CONCAT(neg, tu, 16)(val)); \
            } \
        } \
        printf(AIL_STRINGIFY(tu) "-Negation tested successfully\n"); \
    AIL_CONCAT(tl, NegFail): \
        AIL_BENCH_PROFILE_END(AIL_CONCAT(test, tu, Neg)); \
    } while(0)

#define TEST_ADD(tu, tl) do { \
        AIL_BENCH_PROFILE_START(AIL_CONCAT(test, tu, Add)); \
        for (int i = -128; i <= 128; i++) { \
            for (int j = -128; j <= 128; j++) { \
                AIL_CONCAT(tu, 16) a = AIL_CONCAT(tl, FromInt16)(i); \
                AIL_CONCAT(tu, 16) b = AIL_CONCAT(tl, FromInt16)(j); \
                i16 expected = AIL_CONCAT(intFrom, tu, 16)(a) + AIL_CONCAT(intFrom, tu, 16)(b); \
                AIL_CONCAT(tu, 16) res = AIL_CONCAT(add, tu, 16)(a, b); \
                ASSERT(expected == AIL_CONCAT(intFrom, tu, 16)(res), AIL_CONCAT(tl, AddFail), AIL_STRINGIFY(tu) "-Addition", \
                    "%d + %d = %d (expected %d) | 0x%02x + 0x%02x = 0x%02x (expected 0x%02x)", \
                    AIL_CONCAT(intFrom, tu, 16)(a), AIL_CONCAT(intFrom, tu, 16)(b), AIL_CONCAT(intFrom, tu, 16)(res), expected, \
                    a, b, (u16)res, AIL_CONCAT(tl, FromInt16)(expected)); \
            } \
        } \
        printf(AIL_STRINGIFY(tu) "-Addition tested successfully\n"); \
    AIL_CONCAT(tl, AddFail): \
        AIL_BENCH_PROFILE_END(AIL_CONCAT(test, tu, Add)); \
    } while(0)

#define TEST_SUB(tu, tl) do { \
        AIL_BENCH_PROFILE_START(AIL_CONCAT(test, tu, Sub)); \
        for (int i = -128; i <= 128; i++) { \
            for (int j = -128; j <= 128; j++) { \
                AIL_CONCAT(tu, 16) a = AIL_CONCAT(tl, FromInt16)(i); \
                AIL_CONCAT(tu, 16) b = AIL_CONCAT(tl, FromInt16)(j); \
                i16 expected = AIL_CONCAT(intFrom, tu, 16)(a) - AIL_CONCAT(intFrom, tu, 16)(b); \
                AIL_CONCAT(tu, 16) res = AIL_CONCAT(sub, tu, 16)(a, b); \
                ASSERT(expected == AIL_CONCAT(intFrom, tu, 16)(res), AIL_CONCAT(tl, SubFail), AIL_STRINGIFY(tu) "-Subtraction", \
                    "%d - %d = %d (expected %d) | 0x%02x - 0x%02x = 0x%02x (expected 0x%02x)", \
                    AIL_CONCAT(intFrom, tu, 16)(a), AIL_CONCAT(intFrom, tu, 16)(b), AIL_CONCAT(intFrom, tu, 16)(res), expected, \
                    a, b, (u16)res, AIL_CONCAT(tl, FromInt16)(expected)); \
            } \
        } \
        printf(AIL_STRINGIFY(tu) "-Subtraction tested successfully\n"); \
    AIL_CONCAT(tl, SubFail): \
        AIL_BENCH_PROFILE_END(AIL_CONCAT(test, tu, Sub)); \
    } while(0)

#define TEST_MUL(tu, tl) do { \
        AIL_BENCH_PROFILE_START(AIL_CONCAT(test, tu, Mul)); \
        for (int i = -128; i <= 128; i++) { \
            for (int j = -128; j <= 128; j++) { \
                AIL_CONCAT(tu, 16) a = AIL_CONCAT(tl, FromInt16)(i); \
                AIL_CONCAT(tu, 16) b = AIL_CONCAT(tl, FromInt16)(j); \
                i16 expected = AIL_CONCAT(intFrom, tu, 16)(a) * AIL_CONCAT(intFrom, tu, 16)(b); \
                AIL_CONCAT(tu, 16) res = AIL_CONCAT(mul, tu, 16)(a, b); \
                ASSERT(expected == AIL_CONCAT(intFrom, tu, 16)(res), AIL_CONCAT(tl, MulFail), AIL_STRINGIFY(tu) "-Multiplication", \
                    "%d * %d = %d (expected %d) | 0x%02x * 0x%02x = 0x%02x (expected 0x%02x)", \
                    AIL_CONCAT(intFrom, tu, 16)(a), AIL_CONCAT(intFrom, tu, 16)(b), AIL_CONCAT(intFrom, tu, 16)(res), expected, \
                    a, b, (u16)res, AIL_CONCAT(tl, FromInt16)(expected)); \
            } \
        } \
        printf(AIL_STRINGIFY(tu) "-Multiplication tested successfully\n"); \
    AIL_CONCAT(tl, MulFail): \
        AIL_BENCH_PROFILE_END(AIL_CONCAT(test, tu, Mul)); \
    } while(0)

// @TODO: Add test (quot*b + rem == a)
#define TEST_DIV(tu, tl) do { \
        AIL_BENCH_PROFILE_START(AIL_CONCAT(test, tu, Div)); \
        for (int i = -128; i <= 128; i++) { \
            for (int j = -128; j <= 128; j++) { \
                AIL_CONCAT(tu, 16) a = AIL_CONCAT(tl, FromInt16)(i); \
                AIL_CONCAT(tu, 16) b = AIL_CONCAT(tl, FromInt16)(j); \
                if (!b) continue; \
                i16 expectedQ = AIL_CONCAT(intFrom, tu, 16)(a) / AIL_CONCAT(intFrom, tu, 16)(b); \
                i16 expectedR = AIL_CONCAT(intFrom, tu, 16)(a) % AIL_CONCAT(intFrom, tu, 16)(b); \
                AIL_CONCAT(tu, DivResult) res = AIL_CONCAT(fullDiv, tu, 16)(a, b); \
                ASSERT(expectedQ == AIL_CONCAT(intFrom, tu, 16)(res.q), AIL_CONCAT(tl, DivFail), AIL_STRINGIFY(tu) "-Division", \
                    "%d / %d = %d (expected %d) | 0x%02x / 0x%02x = 0x%02x (expected 0x%02x)", \
                    AIL_CONCAT(intFrom, tu, 16)(a), AIL_CONCAT(intFrom, tu, 16)(b), AIL_CONCAT(intFrom, tu, 16)(res.q), expectedQ, \
                    a, b, (u16)res.q, AIL_CONCAT(tl, FromInt16)(expectedQ)); \
                ASSERT(expectedR == AIL_CONCAT(intFrom, tu, 16)(res.r), AIL_CONCAT(tl, DivFail), AIL_STRINGIFY(tu) "-Division", \
                    "%d %% %d = %d (expected %d) | 0x%02x %% 0x%02x = 0x%02x (expected 0x%02x)", \
                    AIL_CONCAT(intFrom, tu, 16)(a), AIL_CONCAT(intFrom, tu, 16)(b), AIL_CONCAT(intFrom, tu, 16)(res.r), expectedR, \
                    a, b, (u16)res.r, AIL_CONCAT(tl, FromInt16)(expectedR)); \
            } \
        } \
        printf(AIL_STRINGIFY(tu) "-Division tested successfully\n"); \
    AIL_CONCAT(tl, DivFail): \
        AIL_BENCH_PROFILE_END(AIL_CONCAT(test, tu, Div)); \
    } while(0)

#define TEST_ALL(tu, tl) do { \
        TEST_CONV(tu, tl);    \
        TEST_NEG(tu, tl);     \
        TEST_ADD(tu, tl);     \
        TEST_SUB(tu, tl);     \
        TEST_MUL(tu, tl);     \
        TEST_DIV(tu, tl);     \
    } while(0)

int main(void) {
#ifdef BENCHMARK
    ail_bench_init();
    ail_bench_begin_profile();
    for (int i = 0; i < BENCHMARK_COUNT; i++)
#endif
    { // Tests
        TEST_ALL(SignMag, signMag);
        TEST_ALL(OnesComp, onesComp);
    }
#ifdef BENCHMARK
    printf("--------\n");
    printf("Profiling results:\n");
    ail_bench_end_and_print_profile(16, true);
#endif
    return 0;
}

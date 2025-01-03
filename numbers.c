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
#   define ASSERT(expr, test_type, fmt, ...) do { \
        if (!(expr)) { \
            printf("%s failed at " __FILE__ ":" AIL_STR_LINE " '" AIL_STRINGIFY(expr) "':\n", test_type); \
            printf("  " fmt "\n", __VA_ARGS__); \
            return false; \
        } \
    } while(0)
#endif


///////////////////
// Abstract Format Type
///////////////////

typedef struct { u16 q, r; } DivResult;
typedef u16 (*fromIntFunc)(i16 num);
typedef u8  (*fromInt8Func)(i8 num);
typedef i16 (*toIntFunc)(u16 num);
typedef u16 (*extendFunc)(u8 num);
typedef u8  (*truncFunc)(u16 num);
typedef u16 (*negFunc)(u16 num);
typedef u16 (*addFunc)(u16 a, u16 b);
typedef u16 (*subFunc)(u16 a, u16 b);
typedef u16 (*mulFunc)(u16 a, u16 b);
typedef DivResult (*divFunc)(u16 a, u16 b);

typedef struct {
    const char *name;
    fromIntFunc  fromInt;
    fromInt8Func fromInt8;
    toIntFunc    toInt;
    extendFunc   extend;
    truncFunc    trunc;
    negFunc      neg;
    addFunc      add;
    subFunc      sub;
    mulFunc      mul;
    divFunc      div;
} IntFormat;

// Utility Function
i8 intTrunc(i16 x) {
    return OR(AND(x, 0x7f), SHIFTL(GET_BIT(x, 15), 7));
}


///////////////////
// Two's Complement
///////////////////

u16 twosCompFromInt(i16 num) {
    return *(u16*)&num; // Just casts the type
}
u8 twosCompFromInt8(i8 num) {
    return *(u8*)&num; // Just casts the type
}
i16 twosCompToInt(u16 num) {
    return *(i16*)&num; // Just casts the type
}
u16 twosCompExtend(u8 x) {
    return SELECT(GET_BIT(x, 7), OR((u16)x, 0xff00), x);
}
u8 twosCompTrunc(u16 x) {
    return OR(AND(x, 0x7f), SHIFTL(GET_BIT(x, 15), 7));
}
u16 twosCompNeg(u16 x) {
    return XOR(x, NOT(0)) + 1;
}

u16 twosCompAdd(u16 a, u16 b) {
    AIL_BENCH_PROFILE_START(twosCompAdd);
    // Ripple Carry Adder
    u16 res = 0;
    u8 c = 0;
    for (int i = 0; i < 16; i++) {
        u8 ab = GET_BIT(a, i);
        u8 bb = GET_BIT(b, i);
        res = SET_BIT(res, i, BXOR(ab, bb, c));
        c   = OR(AND(ab, bb), AND(c, BXOR(ab, bb)));
    }
    AIL_BENCH_PROFILE_END(twosCompAdd);
    return res;
}
u16 twosCompSub(u16 a, u16 b) {
    // Subtraction is just addition in two's complement ;)
    AIL_BENCH_PROFILE_START(twosCompSub);
    u16 res = twosCompAdd(a, twosCompNeg(b));
    AIL_BENCH_PROFILE_END(twosCompSub);
    return res;
}
u16 twosCompMul(u16 a, u16 b) {
    AIL_BENCH_PROFILE_START(twosCompMul);
    // Long Multiplication
    u16 res = 0;
    for (int i = 0; i < 16; i++) {
        res = twosCompAdd(res, SWITCH(GET_BIT(b, i), SHIFTL(a, i)));
    }
    AIL_BENCH_PROFILE_END(twosCompMul);
    return res;
}
DivResult twosCompDiv(u16 a, u16 b) {
    AIL_BENCH_PROFILE_START(twosCompDiv);
    // Euclidean Division
    u8  an   = BAND(GET_BIT(a, 15), 1);
    u8  bn   = BAND(GET_BIT(b, 15), 1);
    u16 num  = SELECT(an, twosCompNeg(a), a);
    u16 den  = SELECT(bn, twosCompNeg(b), b);
    u16 quot = 0;
    u16 rem  = num;
    while (rem >= den) {
        quot = twosCompAdd(quot, 1);
        rem  = twosCompSub(rem, den);
    }
    rem  = SELECT(an, twosCompNeg(rem), rem);
    quot = SELECT(BXOR(an, bn), twosCompNeg(quot), quot);
    AIL_BENCH_PROFILE_END(twosCompDiv);
    return (DivResult) { .q = quot, .r = rem };
}

global IntFormat TwosComplement = {
    .name = "Two's Complement",
    .fromInt  = &twosCompFromInt,
    .fromInt8 = &twosCompFromInt8,
    .toInt    = &twosCompToInt,
    .extend   = &twosCompExtend,
    .trunc    = &twosCompTrunc,
    .neg = &twosCompNeg,
    .add = &twosCompAdd,
    .sub = &twosCompSub,
    .mul = &twosCompMul,
    .div = &twosCompDiv,
};


///////////////////
// Sign Magnitude
///////////////////

typedef u8 SignMag8;
typedef u16 SignMag16;

i16 signMagToInt(SignMag16 num) {
    u16 pos = SET_BIT(num, 15, 0);
    return SELECT(GET_BIT(num, 15), twosCompNeg(pos), pos);
}
SignMag16 signMagFromInt(i16 num) {
    return SELECT(BNOT(GET_BIT(num, 15)), (num), (SET_BIT(twosCompAdd(NOT(num), 1), 15, 1)));
}
SignMag8 signMagFromInt8(i8 num) {
    return SELECT(BNOT(GET_BIT(num, 7)), (num), (SET_BIT(twosCompAdd(NOT(num), 1), 7, 1)));
}
SignMag16 signMagExtend(SignMag8 num) {
    return SET_BIT(SET_BIT(num, 7, 0), 15, GET_BIT(num, 7));
}
SignMag8 signMagTrunc(SignMag16 num) {
    return SET_BIT(AND(num, 0x7f), 7, GET_BIT(num, 15));
}
SignMag16 signMagNeg(SignMag16 x) {
    return SET_BIT(x, 15, BNOT(GET_BIT(x, 15)));
}

SignMag16 signMagAdd(SignMag16 a, SignMag16 b) {
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    u16 res   = OR(
        SWITCH(AND(asign, bsign),  SET_BIT(twosCompAdd(abits, bbits), 15, 1)),
        SWITCH(XOR(asign, bsign),  SELECT(abits >= bbits, SET_BIT(twosCompSub(abits, bbits), 15, asign), SET_BIT(twosCompSub(bbits, abits), 15, bsign))),
        SWITCH(BNOR(asign, bsign), twosCompAdd(abits, bbits))
    );
    return res;
}
SignMag16 signMagSub(SignMag16 a, SignMag16 b) {
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    u16 res   =  OR(
        SWITCH(AND(asign, bsign),  SELECT(bbits >= abits, (twosCompSub(bbits, abits)), SET_BIT(twosCompSub(abits, bbits), 15, 1))),
        SWITCH(XOR(asign, bsign),  SET_BIT(twosCompAdd(abits, bbits), 15, asign)),
        SWITCH(BNOR(asign, bsign), SELECT(abits >= bbits, twosCompSub(abits, bbits), SET_BIT(twosCompSub(bbits, abits), 15, 1)))
    );
    return res;
}
SignMag16 signMagMul(SignMag16 a, SignMag16 b) {
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    u16 res   =  OR(
        SWITCH(AND(asign, bsign),  twosCompMul(abits, bbits)),
        SWITCH(BNOR(asign, bsign), twosCompMul(abits, bbits)),
        SWITCH(XOR(asign, bsign),  SET_BIT(twosCompMul(abits, bbits), 15, 1))
    );
    return res;
}
DivResult signMagDiv(SignMag16 a, SignMag16 b) {
    // Euclidean Division
    u16 asign = GET_BIT(a, 15);
    u16 bsign = GET_BIT(b, 15);
    u16 abits = SET_BIT(a, 15, 0);
    u16 bbits = SET_BIT(b, 15, 0);
    SignMag16 quot = 0;
    SignMag16 rem  = abits;
    while (rem >= bbits) {
        quot = signMagAdd(quot, 1);
        rem  = signMagSub(rem, bbits);
    }
    rem  = SELECT(asign, signMagNeg(rem),  rem);
    quot = SELECT(BXOR(asign, bsign), signMagNeg(quot), quot);
    return (DivResult) { .q = quot, .r = rem };
}

global IntFormat SignMagnitude = {
    .name = "Sign Magnitude",
    .fromInt  = &signMagFromInt,
    .fromInt8 = &signMagFromInt8,
    .toInt    = &signMagToInt,
    .extend   = &signMagExtend,
    .trunc    = &signMagTrunc,
    .neg = &signMagNeg,
    .add = &signMagAdd,
    .sub = &signMagSub,
    .mul = &signMagMul,
    .div = &signMagDiv,
};


///////////////////
// One's Complement
///////////////////

typedef u8 OnesComp8;
typedef u16 OnesComp16;

i16 onesCompToInt(OnesComp16 num) {
    return SELECT(GET_BIT(num, 15), twosCompAdd(num, 1), num);
}
OnesComp16 onesCompFromInt(i16 num) {
    return SELECT(GET_BIT(num, 15), twosCompAdd(num, -1), num);
}
OnesComp8 onesCompFromInt8(i8 num) {
    return SELECT(GET_BIT(num, 7), twosCompAdd(num, -1), num);
}
OnesComp16 onesCompExtend(OnesComp8 num) {
    return SELECT(GET_BIT(num, 7), OR(num, 0xff00), num);
}
OnesComp8 onesCompTrunc(OnesComp16 num) {
    return OR(AND(num, 0x7f), SHIFTL(GET_BIT(num, 15), 7));
}
OnesComp16 onesCompNeg(OnesComp16 x) {
    return XOR(x, NOT(0));
}

OnesComp16 onesCompAdd(OnesComp16 a, OnesComp16 b) {
    // Ripple Carry Adder
    u8 both_neg = AND(GET_BIT(a, 15), GET_BIT(b, 15));
    a = SWITCH(both_neg, onesCompNeg(a));
    b = SWITCH(both_neg, onesCompNeg(b));
    u8 c = 0;
    for (int i = 0; i < 16; i++) {
        u8 ab = GET_BIT(a, i);
        u8 bb = GET_BIT(b, i);
        a = SET_BIT(a, i, BXOR(ab, bb, c));
        c = OR(AND(ab, bb), AND(c, OR(ab, bb)));
    }
    // for (int i = 0; i < 16; i++) {
    //     u8 ab = GET_BIT(a, i);
    //     u8 bb = GET_BIT(b, i);
    //     a = SET_BIT(a, i, BXOR(ab, bb, c));
    //     c = OR(AND(ab, bb), AND(c, OR(ab, bb)));
    // }
    a = SWITCH(both_neg, onesCompNeg(a));
    return a;
}
OnesComp16 onesCompSub(OnesComp16 a, OnesComp16 b) {
    return onesCompAdd(a, onesCompNeg(b));
}
OnesComp16 onesCompMul(OnesComp16 a, OnesComp16 b) {
    return twosCompMul(a, b);
}
DivResult onesCompDiv(OnesComp16 a, OnesComp16 b) {
    DivResult res = twosCompDiv(a, b);
    return (DivResult) { .q = res.q, .r = res.r };
}

global IntFormat OnesComplement = {
    .name = "One's Complement",
    .fromInt  = &onesCompFromInt,
    .fromInt8 = &onesCompFromInt8,
    .toInt    = &onesCompToInt,
    .extend   = &onesCompExtend,
    .trunc    = &onesCompTrunc,
    .neg = &onesCompNeg,
    .add = &onesCompAdd,
    .sub = &onesCompSub,
    .mul = &onesCompMul,
    .div = &onesCompDiv,
};


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

bool test_conv(IntFormat fmt) {
    char buf[64];
    snprintf(buf, ail_arrlen(buf), "%s-Conversion", fmt.name);
    for (int i = 0; i <= 0xffff; i++) {
        i16 x = (i16)i;
        if (x != (i16)0x8000) {
            u16 val16 = fmt.fromInt(x);
            ASSERT(x == fmt.toInt(val16), buf, "Conversion: %d -> 0x%04x -> %d", x, val16, fmt.toInt(val16));
            if (intTrunc(x) != (i8)0x80) {
                u8 val8 = fmt.fromInt8(intTrunc(x));
                ASSERT(fmt.trunc(val16) == val8, buf,
                    "Truncation: %d -> %d (expected %d) | 0x%04x -> 0x%02x (expected 0x%02x)",
                    x, fmt.toInt(fmt.trunc(val16)), intTrunc(x), val16, fmt.trunc(val16), val8);
                ASSERT(fmt.extend(val8) == fmt.fromInt((i16)(intTrunc(x))), buf,
                    "Extension: %d -> %d (expected %d) | 0x%04x -> 0x%04x (expected 0x%04x)",
                    intTrunc(x), fmt.toInt(fmt.extend(val8)), (u16)((i16)(intTrunc(x))), val8, fmt.extend(val8), fmt.fromInt((i16)(intTrunc(x))));
            }
        }
    }
    printf("%s tested successfully\n", buf);
    return true;
}

bool test_neg(IntFormat fmt) {
    char buf[64];
    snprintf(buf, ail_arrlen(buf), "%s-Negation", fmt.name);
    for (int i = 0; i <= 0xffff; i++) {
        i16 x = (i16)i;
        u16 val = fmt.fromInt(x);
        ASSERT(val == fmt.neg(fmt.neg(val)), buf,
            "Negation: 0x%02x -> 0x%02x -> 0x%02x",
            val, fmt.neg(val), fmt.neg(fmt.neg(val)));
        if (x != 0 && x != (i16)0x8000) {
            ASSERT(fmt.fromInt(-x) == fmt.neg(val), buf, "%d -> 0x%02x != 0x%02x -> 0x%02x", -x, fmt.fromInt(-x), val, fmt.neg(val));
        }
    }
    printf("%s tested successfully\n", buf);
    return true;
}

bool test_add(IntFormat fmt) {
    char buf[64];
    snprintf(buf, ail_arrlen(buf), "%s-Addition", fmt.name);
    for (int i = -128; i <= 128; i++) {
        for (int j = -128; j <= 128; j++) {
            u16 a = fmt.fromInt(i);
            u16 b = fmt.fromInt(j);
            i16 expected = fmt.toInt(a) + fmt.toInt(b);
            u16 res = fmt.add(a, b);
            ASSERT(expected == fmt.toInt(res), buf, "%d + %d = %d (expected %d) | 0x%02x + 0x%02x = 0x%02x (expected 0x%02x)",
                fmt.toInt(a), fmt.toInt(b), fmt.toInt(res), expected, a, b, (u16)res, fmt.fromInt(expected));
        }
    }
    printf("%s tested successfully\n", buf);
    return true;
}

bool test_sub(IntFormat fmt) {
    char buf[64];
    snprintf(buf, ail_arrlen(buf), "%s-Subtraction", fmt.name);
    for (int i = -128; i <= 128; i++) {
        for (int j = -128; j <= 128; j++) {
            u16 a = fmt.fromInt(i);
            u16 b = fmt.fromInt(j);
            i16 expected = fmt.toInt(a) - fmt.toInt(b);
            u16 res = fmt.sub(a, b);
            ASSERT(expected == fmt.toInt(res), buf, "%d - %d = %d (expected %d) | 0x%02x - 0x%02x = 0x%02x (expected 0x%02x)",
                fmt.toInt(a), fmt.toInt(b), fmt.toInt(res), expected, a, b, (u16)res, fmt.fromInt(expected));
        }
    }
    printf("%s tested successfully\n", buf);
    return true;
}

bool test_mul(IntFormat fmt) {
    char buf[64];
    snprintf(buf, ail_arrlen(buf), "%s-Multiplication", fmt.name);
    for (int i = -128; i <= 128; i++) {
        for (int j = -128; j <= 128; j++) {
            u16 a = fmt.fromInt(i);
            u16 b = fmt.fromInt(j);
            i16 expected = fmt.toInt(a) * fmt.toInt(b);
            u16 res = fmt.mul(a, b);
            ASSERT(expected == fmt.toInt(res), buf, "%d * %d = %d (expected %d) | 0x%02x * 0x%02x = 0x%02x (expected 0x%02x)",
                fmt.toInt(a), fmt.toInt(b), fmt.toInt(res), expected, a, b, (u16)res, fmt.fromInt(expected));
        }
    }
    printf("%s tested successfully\n", buf);
    return true;
}

// @TODO: Add test (quot*b + rem == a)
bool test_div(IntFormat fmt) {
    char buf[64];
    snprintf(buf, ail_arrlen(buf), "%s-Division", fmt.name);
    for (int i = -128; i <= 128; i++) {
        for (int j = -128; j <= 128; j++) {
            u16 a = fmt.fromInt(i);
            u16 b = fmt.fromInt(j);
            if (!b) continue;
            i16 expectedQ = fmt.toInt(a) / fmt.toInt(b);
            i16 expectedR = fmt.toInt(a) % fmt.toInt(b);
            DivResult res = fmt.div(a, b);
            ASSERT(expectedQ == fmt.toInt(res.q), buf, "%d / %d = %d (expected %d) | 0x%02x / 0x%02x = 0x%02x (expected 0x%02x)",
                fmt.toInt(a), fmt.toInt(b), fmt.toInt(res.q), expectedQ, a, b, (u16)res.q, fmt.fromInt(expectedQ));
            ASSERT(expectedR == fmt.toInt(res.r), buf, "%d %% %d = %d (expected %d) | 0x%02x %% 0x%02x = 0x%02x (expected 0x%02x)",
                fmt.toInt(a), fmt.toInt(b), fmt.toInt(res.r), expectedR, a, b, (u16)res.r, fmt.fromInt(expectedR));
        }
    }
    printf("%s tested successfully\n", buf);
    return true;
}

bool test_all(IntFormat fmt) {
    bool res = true;
    res &= test_conv(fmt);
    res &= test_neg(fmt);
    res &= test_add(fmt);
    res &= test_sub(fmt);
    res &= test_mul(fmt);
    res &= test_div(fmt);
    return res;
}

int main(void) {
#ifdef BENCHMARK
    ail_bench_init();
    ail_bench_begin_profile();
    for (int i = 0; i < BENCHMARK_COUNT; i++)
#endif
    { // Tests
        test_all(TwosComplement);
        test_all(SignMagnitude);
        test_all(OnesComplement);
    }
#ifdef BENCHMARK
    printf("--------\n");
    printf("Profiling results:\n");
    ail_bench_end_and_print_profile(16, true);
#endif
    return 0;
}

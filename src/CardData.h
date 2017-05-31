#pragma once

#include <stdint.h>

// fuck you GCC and your fucking 'enum is too big for bitfield' warning
#pragma GCC system_header

#define COLORS_APPLY(MACRO, ...)                \
        MACRO(RED, __VA_ARGS__)                 \
        MACRO(GREEN, __VA_ARGS__)               \
        MACRO(BLUE, __VA_ARGS__)                \
        MACRO(YELLOW, __VA_ARGS__)

#define SPECIALS_APPLY(MACRO, ...)              \
        MACRO(SKIP, __VA_ARGS__)                \
        MACRO(REVERSE, __VA_ARGS__)             \
        MACRO(PLUS_2, __VA_ARGS__)

#define COLORLESS_APPLY(MACRO, ...)             \
        MACRO(COLOR_CHOOSE, __VA_ARGS__)        \
        MACRO(PLUS_4, __VA_ARGS__)

#define NUMBERS_APPLY(MACRO, ...)               \
        MACRO(ZERO, __VA_ARGS__)                \
        MACRO(ONE, __VA_ARGS__)                 \
        MACRO(TWO, __VA_ARGS__)                 \
        MACRO(THREE, __VA_ARGS__)               \
        MACRO(FOUR, __VA_ARGS__)                \
        MACRO(FIVE, __VA_ARGS__)                \
        MACRO(SIX, __VA_ARGS__)                 \
        MACRO(SEVEN, __VA_ARGS__)               \
        MACRO(EIGHT, __VA_ARGS__)               \
        MACRO(NINE, __VA_ARGS__)

#define ENUM_DECL(name, ...) name,

#define NUMBERS_DECL(color, number, ...) NumberData(color, number),
#define SPECIALS_DECL(color, type, ...) SpecialData(color, type),
#define COLORLESS_DECL(type, ...) ColorlessData(type),

#define FOR_VALUE(value, MACRO, ...) MACRO(__VA_ARGS__, value)

namespace Card {
    enum Category : uint8_t {
        NUMBER,
        SPECIAL,
        COLORLESS,
    };

    enum Color : uint8_t {
        COLORS_APPLY(ENUM_DECL)
    };

    enum SpecialType : uint8_t {
        SPECIALS_APPLY(ENUM_DECL)
    };

    enum ColorlessType : uint8_t {
        COLORLESS_APPLY(ENUM_DECL)
    };

    enum Number : uint8_t {
        NUMBERS_APPLY(ENUM_DECL)
    };

    struct _Empty{};

    struct NumberData {
        union {
            struct {
                Category category: 2; // must be NUMBER
                Color color: 2;
                Number number: 4;
            };
            _Empty __empty;
        };

        constexpr NumberData() : __empty() {}
        constexpr NumberData(Color color, Number number) : category(NUMBER), color(color), number(number) {}
    };

    struct SpecialData {
        union {
            struct {
                Category category: 2; // must be SPECIAL
                Color color: 2;
                SpecialType type: 2;
                uint8_t reserved: 2;
            };
            _Empty __empty;
        };

        constexpr SpecialData() : __empty() {}
        constexpr SpecialData(Color color, SpecialType type) : category(SPECIAL), color(color), type(type) {}
    };

    struct ColorlessData {
        union {
            struct {
                Category category: 2; // must be COLORLESS
                ColorlessType type: 1;
                uint8_t reserved: 5;
            };
            _Empty __empty;
        };

        constexpr ColorlessData() : __empty() {}
        explicit constexpr ColorlessData(ColorlessType type) : category(COLORLESS), type(type) {}
    };

    struct Data {
        union {
            struct {
                Category category: 2;
                Color color: 2; // not valid for COLORLESS
            };
            NumberData asNumber;
            SpecialData asSpecial;
            ColorlessData asColorless;
            uint8_t asByte;
            _Empty __empty;
        };

        bool isNumber() const { return category == NUMBER; }
        bool isSpecial() const { return category == SPECIAL; }
        bool isColorless() const { return category == COLORLESS; }

        constexpr Data() : __empty() {}
        constexpr Data(uint8_t byte) : asByte(byte) {}
        constexpr Data(NumberData number) : asNumber(number) {}
        constexpr Data(SpecialData special) : asSpecial(special) {}
        constexpr Data(ColorlessData colorless) : asColorless(colorless) {}
    };
    static_assert(sizeof(Data) == 1, "Data class should be exactly one byte. Lots of blood was spilled for it.");

    static const Data ALL_CARDS[] = {
            NUMBERS_APPLY(FOR_VALUE, COLORS_APPLY, NUMBERS_DECL)

            SPECIALS_APPLY(FOR_VALUE, COLORS_APPLY, SPECIALS_DECL)

            COLORLESS_APPLY(COLORLESS_DECL)
    };
}

#undef COLORS_APPLY
#undef SPECIALS_APPLY
#undef COLORLESS_APPLY
#undef NUMBERS_APPLY
#undef ENUM_DECL
#undef NUMBERS_DECL
#undef SPECIALS_DECL
#undef COLORLESS_DECL
#undef FOR_VALUE

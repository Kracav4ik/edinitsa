#pragma once

#include <stdint.h>

// fuck you GCC and your fucking 'enum is too big for bitfield' warning
#pragma GCC system_header

namespace Card {
    enum Category : uint8_t {
        NUMBER,
        SPECIAL,
        COLORLESS,
    };

    enum Color : uint8_t {
        RED,
        GREEN,
        BLUE,
        YELLOW,
    };

    enum SpecialType : uint8_t {
        SKIP,
        REVERSE,
        PLUS_2,
    };

    enum ColorlessType : uint8_t {
        COLOR_CHOOSE,
        PLUS_4,
    };

    enum Number : uint8_t {
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
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
            NumberData(RED, ZERO),
            SpecialData(RED, SKIP),
            ColorlessData(PLUS_4),
    };
}

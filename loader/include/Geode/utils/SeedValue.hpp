#pragma once

#include <array>

namespace geode {
    class BaseSeedValue {
    public:
        BaseSeedValue() = default;
        BaseSeedValue(BaseSeedValue const& other) = default;
        BaseSeedValue(BaseSeedValue&& other) = default;
        BaseSeedValue& operator=(BaseSeedValue const& other) = default;
        BaseSeedValue& operator=(BaseSeedValue&& other) = default;
    };

#define GEODE_SEED_VALUE_COMMON_OPS()                        \
    SeedValue& operator=(int value) {                        \
        internalValue() = value;                             \
        internalRandom() = value + internalSeed();           \
        return *this;                                        \
    }                                                        \
    int value() {                                            \
        internalValue() = internalRandom() - internalSeed(); \
        return internalValue();                              \
    }                                                        \
    operator int() { return this->value(); }

    class SeedValueSR : public BaseSeedValue {
    private:
        using SeedValue = SeedValueSR;
        std::array<int, 2> m_values;

        int& internalSeed() {
            return m_values[0];
        }

        int& internalRandom() {
            return m_values[1];
        }

    public:

        using BaseSeedValue::BaseSeedValue;

        SeedValueSR(int value, int seed) : m_values({seed, value + seed}) {}

        SeedValue& operator=(int value) {
            internalRandom() = value + internalSeed();
            return *this;
        }

        int value() {
            return internalRandom() - internalSeed();
        }

        operator int() {
            return this->value();
        }
    };

    class SeedValueRS : public BaseSeedValue {
    private:
        using SeedValue = SeedValueRS;
        std::array<int, 2> m_values;

        int& internalRandom() {
            return m_values[0];
        }

        int& internalSeed() {
            return m_values[1];
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueRS(int value, int seed) : m_values({value + seed, seed}) {}

        SeedValue& operator=(int value) {
            internalRandom() = value + internalSeed();
            return *this;
        }

        int value() {
            return internalRandom() - internalSeed();
        }

        operator int() {
            return this->value();
        }
    };

    class SeedValueVRS : public BaseSeedValue {
    private:
        using SeedValue = SeedValueVRS;
        std::array<int, 3> m_values;

        int& internalValue() {
            return m_values[0];
        }

        int& internalRandom() {
            return m_values[1];
        }

        int& internalSeed() {
            return m_values[2];
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueVRS(int value, int seed) : m_values({value, value + seed, seed}) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueVSR : public BaseSeedValue {
    private:
        using SeedValue = SeedValueVSR;
        std::array<int, 3> m_values;

        int& internalValue() {
            return m_values[0];
        }

        int& internalSeed() {
            return m_values[1];
        }

        int& internalRandom() {
            return m_values[2];
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueVSR(int value, int seed) : m_values({value, seed, value + seed}) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueRVS : public BaseSeedValue {
    private:
        using SeedValue = SeedValueRVS;
        std::array<int, 3> m_values;

        int& internalRandom() {
            return m_values[0];
        }

        int& internalValue() {
            return m_values[1];
        }

        int& internalSeed() {
            return m_values[2];
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueRVS(int value, int seed) : m_values({value + seed, value, seed}) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueRSV : public BaseSeedValue {
    private:
        using SeedValue = SeedValueRSV;
        std::array<int, 3> m_values;

        int& internalRandom() {
            return m_values[0];
        }

        int& internalSeed() {
            return m_values[1];
        }

        int& internalValue() {
            return m_values[2];
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueRSV(int value, int seed) : m_values({value + seed, seed, value}) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueSVR : public BaseSeedValue {
    private:
        using SeedValue = SeedValueSVR;
        std::array<int, 3> m_values;

        int& internalSeed() {
            return m_values[0];
        }

        int& internalValue() {
            return m_values[1];
        }

        int& internalRandom() {
            return m_values[2];
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueSVR(int value, int seed) : m_values({seed, value, value + seed}) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueSRV : public BaseSeedValue {
    private:
        using SeedValue = SeedValueSRV;
        std::array<int, 3> m_values;

        int& internalSeed() {
            return m_values[0];
        }

        int& internalRandom() {
            return m_values[1];
        }

        int& internalValue() {
            return m_values[2];
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueSRV(int value, int seed) : m_values({seed, value + seed, value}) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

#undef GEODE_SEED_VALUE_COMMON_OPS
} // namespace geode
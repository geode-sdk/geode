#pragma once

namespace geode {
    class BaseSeedValue {
    protected:
        int m_v1 = 0;
        int m_v2 = 0;
        int m_v3 = 0;

    public:
        BaseSeedValue(int v1, int v2, int v3) : m_v1(v1), m_v2(v2), m_v3(v3) {}

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

    class SeedValueVRS : public BaseSeedValue {
    private:
        using SeedValue = SeedValueVRS;
        int& internalValue() {
            return m_v1;
        }

        int& internalRandom() {
            return m_v2;
        }

        int& internalSeed() {
            return m_v3;
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueVRS(int value, int seed) : BaseSeedValue(value, value + seed, seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueVSR : public BaseSeedValue {
    private:
        using SeedValue = SeedValueVSR;
        int& internalValue() {
            return m_v1;
        }

        int& internalSeed() {
            return m_v2;
        }

        int& internalRandom() {
            return m_v3;
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueVSR(int value, int seed) : BaseSeedValue(value, seed, value + seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueRVS : public BaseSeedValue {
    private:
        using SeedValue = SeedValueRVS;
        int& internalRandom() {
            return m_v1;
        }

        int& internalValue() {
            return m_v2;
        }

        int& internalSeed() {
            return m_v3;
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueRVS(int value, int seed) : BaseSeedValue(value + seed, value, seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueRSV : public BaseSeedValue {
    private:
        using SeedValue = SeedValueRSV;
        int& internalRandom() {
            return m_v1;
        }

        int& internalSeed() {
            return m_v2;
        }

        int& internalValue() {
            return m_v3;
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueRSV(int value, int seed) : BaseSeedValue(value + seed, seed, value) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueSVR : public BaseSeedValue {
    private:
        using SeedValue = SeedValueSVR;
        int& internalSeed() {
            return m_v1;
        }

        int& internalValue() {
            return m_v2;
        }

        int& internalRandom() {
            return m_v3;
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueSVR(int value, int seed) : BaseSeedValue(seed, value, value + seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    class SeedValueSRV : public BaseSeedValue {
    private:
        using SeedValue = SeedValueSRV;
        int& internalSeed() {
            return m_v1;
        }

        int& internalRandom() {
            return m_v2;
        }

        int& internalValue() {
            return m_v3;
        }

    public:
        using BaseSeedValue::BaseSeedValue;

        SeedValueSRV(int value, int seed) : BaseSeedValue(seed, value + seed, value) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

#undef GEODE_SEED_VALUE_COMMON_OPS
} // namespace geode
#pragma once

namespace geode {
    enum class SeedOrdering {
        VRS,
        VSR,
        RVS,
        RSV,
        SVR,
        SRV,
    };

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

    template <SeedOrdering Ordering = SeedOrdering::RSV>
    class SeedValue {};

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

    template <>
    class SeedValue<SeedOrdering::VRS> : public BaseSeedValue {
    private:
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

        SeedValue(int value, int seed) : BaseSeedValue(value, value + seed, seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    template <>
    class SeedValue<SeedOrdering::VSR> : public BaseSeedValue {
    private:
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

        SeedValue(int value, int seed) : BaseSeedValue(value, seed, value + seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    template <>
    class SeedValue<SeedOrdering::RVS> : public BaseSeedValue {
    private:
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

        SeedValue(int value, int seed) : BaseSeedValue(value + seed, value, seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    template <>
    class SeedValue<SeedOrdering::RSV> : public BaseSeedValue {
    private:
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

        SeedValue(int value, int seed) : BaseSeedValue(value + seed, seed, value) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    template <>
    class SeedValue<SeedOrdering::SVR> : public BaseSeedValue {
    private:
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

        SeedValue(int value, int seed) : BaseSeedValue(seed, value, value + seed) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    template <>
    class SeedValue<SeedOrdering::SRV> : public BaseSeedValue {
    private:
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

        SeedValue(int value, int seed) : BaseSeedValue(seed, value + seed, value) {}

        GEODE_SEED_VALUE_COMMON_OPS();
    };

    using SeedValueRSV = SeedValue<SeedOrdering::RSV>;
    using SeedValueSRV = SeedValue<SeedOrdering::SRV>;
    using SeedValueVRS = SeedValue<SeedOrdering::VRS>;
    using SeedValueVSR = SeedValue<SeedOrdering::VSR>;
    using SeedValueRVS = SeedValue<SeedOrdering::RVS>;
    using SeedValueSVR = SeedValue<SeedOrdering::SVR>;

#undef GEODE_SEED_VALUE_COMMON_OPS
} // namespace geode
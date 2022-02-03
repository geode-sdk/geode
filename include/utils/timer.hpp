#pragma once

#include <chrono>

namespace geode::utils {
    template <typename T>
    struct is_chrono_duration {
        static constexpr bool value = false;
    };

    template <typename Rep, typename Period>
    struct is_chrono_duration<std::chrono::duration<Rep, Period>> {
        static constexpr bool value = true;
    };

    template<class Clock = std::chrono::high_resolution_clock>
    class Timer {
    public:
        using clock_point = std::chrono::time_point<Clock>;

    private:
        clock_point m_start;

    public:
        Timer() {
            this->m_start = Clock::now();
        }

        clock_point time() const { return m_start; }

        template<typename Duration = std::chrono::milliseconds>
        int64_t time() const {
            static_assert(is_chrono_duration<Duration>::value, "Duration must be a std::chrono::duration");
            auto end = Clock::now();
            return std::chrono::duration_cast<Duration>(end - m_start).count();
        }

        template<typename Duration = std::chrono::milliseconds>
        std::string timeToString() const {
            static_assert(is_chrono_duration<Duration>::value, "Duration must be a std::chrono::duration");
            if constexpr (std::is_same<Duration, std::chrono::milliseconds>::value) {
                return std::to_string(this->time<Duration>()) + "ms";
            }
            if constexpr (std::is_same<Duration, std::chrono::microseconds>::value) {
                return std::to_string(this->time<Duration>()) + u8"\u00b5s";
            }
        }
    };
}

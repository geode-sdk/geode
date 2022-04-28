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
            m_start = Clock::now();
        }

        void reset() {
            m_start = Clock::now();
        }

        clock_point time() const { return m_start; }

        template<typename Duration = std::chrono::milliseconds>
        int64_t elapsed() const {
            static_assert(is_chrono_duration<Duration>::value, "Duration must be a std::chrono::duration");
            auto end = Clock::now();
            return std::chrono::duration_cast<Duration>(end - m_start).count();
        }

        template<typename Duration = std::chrono::milliseconds>
        std::string elapsedAsString() const {
            static_assert(is_chrono_duration<Duration>::value, "Duration must be a std::chrono::duration");
            if constexpr (std::is_same<Duration, std::chrono::milliseconds>::value) {
                return std::to_string(this->elapsed<Duration>()) + "ms";
            }
            else if constexpr (std::is_same<Duration, std::chrono::microseconds>::value) {
                return std::to_string(this->elapsed<Duration>()) + "us";
            }
            else if constexpr (std::is_same<Duration, std::chrono::nanoseconds>::value) {
                return std::to_string(this->elapsed<Duration>()) + "ns";
            }
            else {
                // static_assert(!std::is_same_v<bool, bool>, "Unsupported duration type");
            }
        }
    };

    template<
        typename Duration = std::chrono::milliseconds,
        class Clock = std::chrono::high_resolution_clock
    >
    struct LogPerformance {
        std::ostream& m_output;
        std::string m_msg;
        Timer<Clock> m_timer;

        LogPerformance(
            std::string const& msg = "",
            std::ostream& out = std::cout
        ) : m_msg(msg), m_output(out) {
            m_timer = Timer<Clock>();
        };
        ~LogPerformance() {
            m_output << "Running \"" << m_msg << "\" took " << m_timer.template elapsedAsString<Duration>() << std::endl;
        }
    };
}

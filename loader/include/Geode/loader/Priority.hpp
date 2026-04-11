#pragma once

namespace geode {
    class Priority {
    public:
        /// @brief First priority, used for running hooks before all others
        /// @note Should be used with caution, consider using VeryEarly instead
        static inline constexpr int First = -3000;

        /// @brief Very early priority, used for running hooks very early
        /// @note Recommended over First
        static inline constexpr int VeryEarly = -2000;

        /// @brief Early priority, used for running hooks early
        static inline constexpr int Early = -1000;

        /// @brief Normal priority, used for running hooks at the normal time
        static inline constexpr int Normal = 0;

        /// @brief Late priority, used for running hooks late
        static inline constexpr int Late = 1000;

        /// @brief Very late priority, used for running hooks very late
        /// @note Recommended over Last
        static inline constexpr int VeryLate = 2000;

        /// @brief Last priority, used for running hooks after all others
        /// @note Should be used with caution, consider using VeryLate instead
        static inline constexpr int Last = 3000;

        /// @brief First pre priority, used for running hooks before all others
        /// @note Should be used with caution, consider using VeryEarlyPre instead
        static inline constexpr int FirstPre = First;

        /// @brief Very early pre priority, used for running hooks very early
        /// @note Recommended over FirstPre
        static inline constexpr int VeryEarlyPre = VeryEarly;

        /// @brief Early pre priority, used for running hooks early
        static inline constexpr int EarlyPre = Early;

        /// @brief Normal pre priority, used for running hooks at the normal time
        static inline constexpr int NormalPre = Normal;

        /// @brief Late pre priority, used for running hooks late
        static inline constexpr int LatePre = Late;

        /// @brief Very late pre priority, used for running hooks very late
        /// @note Recommended over LastPre
        static inline constexpr int VeryLatePre = VeryLate;

        /// @brief Last pre priority, used for running hooks after all others
        /// @note Should be used with caution, consider using VeryLatePre instead
        static inline constexpr int LastPre = Last;

        /// @brief First post priority, used for running hooks before all others
        /// @note Should be used with caution, consider using VeryEarlyPost instead
        static inline constexpr int FirstPost = Last;

        /// @brief Very early post priority, used for running hooks very early
        /// @note Recommended over FirstPost
        static inline constexpr int VeryEarlyPost = VeryLate;

        /// @brief Early post priority, used for running hooks early
        static inline constexpr int EarlyPost = Late;

        /// @brief Normal post priority, used for running hooks at the normal time
        static inline constexpr int NormalPost = Normal;

        /// @brief Late post priority, used for running hooks late
        static inline constexpr int LatePost = Early;

        /// @brief Very late post priority, used for running hooks very late
        /// @note Recommended over LastPost
        static inline constexpr int VeryLatePost = VeryEarly;

        /// @brief Last post priority, used for running hooks after all others
        /// @note Should be used with caution, consider using VeryLatePost instead
        static inline constexpr int LastPost = First;

        /// @brief Stub priority, used for stubbing out functions & editing parameters
        /// @note Should be used with extreme caution, may cause mod incompatibilities
        static inline constexpr int Stub = -4000;

        /// @brief Replace priority, used for replacing original functions
        /// @note Should be used with extreme caution, may cause mod incompatibilities
        static inline constexpr int Replace = 4000;
    };
}

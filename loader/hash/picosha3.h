#ifndef PICOSHA3_H
#define PICOSHA3_H

#include <array>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace picosha3 {
    constexpr size_t bits_to_bytes(size_t bits) { return bits / 8; };
    constexpr static size_t b_bytes = bits_to_bytes(1600);
    constexpr static uint64_t RC[24] = {
      0x0000000000000001ull, 0x0000000000008082ull, 0x800000000000808Aull,
      0x8000000080008000ull, 0x000000000000808Bull, 0x0000000080000001ull,
      0x8000000080008081ull, 0x8000000000008009ull, 0x000000000000008Aull,
      0x0000000000000088ull, 0x0000000080008009ull, 0x000000008000000Aull,
      0x000000008000808Bull, 0x800000000000008Bull, 0x8000000000008089ull,
      0x8000000000008003ull, 0x8000000000008002ull, 0x8000000000000080ull,
      0x000000000000800Aull, 0x800000008000000Aull, 0x8000000080008081ull,
      0x8000000000008080ull, 0x0000000080000001ull, 0x8000000080008008ull};

    using byte_t = uint8_t;
    using state_t = std::array<std::array<uint64_t, 5>, 5>;

    inline void theta(state_t& A) {
        uint64_t C[5] = {0, 0, 0, 0, 0};
        for(size_t x = 0; x < 5; ++x) {
            C[x] = A[x][0] ^ A[x][1] ^ A[x][2] ^ A[x][3] ^ A[x][4];
        };
        uint64_t D[5] = {0, 0, 0, 0, 0};
        D[0] = C[4] ^ (C[1] << 1 | C[1] >> (64 - 1));
        D[1] = C[0] ^ (C[2] << 1 | C[2] >> (64 - 1));
        D[2] = C[1] ^ (C[3] << 1 | C[3] >> (64 - 1));
        D[3] = C[2] ^ (C[4] << 1 | C[4] >> (64 - 1));
        D[4] = C[3] ^ (C[0] << 1 | C[0] >> (64 - 1));
        for(size_t x = 0; x < 5; ++x) {
            for(size_t y = 0; y < 5; ++y) {
                A[x][y] ^= D[x];
            }
        }
    };

    inline void rho(state_t& A) {
        size_t x{1};
        size_t y{0};
        for(size_t t = 0; t < 24; ++t) {
            size_t offset = ((t + 1) * (t + 2) / 2) % 64;
            A[x][y] = (A[x][y] << offset) | (A[x][y] >> (64 - offset));
            size_t tmp{y};
            y = (2 * x + 3 * y) % 5;
            x = tmp;
        };
    };

    inline void pi(state_t& A) {
        state_t tmp{A};
        for(size_t x = 0; x < 5; ++x) {
            for(size_t y = 0; y < 5; ++y) {
                A[x][y] = tmp[(x + 3 * y) % 5][x];
            }
        }
    };

    inline void chi(state_t& A) {
        state_t tmp{A};
        for(size_t x = 0; x < 5; ++x) {
            for(size_t y = 0; y < 5; ++y) {
                A[x][y] =
                  tmp[x][y] ^ (~(tmp[(x + 1) % 5][y]) & tmp[(x + 2) % 5][y]);
            }
        }
    };

    inline void iota(state_t& A, size_t round_index) {
        A[0][0] ^= RC[round_index];
    };

    inline void keccak_p(state_t& A) {
        for(size_t round_index = 0; round_index < 24; ++round_index) {
            theta(A);
            rho(A);
            pi(A);
            chi(A);
            iota(A, round_index);
        }
    };

    namespace {
        inline void next(size_t& x, size_t& y, size_t& i) {
            if(++i != 8) {
                return;
            }
            i = 0;
            if(++x != 5) {
                return;
            }
            x = 0;
            if(++y != 5) {
                return;
            }
        }
    } // namespace

    template <typename InIter>
    void absorb(InIter first, InIter last, state_t& A) {
        size_t x = 0;
        size_t y = 0;
        size_t i = 0;
        for(; first != last && y < 5; ++first) {
            auto tmp = static_cast<uint64_t>(*first);
            A[x][y] ^= (tmp << (i * 8));
            next(x, y, i);
        };
    }

    template <typename InContainer>
    void absorb(const InContainer& src, state_t& A) {
        absorb(src.cbegin(), src.cend(), A);
    };

    template <typename OutIter>
    OutIter squeeze(const state_t& A, OutIter first, OutIter last,
                    size_t rate_bytes) {
        size_t x = 0;
        size_t y = 0;
        size_t i = 0;
        for(size_t read_bytes = 0;
            first != last && y < 5 && read_bytes < rate_bytes;
            ++read_bytes, ++first) {
            auto tmp = static_cast<uint64_t>(A[x][y]);
            auto p = reinterpret_cast<byte_t*>(&tmp);
            *first = *(p + i);
            next(x, y, i);
        }
        return first;
    };

    template <typename OutContainer>
    typename OutContainer::iterator
    squeeze(const state_t& A, OutContainer& dest, size_t rate_bytes) {
        return squeeze(A, dest.begin(), dest.end(), rate_bytes);
    }

    enum class PaddingType {
        SHA,
        SHAKE,
    };

    template <typename InIter>
    std::string bytes_to_hex_string(InIter first, InIter last) {
        std::stringstream ss;
        ss << std::hex;
        for(; first != last; ++first) {
            ss << std::setw(2) << std::setfill('0')
               << static_cast<uint64_t>(*first);
        }
        return ss.str();
    }

    template <typename InContainer>
    std::string bytes_to_hex_string(const InContainer& src) {
        return bytes_to_hex_string(src.cbegin(), src.cend());
    }

    template <size_t rate_bytes, size_t d_bytes, PaddingType padding_type>
    class HashGenerator {
    public:
        HashGenerator()
          : buffer_{}, buffer_pos_{buffer_.begin()}, A_{}, hash_{},
            is_finished_{false} {}

        void clear() {
            clear_state();
            clear_buffer();
            is_finished_ = false;
        }

        template <typename InIter>
        void process(InIter first, InIter last) {
            static_assert(
              sizeof(typename std::iterator_traits<InIter>::value_type) == 1,
              "The size of input iterator value_type must be one byte.");

            for(; first != last; ++first) {
                *buffer_pos_ = *first;
                if(++buffer_pos_ == buffer_.end()) {
                    absorb(buffer_, A_);
                    keccak_p(A_);
                    clear_buffer();
                }
            }
        };

        void finish() {
            add_padding();
            absorb(buffer_, A_);
            keccak_p(A_);
            squeeze_();
            is_finished_ = true;
        };

        template <typename OutIter>
        void get_hash_bytes(OutIter first, OutIter last) {
            if(!is_finished_) {
                throw std::runtime_error("Not finished!");
            }
            std::copy(hash_.cbegin(), hash_.cend(), first);
        };

        template <typename OutCotainer>
        void get_hash_bytes(OutCotainer& dest) {
            get_hash_bytes(dest.begin(), dest.end());
        };

        template <typename InIter, typename OutIter>
        void operator()(InIter in_first, InIter in_last, OutIter out_first,
                        OutIter out_last) {
            static_assert(
              sizeof(typename std::iterator_traits<InIter>::value_type) == 1,
              "The size of input iterator value_type must be one byte.");
            static_assert(
              sizeof(typename std::iterator_traits<OutIter>::value_type) == 1,
              "The size of output iterator value_type must be one byte.");
            process(in_first, in_last);
            finish();
            std::copy(hash_.cbegin(), hash_.cend(), out_first);
            clear();
        };

        template <typename InIter, typename OutCotainer>
        void operator()(InIter in_first, InIter in_last, OutCotainer& dest) {
            operator()(in_first, in_last, dest.begin(), dest.end());
        };

        template <typename InContainer, typename OutIter>
        void operator()(const InContainer& src, OutIter out_first,
                        OutIter out_last) {
            operator()(src.cbegin(), src.cend(), out_first, out_last);
        };

        template <typename InContainer, typename OutContainer>
        void operator()(const InContainer& src, OutContainer& dest) {
            operator()(src.cbegin(), src.cend(), dest.begin(), dest.end());
        };

        template <typename OutIter>
        void operator()(std::ifstream& ifs, OutIter out_first,
                        OutIter out_last) {
            auto in_first = std::istreambuf_iterator<char>(ifs);
            auto in_last = std::istreambuf_iterator<char>();
            operator()(in_first, in_last, out_first, out_last);
        };

        template <typename OutCotainer>
        void operator()(std::ifstream& ifs, OutCotainer& dest) {
            operator()(ifs, dest.begin(), dest.end());
        };

        std::string get_hex_string() {
            if(!is_finished_) {
                throw std::runtime_error("Not finished!");
            }
            return bytes_to_hex_string(hash_);
        };

        template <typename InIter>
        std::string get_hex_string(InIter in_first, InIter in_last) {
            process(in_first, in_last);
            finish();
            auto hash = get_hex_string();
            clear();
            return hash;
        };

        template <typename InContainer>
        std::string get_hex_string(const InContainer& src) {
            return get_hex_string(src.cbegin(), src.cend());
        };

        std::string get_hex_string(std::ifstream& ifs) {
            auto in_first = std::istreambuf_iterator<char>(ifs);
            auto in_last = std::istreambuf_iterator<char>();
            return get_hex_string(in_first, in_last);
        };

    private:
        void clear_buffer() {
            buffer_.fill(0);
            buffer_pos_ = buffer_.begin();
        };

        void clear_state() {
            for(auto& row : A_) {
                row.fill(0);
            }
        };

        void add_padding() {
            const auto q =
              buffer_.size() - std::distance(buffer_pos_, buffer_.begin());

            if(padding_type == PaddingType::SHA) {
                if(q == 1) {
                    *buffer_pos_ = 0x86;
                } else {
                    *buffer_pos_ = 0x06;
                    buffer_.back() = 0x80;
                }
            } else if(padding_type == PaddingType::SHAKE) {
                if(q == 1) {
                    *buffer_pos_ = 0x9F;
                } else {
                    *buffer_pos_ = 0x1F;
                    buffer_.back() = 0x80;
                }
            }
        };

        void squeeze_() {
            auto first = hash_.begin();
            auto last = hash_.end();
            first = squeeze(A_, first, last, rate_bytes);
            while(first != last) {
                keccak_p(A_);
                first = squeeze(A_, first, last, rate_bytes);
            }
        };

        std::array<byte_t, rate_bytes> buffer_;
        typename decltype(buffer_)::iterator buffer_pos_;
        state_t A_;
        std::array<byte_t, d_bytes> hash_;
        bool is_finished_;
    };

    template <size_t d_bits>
    auto get_sha3_generator() {
        static_assert(
          d_bits == 224 or d_bits == 256 or d_bits == 384 or d_bits == 512,
          "SHA3 only accepts digest message length 224, 256 384 or 512 bits.");
        constexpr auto d_bytes = bits_to_bytes(d_bits);
        constexpr auto capacity_bytes = d_bytes * 2;
        constexpr auto rate_bytes = b_bytes - capacity_bytes;
        return HashGenerator<rate_bytes, d_bytes, PaddingType::SHA>{};
    }

    template <size_t strength_bits, size_t d_bits>
    auto get_shake_generator() {
        static_assert(strength_bits == 128 or strength_bits == 256,
                      "SHAKE only accepts strength 128 or 256 bits.");
        constexpr auto strength_bytes = bits_to_bytes(strength_bits);
        constexpr auto capacity_bytes = strength_bytes * 2;
        constexpr auto rate_bytes = b_bytes - capacity_bytes;
        constexpr auto d_bytes = bits_to_bytes(d_bits);
        return HashGenerator<rate_bytes, d_bytes, PaddingType::SHAKE>{};
    }

} // namespace picosha3

#endif

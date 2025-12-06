#pragma once

#include <cstdint>
#include <optional>
#include <random>
#include <vector>

class FixedSet {
public:
    FixedSet() = default;

    void Initialize(const std::vector<int>& numbers);
    bool Contains(int number) const;

private:
    static constexpr int64_t kMersennePrime = 2147483647;

    static std::mt19937& GetRandomGenerator();

    struct UniversalHash {
        int64_t a_coeff = 0;
        int64_t b_coeff = 0;
        size_t table_size = 0;

        UniversalHash() = default;

        void Reinitialize(size_t new_table_size);
        size_t operator()(int number) const;
    };

    struct Bucket {
        UniversalHash hash;
        std::vector<std::optional<int>> table;

        void Initialize(const std::vector<int>& numbers);
        bool Contains(int number) const;
    };

    UniversalHash primary_hash_;
    std::vector<Bucket> buckets_;
};

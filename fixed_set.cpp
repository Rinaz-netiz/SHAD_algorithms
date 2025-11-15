#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

class FixedSet {
public:
    FixedSet() = default;

    void Initialize(const std::vector<int>& numbers) {
        const size_t kNumbersCount = numbers.size();
        if (kNumbersCount == 0) {
            secondary_tables_.clear();
            return;
        }

        std::vector<std::vector<int>> buckets;
        bool primary_hash_found = false;
        while (!primary_hash_found) {
            primary_hash_.Reinitialize(kNumbersCount);
            buckets.assign(kNumbersCount, std::vector<int>());
            for (const int kNum : numbers) {
                buckets[primary_hash_(kNum)].push_back(kNum);
            }

            int64_t sum_of_squares = 0;
            for (const auto& bucket : buckets) {
                sum_of_squares +=
                    static_cast<int64_t>(bucket.size()) * bucket.size();
            }

            if (sum_of_squares <= 4 * static_cast<int64_t>(kNumbersCount)) {
                primary_hash_found = true;
            }
        }

        secondary_tables_.assign(kNumbersCount, SecondaryTable());
        for (size_t i = 0; i < kNumbersCount; ++i) {
            if (!buckets[i].empty()) {
                secondary_tables_[i].Initialize(buckets[i]);
            }
        }
    }

    bool Contains(int number) const {
        if (secondary_tables_.empty()) {
            return false;
        }
        size_t primary_idx = primary_hash_(number);
        return secondary_tables_[primary_idx].Contains(number);
    }

private:
    static constexpr int64_t kMersennePrime = 2147483647;
    static constexpr int kEmptySentinel = 2000000000;

    static std::mt19937& GetRandomGenerator() {
        static std::mt19937 generator(std::chrono::high_resolution_clock::now()
                                          .time_since_epoch()
                                          .count());
        return generator;
    }

    struct UniversalHash {
        int64_t a_number = 0, b_number = 0;
        size_t table_size = 0;

        UniversalHash() = default;

        void Reinitialize(size_t new_table_size) {
            table_size = new_table_size;
            if (table_size == 0) {
                return;
            }

            std::uniform_int_distribution<int64_t> dist_a(1,
                                                          kMersennePrime - 1);
            std::uniform_int_distribution<int64_t> dist_b(0,
                                                          kMersennePrime - 1);

            a_number = dist_a(GetRandomGenerator());
            b_number = dist_b(GetRandomGenerator());
        }

        size_t operator()(int number) const {
            if (table_size == 0) {
                return 0;
            }
            int64_t hash_val = (a_number * number + b_number) % kMersennePrime;
            if (hash_val < 0) {
                hash_val += kMersennePrime;
            }
            return hash_val % table_size;
        }
    };

    class SecondaryTable {
    public:
        SecondaryTable() = default;

        void Initialize(const std::vector<int>& numbers) {
            const size_t kBucketSize = numbers.size();
            if (kBucketSize == 0) {
                table_.clear();
                return;
            }

            const size_t kTableSize = kBucketSize * kBucketSize;
            if (kTableSize == 0) {
                table_.clear();
                return;
            }

            bool collision_found = true;
            while (collision_found) {
                collision_found = false;
                hash_.Reinitialize(kTableSize);
                table_.assign(kTableSize, kEmptySentinel);

                for (const int kNum : numbers) {
                    size_t idx = hash_(kNum);
                    if (table_[idx] != kEmptySentinel) {
                        collision_found = true;
                        break;
                    }
                    table_[idx] = kNum;
                }
            }
        }

        bool Contains(int number) const {
            if (table_.empty()) {
                return false;
            }
            size_t idx = hash_(number);
            return table_[idx] == number;
        }

    private:
        UniversalHash hash_;
        std::vector<int> table_;
    };

    UniversalHash primary_hash_;
    std::vector<SecondaryTable> secondary_tables_;
};

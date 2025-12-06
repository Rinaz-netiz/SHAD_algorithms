#include "fixed_set.h"

#include <chrono>

std::mt19937& FixedSet::GetRandomGenerator() {
    static std::mt19937 generator(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    return generator;
}

void FixedSet::UniversalHash::Reinitialize(size_t new_table_size) {
    table_size = new_table_size;
    if (table_size == 0) {
        return;
    }

    std::uniform_int_distribution<int64_t> dist_a(1, kMersennePrime - 1);
    std::uniform_int_distribution<int64_t> dist_b(0, kMersennePrime - 1);

    a_coeff = dist_a(GetRandomGenerator());
    b_coeff = dist_b(GetRandomGenerator());
}

size_t FixedSet::UniversalHash::operator()(int number) const {
    if (table_size == 0) {
        return 0;
    }
    int64_t hash_val = (a_coeff * number + b_coeff) % kMersennePrime;
    if (hash_val < 0) {
        hash_val += kMersennePrime;
    }
    return hash_val % table_size;
}

void FixedSet::Bucket::Initialize(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        table.clear();
        return;
    }

    size_t bucket_size = numbers.size();
    size_t table_size = bucket_size * bucket_size;

    bool collision_found = true;
    while (collision_found) {
        collision_found = false;
        hash.Reinitialize(table_size);
        table.assign(table_size, std::nullopt);

        for (int num : numbers) {
            size_t idx = hash(num);
            if (table[idx].has_value()) {
                collision_found = true;
                break;
            }
            table[idx] = num;
        }
    }
}

bool FixedSet::Bucket::Contains(int number) const {
    if (table.empty()) {
        return false;
    }
    size_t idx = hash(number);
    return table[idx].has_value() && table[idx].value() == number;
}

void FixedSet::Initialize(const std::vector<int>& numbers) {
    size_t numbers_count = numbers.size();
    if (numbers_count == 0) {
        buckets_.clear();
        return;
    }

    std::vector<std::vector<int>> primary_buckets;
    bool primary_hash_found = false;
    
    while (!primary_hash_found) {
        primary_hash_.Reinitialize(numbers_count);
        primary_buckets.assign(numbers_count, std::vector<int>());
        
        for (int num : numbers) {
            primary_buckets[primary_hash_(num)].push_back(num);
        }

        int64_t sum_of_squares = 0;
        for (const auto& bucket : primary_buckets) {
            int64_t size = static_cast<int64_t>(bucket.size());
            sum_of_squares += size * size;
        }

        if (sum_of_squares <= 4 * static_cast<int64_t>(numbers_count)) {
            primary_hash_found = true;
        }
    }

    buckets_.assign(numbers_count, Bucket());
    for (size_t i = 0; i < numbers_count; ++i) {
        if (!primary_buckets[i].empty()) {
            buckets_[i].Initialize(primary_buckets[i]);
        }
    }
}

bool FixedSet::Contains(int number) const {
    if (buckets_.empty()) {
        return false;
    }
    size_t primary_idx = primary_hash_(number);
    return buckets_[primary_idx].Contains(number);
}

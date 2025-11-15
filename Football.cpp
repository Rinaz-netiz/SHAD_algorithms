#include <iostream>
#include <vector>
#include <algorithm>

struct Person {
    int64_t value;
    size_t index;
};

struct PointersToRangeAnswerAndSum {
    int64_t sum;
    int64_t left_answer;
    int64_t right_answer;
};

std::vector<Person> ReadInput() {
    int64_t number_of_person;
    std::cin >> number_of_person;

    std::vector<Person> array_of_person(number_of_person);
    for (int64_t i = 0; i < number_of_person; ++i) {
        std::cin >> array_of_person[i].value;
        array_of_person[i].index = i;
    }

    return array_of_person;
}


PointersToRangeAnswerAndSum AlgorithmImplementation(std::vector<Person>& array_of_person) {
    if (array_of_person.size() == 1) {
        return  PointersToRangeAnswerAndSum{.sum = array_of_person[0].value, .left_answer = 0, .right_answer = 0};
    }

    if (array_of_person.size() == 2) {
        return  PointersToRangeAnswerAndSum{
            .sum = array_of_person[0].value + array_of_person[1].value, .left_answer = 0, .right_answer = 1};
    }

    std::sort(array_of_person.begin(), array_of_person.end(), [](const Person& a, const Person& b) {
        return a.value < b.value;
    });

    int64_t answer = array_of_person[0].value + array_of_person[1].value;
    int64_t sum = answer;
    int64_t left_answer = 0, right_answer = 1;
    int64_t left = 0;
    int64_t number_of_person = static_cast<int64_t>(array_of_person.size());

    for (int64_t right = 2; right < number_of_person; ++right) {
        sum += array_of_person[right].value;

        while (left + 1 < right &&
               array_of_person[left].value + array_of_person[left + 1].value < array_of_person[right].value) {
            sum -= array_of_person[left].value;
            ++left;
        }

        if (sum > answer) {
            left_answer = left;
            right_answer = right;
            answer = sum;
        }
    }

    return PointersToRangeAnswerAndSum{.sum = answer, .left_answer = left_answer, .right_answer = right_answer};
}


void PrintAnswer(const PointersToRangeAnswerAndSum& ptr_to_answers, const std::vector<Person>& array_of_person) {
    std::cout << ptr_to_answers.sum << "\n";

    std::vector<int64_t> result_array;
    for (int64_t i = ptr_to_answers.left_answer; i <= ptr_to_answers.right_answer; ++i)
        result_array.push_back(array_of_person[i].index);

    std::sort(result_array.begin(), result_array.end());

    for (const auto& index : result_array) {
        std::cout << index + 1 << " ";
    }
}



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    
    std::vector<Person> array_of_person = ReadInput();
    PointersToRangeAnswerAndSum ptr_to_answers = AlgorithmImplementation(array_of_person);
    PrintAnswer(ptr_to_answers, array_of_person);

    return 0;
}
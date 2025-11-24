#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

struct Person {
    int64_t value;
    size_t index;
};

struct PointersToRangeAnswerAndSum {
    int64_t sum;
    size_t left_answer;
    size_t right_answer;
};

std::vector<Person> ReadInput(std::istream& in) {
    size_t number_of_person;
    in >> number_of_person;

    std::vector<Person> array_of_person(number_of_person);
    for (size_t i = 0; i < number_of_person; ++i) {
        in >> array_of_person[i].value;
        array_of_person[i].index = i;
    }

    return array_of_person;
}

PointersToRangeAnswerAndSum AlgorithmImplementation(std::vector<Person>& array_of_person) {
    const size_t number_of_person = array_of_person.size();

    if (number_of_person == 1) {
        return PointersToRangeAnswerAndSum{.sum = array_of_person[0].value, .left_answer = 0, .right_answer = 0};
    }

    std::sort(array_of_person.begin(), array_of_person.end(), [](const Person& a, const Person& b) {
        return a.value < b.value;
    });

    if (number_of_person == 2) {
        return PointersToRangeAnswerAndSum{
            .sum = array_of_person[0].value + array_of_person[1].value, .left_answer = 0, .right_answer = 1};
    }

    int64_t answer = array_of_person[0].value + array_of_person[1].value;
    int64_t sum = answer;
    size_t left_answer = 0, right_answer = 1;
    size_t left = 0;

    for (size_t right = 2; right < number_of_person; ++right) {
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


void PrintAnswer(const PointersToRangeAnswerAndSum& ptr_to_answers,
                 const std::vector<Person>& array_of_person,
                 std::ostream& out) {

    out << ptr_to_answers.sum << "\n";

    std::vector<size_t> result_array;
    result_array.reserve(ptr_to_answers.right_answer - ptr_to_answers.left_answer + 1);

    for (size_t i = ptr_to_answers.left_answer; i <= ptr_to_answers.right_answer; ++i) {
        result_array.push_back(array_of_person[i].index);
    }

    std::sort(result_array.begin(), result_array.end());

    for (size_t i = 0; i < result_array.size(); ++i) {
        out << result_array[i] + 1 << (i == result_array.size() - 1 ? "" : " ");
    }
    out << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::vector<Person> array_of_person = ReadInput(std::cin);

    PointersToRangeAnswerAndSum ptr_to_answers = AlgorithmImplementation(array_of_person);

    PrintAnswer(ptr_to_answers, array_of_person, std::cout);

    return 0;
}
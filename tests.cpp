#include <gtest/gtest.h>
#include <queue>
#include <utility>
#include "priorityqueue.h"

string randomString(size_t max_length) {
    string result;
    int len = rand() % max_length + 1;
    for (int i = 0; i < len; i++) {
        result += (char) ('a' + rand() % 26);
    }
    return result;
}

template<typename T>
class Wrapper {
public:
    int priority, order;
    T value;

    Wrapper(T value, int priority, int order) : priority(priority), order(order), value(std::move(value)) {}

    friend bool operator<(const Wrapper &a, const Wrapper &b) {
        if (a.priority > b.priority) {
            return true;
        }
        if (a.priority < b.priority) {
            return false;
        }
        return a.order > b.order;
    }
};


template<typename T>
string toString(priority_queue<Wrapper<T>> pq) {
    stringstream result;
    while (!pq.empty()) {
        auto w = pq.top();
        pq.pop();
        result << w.priority << " value: " << w.value << endl;
    }
    return result.str();
}

template<typename T>
string toString(priorityqueue<T> &pq) {
    stringstream result;
    pq.begin();
    T value;
    int priority;
    while (pq.next(value, priority)) {
        result << priority << " value: " << value << endl;
    }
    return result.str();
}

template<class T>
void compare(priorityqueue<T> &pq, priority_queue<Wrapper<T>> &pq_bltn) {
    ASSERT_EQ(pq.Size(), pq_bltn.size());
    ASSERT_EQ(pq.toString(), toString(pq_bltn));
}

TEST(priorityqueue, TestInt) {
    // Needed to reproduce random values on error
    srand(42);

    // Don't forget to check working with duplicates by Pigeon Hole principle
    const int TEST_SIZE = 100, MAX_PRIORITY = 50;

    priorityqueue<int> pq1;
    priority_queue<Wrapper<int>>
            pq1_bltn;

    for (int i = 0; i < TEST_SIZE; i++) {
        int x = rand(), y = rand() % MAX_PRIORITY;
        pq1.enqueue(x, y);
        pq1_bltn.emplace(x, y, i);
        compare(pq1, pq1_bltn);
        ASSERT_EQ(pq1.toString(), toString(pq1));
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        ASSERT_EQ(pq1.peek(), pq1_bltn.top().value);
        ASSERT_EQ(pq1.dequeue(), pq1_bltn.top().value);
        pq1_bltn.pop();
        compare(pq1, pq1_bltn);
        ASSERT_EQ(pq1.toString(), toString(pq1));
    }
}

TEST(priorityqueue, TestDouble) {
    // Needed to reproduce random values on error
    srand(42);
    const int TEST_SIZE = 100, MAX_PRIORITY = 50;
    priorityqueue<double> pq2;
    priority_queue<Wrapper<double>>
            pq2_bltn;

    for (int i = 0; i < TEST_SIZE; i++) {
        double x = rand() / 1e3;
        int y = rand() % MAX_PRIORITY;

        pq2.enqueue(x, y);
        pq2_bltn.emplace(x, y, i);

        compare(pq2, pq2_bltn);
        ASSERT_EQ(pq2.toString(), toString(pq2));
        ASSERT_EQ(pq2, pq2);
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        ASSERT_EQ(pq2.peek(), pq2_bltn.top().value);
        ASSERT_EQ(pq2.dequeue(), pq2_bltn.top().value);
        pq2_bltn.pop();
        compare(pq2, pq2_bltn);
        ASSERT_EQ(pq2.toString(), toString(pq2));
    }
}

TEST(priorityqueue, TestString) {
    // Needed to reproduce random values on error
    srand(42);
    const int TEST_SIZE = 100, MAX_PRIORITY = 50;
    priorityqueue<string> pq3;
    priority_queue<Wrapper<string>>
            pq3_bltn;

    for (int i = 0; i < TEST_SIZE; i++) {
        string x = randomString(10);
        int y = rand() % MAX_PRIORITY;

        pq3.enqueue(x, y);
        pq3_bltn.emplace(x, y, i);

//        auto pq4 = pq3;
//        pq4.clear();
//
//        ASSERT_EQ(pq3 == pq4, false);

        compare(pq3, pq3_bltn);
        ASSERT_EQ(pq3.toString(), toString(pq3));
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        ASSERT_EQ(pq3.peek(), pq3_bltn.top().value);
        ASSERT_EQ(pq3.dequeue(), pq3_bltn.top().value);
        pq3_bltn.pop();
        compare(pq3, pq3_bltn);
        ASSERT_EQ(pq3.toString(), toString(pq3));
    }

}
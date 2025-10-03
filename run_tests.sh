#!/bin/bash

cd build || { echo "Error: build folder not found!"; exit 1; }

echo "Running ./cache_belady_tests"
./cache_belady_tests
BELADY_TESTS_RESULT=$?

echo "Running ./cache_2q_tests"
./cache_2q_tests
TWOQ_TESTS_RESULT=$?

cd ..

echo ""
echo "=== Running huge_test ==="

if [ ! -f "./test_huge/huge_test" ]; then
    echo "Error: File ./test_huge/huge_test not found in root directory!"
    exit 1
fi

if [ ! -f "./test_huge/huge_test_2q_answ" ]; then
    echo "Error: File ./test_huge/huge_test_2q_answ not found in root directory!"
    exit 1
fi

if [ ! -f "./test_huge/huge_test_belady_answ" ]; then
    echo "Error: File ./test_huge/huge_test_belady_answ not found in root directory!"
    exit 1
fi

EXPECTED_ANSWER_2Q=$(cat ./test_huge/huge_test_2q_answ)

echo ""
echo "=== Testing cache_2q ==="
CACHE_2Q_HITS=$(./build/cache_2q < ./test_huge/huge_test)

if [ "$CACHE_2Q_HITS" -eq "$EXPECTED_ANSWER_2Q" ]; then
    echo "cache_2q: TEST PASSED"
else
    echo "cache_2q: TEST FAILED"
    echo "   Expected: $EXPECTED_ANSWER_2Q"
    echo "   Actual: $CACHE_2Q_HITS"
fi

EXPECTED_ANSWER_BELADY=$(cat ./test_huge/huge_test_belady_answ)

echo ""
echo "=== Testing cache_belady ==="
CACHE_BELADY_HITS=$(./build/cache_belady < ./test_huge/huge_test)

if [ "$CACHE_BELADY_HITS" -eq "$EXPECTED_ANSWER_BELADY" ]; then
    echo "cache_belady: TEST PASSED"
else
    echo "cache_belady: TEST FAILED"
    echo "   Expected: $EXPECTED_ANSWER_BELADY"
    echo "   Actual: $CACHE_BELADY_HITS"
fi

echo ""
echo "=== RESULT ==="

if [ $BELADY_TESTS_RESULT -eq 0 ] && [ $TWOQ_TESTS_RESULT -eq 0 ] &&
   [ "$CACHE_2Q_HITS" -eq "$EXPECTED_ANSWER_2Q" ] && [ "$CACHE_BELADY_HITS" -eq "$EXPECTED_ANSWER_BELADY" ]; then
    echo "All tests passed successfully."
    exit 0
else
    echo "Some tests failed."
    exit 1
fi

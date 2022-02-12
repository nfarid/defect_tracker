
// Tell DrogonTest to generate `test::run()`. Only defined this in the main file
#define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>

int sum_all(int n) {
    int result = 0;
    for(int i=1; i<=n; i++)
        result += i;
    return result;
}

DROGON_TEST(Sum){
    CHECK(sum_all(0) == 0);
    CHECK(sum_all(1) == 1);
    CHECK(sum_all(2) == 3);
    CHECK(sum_all(3) == 6);
}

int main(int argc, char** argv) {
    return drogon::test::run(argc, argv);
}

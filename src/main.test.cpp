
// Tell DrogonTest to generate `test::run()`. Only defined this in the main file
#define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>

int main(int argc, char** argv) {
    return drogon::test::run(argc, argv);
}

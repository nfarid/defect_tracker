
#include "./app.hpp"

int main() {
    auto& app = get_app();
    app.run();

    return EXIT_SUCCESS;
}

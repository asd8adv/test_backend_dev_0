#include "../source/TopTrackerTest.h"

int main()
{
    test_add_and_limit();
    test_cleanup();
    test_order();

    std::cout << "All TopTracker tests passed." << std::endl;
    return 0;
}
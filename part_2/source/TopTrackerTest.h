#include "TopTracker.h"

#include <cassert>
#include <iostream>
#include <thread>


class TopTrackerTest: public TopTracker {
public:
    TopTrackerTest(size_t max_size, std::chrono::seconds timeout):TopTracker(max_size, timeout)
    {
    }
    
    void add_test_action_with_time(uint64_t player_id, const std::string& action, TimePoint time)
    {
        add_action_with_time(player_id, action, time);
    }
};


void test_add_and_limit()
{
    TopTrackerTest tracker(3, std::chrono::seconds(10));
    tracker.add_action(1, "login");
    tracker.add_action(2, "move");
    tracker.add_action(3, "shoot");
    tracker.add_action(4, "logout");

    auto actions = tracker.get_actions();
    assert(actions->size() == 3);
    assert((*actions)[0].player_id == 2);
    assert((*actions)[1].player_id == 3);
    assert((*actions)[2].player_id == 4);
}

void test_cleanup()
{
    TopTrackerTest tracker(5, std::chrono::seconds(1));
    auto now = std::chrono::steady_clock::now();

    tracker.add_test_action_with_time(1, "login", now - std::chrono::seconds(5));
    tracker.add_test_action_with_time(2, "move", now - std::chrono::seconds(3));
    tracker.add_test_action_with_time(3, "shoot", now);

    tracker.cleanup();

    auto actions = tracker.get_actions();
    assert(actions->size() == 1);
    assert((*actions)[0].player_id == 3);
}

void test_order()
{
    TopTrackerTest tracker(3, std::chrono::seconds(10));
    tracker.add_action(10, "jump");
    tracker.add_action(20, "run");
    tracker.add_action(30, "hide");

    auto actions = tracker.get_actions();
    assert((*actions)[0].action == "jump");
    assert((*actions)[2].action == "hide");
}

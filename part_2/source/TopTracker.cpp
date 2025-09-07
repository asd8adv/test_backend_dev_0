#include "TopTracker.h"

TopTracker::TopTracker(size_t max_size, std::chrono::seconds timeout)
    : max_size(max_size), timeout(timeout)
{
    actions = std::make_shared<std::deque<PlayerAction>>();
}

void TopTracker::add_action(uint64_t player_id, const std::string& action)
{
    auto now = std::chrono::steady_clock::now();
    add_action_with_time(player_id, action, now);
}

void TopTracker::add_action_with_time(uint64_t player_id, const std::string& action, TimePoint time)
{
    // ƒобавл€ем новое действие в конец очереди
    actions->emplace_back(player_id, action, time);

    // ≈сли превышен лимит - удал€ем самое старое действие (с начала)
    while (actions->size() > max_size)
        actions->pop_front();
}

void TopTracker::cleanup()
{
    auto now = std::chrono::steady_clock::now();
    // ”дал€ем просроченные действи€ (с начала очереди)
    while (!actions->empty() &&
           std::chrono::duration_cast<std::chrono::seconds>(now - actions->front().timestamp) > timeout)
    {
        actions->pop_front();
    }
}

std::shared_ptr<const std::deque<PlayerAction>> TopTracker::get_actions() const
{
    return std::static_pointer_cast<const std::deque<PlayerAction>>(actions);
}
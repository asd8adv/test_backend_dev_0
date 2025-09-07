// � �������� ���������� �������� ������ ������ deque �������:
// ������� ��������(FIFO) � ������ ����� ������� ����������� �� ������� ���������� �������� �� ��������.
// ������� � �������� O(1)
// ����������� ������ �� ������� (Random Access Iterator)
// ������� ����� ���� �� ����������� ������� ������� std::unordered_set �� ��������, 
// �� �������� ������������� std::multiset, 
// � �� ������ ��� ���� ����� ��������� �������� � ������� ��� ��������� ��������.
// ***
// ����� get_actions - deque ����������� � ������� �� ���������, 
// �� �������� ����� ���� ��������. ���� �� ����� ��������� ����� �������� �����:
// ���������� ����� ��� ������������� ���������� ��������� �������(�������� cbegin,cend)

#pragma once

#include <chrono>
#include <deque>
#include <memory>
#include <string>

// ��������� ��� �������� �������� ������
struct PlayerAction
{
    uint64_t player_id;
    std::string action;
    std::chrono::steady_clock::time_point timestamp;

    PlayerAction(uint64_t id, const std::string& act, std::chrono::steady_clock::time_point time)
        : player_id(id), action(act), timestamp(time) {}
};

// ������ ������������ ��������� N �������� �������
class TopTracker
{
public:
    using TimePoint = std::chrono::steady_clock::time_point;

    TopTracker(size_t max_size, std::chrono::seconds timeout);

    void add_action(uint64_t player_id, const std::string& action);
    void cleanup();

    std::shared_ptr<const std::deque<PlayerAction>> get_actions() const;

protected:
    void add_action_with_time(uint64_t player_id, const std::string& action, TimePoint time);

protected:
    size_t max_size;
    std::chrono::seconds timeout;
    std::shared_ptr<std::deque<PlayerAction>> actions;
};
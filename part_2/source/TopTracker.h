// в качестве контейнера хранения данных выбран deque причины:
// порядок хранения(FIFO) в случае когда вставка упорядочена по времени упрощается отчистка по таймауту.
// вставка и удаление O(1)
// возможность обхода по индексу (Random Access Iterator)
// события могут быть не уникальными поэтому обычный std::unordered_set не подойдет, 
// но возможно использование std::multiset, 
// я бы выбрал его если нужно добавлять действия с будущим или прошедшим временем.
// ***
// метод get_actions - deque константный и защищен от изменений, 
// но элементы могут быть изменены. если мы хотим полностью этого избежать нужно:
// возвращать копию или предоставлять защищенный интерфейс доступа(например cbegin,cend)

#pragma once

#include <chrono>
#include <deque>
#include <memory>
#include <string>

// Структура для хранения действия игрока
struct PlayerAction
{
    uint64_t player_id;
    std::string action;
    std::chrono::steady_clock::time_point timestamp;

    PlayerAction(uint64_t id, const std::string& act, std::chrono::steady_clock::time_point time)
        : player_id(id), action(act), timestamp(time) {}
};

// Модуль отслеживания последних N действий игроков
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
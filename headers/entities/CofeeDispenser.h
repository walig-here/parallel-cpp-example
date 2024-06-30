#ifndef COFFEE_DISPENSER_H
#define COFFEE_DISPENSER_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Subject.h"

class EmployeeThread;
class MaintainerThread;

class CoffeeDispenser : public Subject
{
public:
    static const int MAX_CAPACITY;
    static const int REFILL_COFFEE_LEVEL;
    
    static CoffeeDispenser& getDispenser(int index);
    static int getNumberOfDispensers();
    static int waitUntilAnyDispenserNeedsMaintainer(const MaintainerThread& maintainer);
    static void notifyDispenserNeedsMaintainer();

    CoffeeDispenser(const int& initial_cofee = 100);
    ~CoffeeDispenser();
    int getAmountOfCoffee() { return coffee; }
    void buyCoffee(EmployeeThread& buyer);
    void maintain(MaintainerThread& maintainer);
    bool doNeedMainainer();

private:
    static std::vector<CoffeeDispenser*> all_dispensers;
    static const int COFFEE_RATION_FOR_ONE_EMPLOYEE;
    static const int DISPENSING_COFFEE_TIME_SECONDS; 
    static const int INSPECT_TIME_SECONDS;
    static std::condition_variable does_any_dispenser_need_maintainer;
    static std::mutex dispensers_list_mutex;

    int coffee;
    std::mutex access_dispenser_mutex;
    std::queue<std::string> queue_to_dispenser;
    std::condition_variable is_dispenser_functional;
    bool is_broken;
    bool is_assigned_to_maintainer;
    int breaking_probability_percent;
    int repair_time_miliseconds;

    void simulate_breaking();
};

#endif

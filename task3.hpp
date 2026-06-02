#ifndef TASK3_HPP
#define TASK3_HPP

#include <string>
#include "Task2_robotAssignment.hpp"

namespace RobotNavigation
{
    // Entry point called from main.cpp — displays robot list, collects input, then navigates
    void runMenu(RobotAssignment::CircularLinkedList<RobotAssignment::Robot> &robotList);

    // Core navigation logic — receives robotID and destination from runMenu()
    void runDemo(const std::string &robotID, const std::string &destination);
}

#endif

#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include <string>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayCommands(WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent, float total = 100,
                        std::string unit = "%");
};  // namespace NCursesDisplay

#endif
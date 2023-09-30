#pragma once
#include <functional>

void
init_signals(const std::function<void(const char*)>& reset);

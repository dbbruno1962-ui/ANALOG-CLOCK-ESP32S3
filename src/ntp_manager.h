#pragma once

#include <stdint.h>

void initTime();
bool syncTime(int16_t &hh, int16_t &mm, int16_t &ss);
bool needResync();
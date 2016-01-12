//
// Created by Christian Hamm on 26.04.15.

#pragma once

#include <SDL.h>

#define log_debug(...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define HERO_DYNLOAD

#include "globals.h"
#include "logic.h"
#include "platform.h"
#include "debug_stuff.h"


//
// Created by Christian Hamm on 26.04.15.

#ifndef HERO_HERO_H
#define HERO_HERO_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <SDL.h>

#define log_debug(...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

#include "globals.h"
#include "logic.h"
#include "platform.h"
#include "debug_stuff.h"

#endif //HERO_HERO_H

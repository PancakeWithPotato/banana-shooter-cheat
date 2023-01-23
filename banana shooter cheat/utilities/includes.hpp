#pragma once
#include <Windows.h>
#include <iostream>
#include <format>
#include <stdio.h>
#include <string>
#include <thread>
#include <d3d11.h>
#include <array>
#include <ctime>

#include "../dependencies/termcolor/termcolor.hpp"

#define SUCCES termcolor::green
#define WAIT termcolor::yellow
#define ERR termcolor::red 
#define RESET termcolor::reset
#define CHANGE termcolor::cyan

#include "debug.hpp"

#include "../core/config/config.hpp"

#include "../sdk/sdk.hpp"
#include "../sdk/functions.hpp"

#include "../core/hack.hpp"
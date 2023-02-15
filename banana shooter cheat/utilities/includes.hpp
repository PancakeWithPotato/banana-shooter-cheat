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
#include <vector>
#include <unordered_map>
#include <map>
#include <variant>
#include <filesystem>
#include <fstream>

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_freetype.h"

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

#include "../core/visuals/visuals.hpp"
#include "../core/visuals/notifactions.hpp"

#include "../dependencies/easing-functions/easing.h" //credit: https://github.com/nicolausYes/easing-functions

//lua
#include "../lua/meowLua.hpp"

//fonts
#include "font/roboto.hpp"
#include "font/monsterrat.hpp"
#include "font/verdana.hpp"
#include "font/helvetica.hpp"
#include "font/pixel7.hpp"
#include "font/segoui.hpp"

#define WHITE ImVec4(1.f, 1.f, 1.f, 1.f);


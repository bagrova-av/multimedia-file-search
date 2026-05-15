#pragma once
#ifndef PARSE_UTILS_H
#define PARSE_UTILS_H

#include <optional>

#include "../AppConfig/AppConfig.h"

std::optional<AppConfig> parseArguments(int argc, char**);

#endif
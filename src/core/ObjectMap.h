#pragma once

#include <unordered_map>

class Object;

template<class T>
using ObjectMap = std::unordered_multimap<std::string, T*>;

template<class T>
using ObjectMapRef = typename std::unordered_multimap<std::string, T*>::iterator;

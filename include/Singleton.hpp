/**
 * @file Singleton.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief Singleton class is a simple class for creating a singleton object
 * @version 0.1
 * @date 2024-05-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

template<typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }
};

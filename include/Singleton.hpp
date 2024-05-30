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

/**
 * @brief Singleton class is a simple
 *       class for creating a singleton object
 * @tparam T The type of the singleton object
 */
template<typename T>
class Singleton {
public:
    /**
     * @brief getInstance is a static function that returns the singleton
     *        instance of the Singleton object
     * @return T& The singleton instance of the Singleton object
     */
    static T& getInstance() {
        static T instance;
        return instance;
    }
};

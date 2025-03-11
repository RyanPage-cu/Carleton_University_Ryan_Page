/*
SYSC 3303 - Assignment 1
Author : Ryan Page (101268082)
Email : RyanPage@cmail.carleton.ca
Date Created : 2025-01-23
Last Edited : 2025-01-25
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <vector>
#include <string>
#include <algorithm>

int sandwich_count = 0;

/**
 * Represents a table where ingredients are placed for making sandwiches.
 * Synchronization is handled using a mutex and condition variable mechanism.
 */

class Table {
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::string ingredient1, ingredient2;
    bool ready = false;

public:

    /**
     * Places two ingredients on the table. Blocks if the table is already occupied.
     *
     * @param ing1 The first ingredient.
     * @param ing2 The second ingredient.
     */

    void placeIngredients(const std::string &ing1, const std::string &ing2) {
        std::unique_lock<std::mutex> lock(mtx); //locking mutex

        cv.wait(lock, [this]() { return !ready; });

        ingredient1 = ing1;
        ingredient2 = ing2;
        ready = true;

        sandwich_count++;
        std::cout << "\n--------------------------------" << std::endl;
        std::cout << "Sandwich #" << sandwich_count <<std::endl;

        std::cout << "Agent placed: " << ing1 << " and " << ing2 << std::endl;
        cv.notify_all(); 
    }

    /**
     * Allows a chef to take ingredients from the table if they do not match the chef's own ingredient.
     * Blocks if ingredients are not ready or not suitable.
     *
     * @param chefIngredient The ingredient the chef already has.
     * @param id             The unique identifier for the chef.
     * @return true if ingredients were successfully taken, false if operation should terminate.
     */

    bool takeIngredients(const std::string &chefIngredient, int id) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this, &chefIngredient]() { 
            return ready && (ingredient1 != chefIngredient && ingredient2 != chefIngredient);
        });

        
        std::cout << "Chef #" << id << " with " << chefIngredient << " making sandwich using " 
                    << ingredient1 << " and " << ingredient2 << std::endl;
        std::cout << "--------------------------------" << std::endl;


        ready = false;
        cv.notify_one();

        // Simulate the time taken to make a sandwich
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 1000ms delay

        return true;
    }
};

/**
 * Represents an agent responsible for placing ingredients on the table.
 */
class Agent {
private:
    Table &table;
    std::vector<std::string> ALLingredients = {"Bread", "Peanut Butter", "Jam"};
    std::random_device rd;
    std::mt19937 gen;

public:
    /**
     * Constructor for the Agent class.
     *
     * @param table The shared table object.
     */
    Agent(Table &table) : table(table), gen(rd()) {}

    void operator()() {
        while (sandwich_count < 20) { // Run until 20 sandwiches are made
            /**
             * Randomly shuffles the ingredients array.
             */
            std::shuffle(ALLingredients.begin(), ALLingredients.end(), gen); // Shuffle the ingredients
            table.placeIngredients(ALLingredients[0], ALLingredients[1]);    // Place two random ingredients
        }
    }
};

/**
 * Represents a chef responsible for making sandwiches.
 */
class Chef {
private:
    std::string ingredient;
    Table &table;
    int id;

public:
    /**
     * Constructor for the Chef class.
     *
     * @param table      The shared table object.
     * @param ingredient The ingredient the chef always has.
     * @param id         The unique identifier for the chef.
     */
    Chef(Table &table, const std::string &ingredient, int id) : table(table), ingredient(ingredient), id(id) {}

    void operator()() {
        while (sandwich_count < 20) {
            if (!table.takeIngredients(ingredient, id)) break; // Exit if no more sandwiches are needed
            
            // Simulate the time taken to make a sandwich
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 1000ms delay
        }
    }
};

/**
 * Main class to initiate the simulation.
 */
int main() {
    Table table;

    // Create agent and chefs with unique ids
    Agent agent(table);
    Chef chef1(table, "Bread", 1);
    Chef chef2(table, "Peanut Butter", 2);
    Chef chef3(table, "Jam", 3);

    // Start threads
    std::thread agentThread(std::ref(agent)); // Pass agent by reference
    std::thread chef1Thread(std::ref(chef1)); // Pass chef1 by reference
    std::thread chef2Thread(std::ref(chef2)); // Pass chef2 by reference
    std::thread chef3Thread(std::ref(chef3)); // Pass chef3 by reference

    // Wait for threads to complete
    agentThread.join();
    chef1Thread.join();
    chef2Thread.join();
    chef3Thread.join();

    return 0;
}

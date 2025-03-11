#include <iostream>
#include <thread>
#include "Assignment_3_StateMachine.hpp"

void runContext(Context& context) {
    context.run();
}

int main() {
    Context context;
    std::thread Traffic_Loop(runContext, std::ref(context));
    
    std::thread Pedestrian_Waiting([&context](){
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(25));
            context.signalPedestrianWaiting();
        }
    });

    // Wait for the thread to finish
    Traffic_Loop.join();
    Pedestrian_Waiting.join();

    return 0;
}
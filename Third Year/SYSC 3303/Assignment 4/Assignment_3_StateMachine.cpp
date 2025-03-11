/**
 * SYSC 3303 RealTime Concurrent Systems
 * Assignment #3 - Statemachines
 * @author Ryan Page
 * @date 2021-06-20
 * @version 1.0
 */

 #include <thread>
 #include <chrono>
 #include <iostream>
 #include "Assignment_3_StateMachine.hpp"

//Starts in Vehicle Green State
 Context::Context() {currentState = States::vehicleGreen; isPedestrianWaiting = signalPedestrians::DONT_WALK; vehicleSignal = signalVehicles::GREEN;}

void Context::run() {
    while(true){
        switch(currentState) {
            case States::vehicleGreen:
                handlevehicleGreen();
                break;
            case States::vehicleGreenInt:
                handlevehicleGreenInt();
                break;
            case States::vehicleYellow:
                handlevehicleYellow();
                break;
            case States::pedestrianWalk:
                handlepedestrianWalk();
                break;
            case States::pedestrianFlash:
                handlepedestrianFlash();
                break;
        }
    }
}

void Context::setState(States newState) {
    currentState = newState;
}

States Context::getState() {
    return currentState;
}

void Context::pedestrianWaiting() {
    isPedestrianWaiting = signalPedestrians::WALK;
    if(getState() == States::vehicleGreen || getState() == States::vehicleGreenInt){
        std::cout << "Pedestrian pressed button\n";
    }else if (getState() == States::vehicleYellow){
        std::cout << "Pedestrian Walk Request was Registered will change to Green Figure soon\n";
    }else if (getState() == States::pedestrianWalk){
        std::cout << "PedestrianWalk: Pedestrian Walking Across\n";
    }else if (getState() == States::pedestrianFlash){
        std::cout << "Pedestrian request will be handled in next cycle\n";
    }
}

void Context::timeout() {
    if(getState() == States::vehicleGreen){
        if (getPedestrianWaiting() == signalPedestrians::WALK) {
            pedestrianWaiting();
            currentState = States::vehicleYellow;
        } else {
            currentState = States::vehicleGreenInt;
        }
    }else if(getState() == States::vehicleGreenInt){
        currentState = States::vehicleYellow;
    }else if (getState() == States::vehicleYellow){
        currentState = States::pedestrianWalk;
    }else if (getState() == States::pedestrianWalk){
        resetPedestrianFlashCtr();
        currentState = States::pedestrianFlash;
    }else if (getState() == States::pedestrianFlash){
        if (getPedestrianFlashCtr() > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            currentState = States::pedestrianFlash;
        }else{
            pedestrianWaiting();
            currentState = States::vehicleGreen;
        }
    }
}

void Context::signalPedestrianWaiting() {
    isPedestrianWaiting = signalPedestrians::WALK;
}

signalPedestrians Context::getPedestrianWaiting() {
    return isPedestrianWaiting;
}

void Context::resetPedestrianWaiting() {
    isPedestrianWaiting = signalPedestrians::DONT_WALK;
}

int Context::getPedestrianFlashCtr() {
    return pedestrianFlashCtr;
}

void Context::decrementPedestrianFlashCtr() {
    pedestrianFlashCtr--;
}

void Context::resetPedestrianFlashCtr() {
    if (pedestrianFlashCtr == 0) {  // Only reset if it was previously zero
        pedestrianFlashCtr = 7;
    }
}

signalPedestrians Context::getisPedestrianWaiting() {
    return isPedestrianWaiting;
}

signalVehicles Context::getSingalVehicles() {
    return vehicleSignal;
}

void Context::setSignalVehicles(signalVehicles signal) {
    vehicleSignal = signal;
}

void Context::handlevehicleGreen() {
    setSignalVehicles(signalVehicles::GREEN);
    isPedestrianWaiting = signalPedestrians::DONT_WALK;
    std::cout << "Vehicle State: " << (getSingalVehicles() == signalVehicles::GREEN ? "GREEN" : "YELLOW") << " Pedestrian State: " << (getisPedestrianWaiting() == signalPedestrians::WALK ? "WALK" : "DONT_WALK") << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
    timeout();
}

void Context::handlevehicleGreenInt() {
    setSignalVehicles(signalVehicles::GREEN);
    std::cout << "Vehicle State: " << (getSingalVehicles() == signalVehicles::GREEN ? "GREEN" : "YELLOW") << " Pedestrian State: " << (getisPedestrianWaiting() == signalPedestrians::WALK ? "WALK" : "DONT_WALK") <<  " (This State Until isPedestrianWaiting is WALK not DONT_WALK)" << "\n";
    while(getPedestrianWaiting() == signalPedestrians::DONT_WALK){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    pedestrianWaiting();
    timeout();
}

void Context::handlevehicleYellow() {
    setSignalVehicles(signalVehicles::YELLOW);
    std::cout << "Vehicle State: " << (getSingalVehicles() == signalVehicles::GREEN ? "GREEN" : "YELLOW") << " Pedestrian State: " << (getisPedestrianWaiting() == signalPedestrians::WALK ? "DONT_WALK" : "DONT_WALK") << "\n";
    pedestrianWaiting();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    timeout();
}

void Context::handlepedestrianWalk() {
    setSignalVehicles(signalVehicles::RED);
    std::cout << "Vehicle State: " << (getSingalVehicles() == signalVehicles::GREEN ? "GREEN" : (getSingalVehicles() == signalVehicles::YELLOW ? "YELLOW" : "RED")) << " Pedestrian State: " << (getisPedestrianWaiting() == signalPedestrians::WALK ? "WALK" : "DONT_WALK") << "\n";
    pedestrianWaiting();
    std::this_thread::sleep_for(std::chrono::seconds(15));
    timeout();
}

void Context::handlepedestrianFlash() {
    setSignalVehicles(signalVehicles::YELLOW);
    if (getPedestrianFlashCtr() %2 == 0) {
        isPedestrianWaiting = signalPedestrians::BLANK;
        decrementPedestrianFlashCtr();
    } else {
        isPedestrianWaiting = signalPedestrians::DONT_WALK;
        decrementPedestrianFlashCtr();
    }
    std::cout << "Vehicle State: " << (getSingalVehicles() == signalVehicles::GREEN ? "GREEN" : "YELLOW") << " Pedestrian State: " << (getisPedestrianWaiting() == signalPedestrians::DONT_WALK ? "DONT_WALK" : "BLANK") << "\n";
    timeout();
}


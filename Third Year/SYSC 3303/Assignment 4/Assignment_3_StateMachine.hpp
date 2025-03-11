#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP
#include <iostream>
#include <thread>
#include <chrono>

enum class States {vehicleGreen, vehicleYellow, vehicleGreenInt, pedestrianWalk, pedestrianFlash};
enum class signalPedestrians {DONT_WALK, WALK, BLANK};
enum class signalVehicles {GREEN, YELLOW, RED};

class Context {
public:
    Context();
    void run();
    void setState(States newState);
    States getState();
    void pedestrianWaiting();
    void timeout();
    void signalPedestrianWaiting();
    signalPedestrians getPedestrianWaiting();
    void resetPedestrianWaiting();
    int getPedestrianFlashCtr();
    void decrementPedestrianFlashCtr();
    void resetPedestrianFlashCtr();
    enum signalPedestrians getisPedestrianWaiting();
    enum signalVehicles getSingalVehicles();
    void setSignalVehicles(signalVehicles signal);
private:
    States currentState;
    signalPedestrians isPedestrianWaiting;
    signalVehicles vehicleSignal;
    int pedestrianFlashCtr = 7;

    void handlevehicleGreen();
    void handlevehicleGreenInt();
    void handlevehicleYellow();
    void handlepedestrianWalk();
    void handlepedestrianFlash();
};

#endif // STATEMACHINE_HPP
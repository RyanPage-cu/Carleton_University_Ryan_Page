# SYSC 3303A Realtime Concurrent Systems Winter 2025

## Assignment #3 - State Machines

# **Pelican Crossing – How It Works**

## **Overview**

A **Pelican (Pedestrian Light Controlled) crossing** is a pedestrian crossing system that uses traffic lights to stop vehicles and allow pedestrians to cross safely. It is commonly found in the UK and other regions.

## **How It Works**

1. **Pedestrian Activation**

   - Pedestrians press a button to request to cross the road.

2. **Waiting Period**

   - The pedestrian signal displays **"WAIT"**, and the traffic lights remain **green** for vehicles.

3. **Traffic Light Changes to Red**

   - After a short delay, the traffic lights change to **amber**, then **red**, stopping vehicles.
   - The pedestrian signal changes to a **steady green man**, allowing safe crossing.

4. **Flashing Green Man (Warning Phase)**

   - After a set crossing time, the **green man begins flashing**, warning pedestrians that time is running out.
   - Vehicles must **give way** to pedestrians still on the crossing.

5. **Traffic Light Turns Green for Vehicles**

   - The pedestrian signal turns **off**.
   - The vehicle traffic lights turn **green**, allowing traffic to move again.

---

## **Project Files**

### **1. Assignment_3_StateMachine.hpp**

- Contains the class definitions for the **Context** and **State Machine** handling the Pelican Crossing system.
- Defines the **States** and their transitions.
- Includes function prototypes for handling state changes.

### **2. Assignment_3_StateMachine.cpp**

- Implements the Pelican Crossing state machine.
- Defines behavior for each state: **vehicleGreen, vehicleYellow, pedestrianWalk, pedestrianFlash**.
- Implements transition logic, including pedestrian button press handling.

### **3. Assignment_3_Testing.cpp**

- Provides a test harness for the state machine.
- Simulates pedestrian button presses and state transitions.
- Runs the `Context` object in a separate thread to mimic real-time behavior.

---
## **Issues That Was Needed to Fix**

### ** 1. Handling pedestrainWaiting in Multiple States**

- **Issue:** The pedestrianWaiting signal is ignored when pedestrians start waiting during the PedestriansFlash state.
- **Solution:** In the PedestriansFlash, Operational, and VehiclesEnabled states, set the isPedestrianWaiting variable to true when the pedestrianWaiting signal is received. This ensures that early pedestrian waiting signals are not ignored.

### ** 2. Resetting isPedestrianWaiting in Correct State**

- **Issue:** Resetting the isPedestrianWaiting flag in the VehiclesGreen state is prematurely resetting it and causing early signals to be lost.
- **Solution:** Reset the isPedestrianWaiting flag when transitioning into the PedestriansWalk state, which ensures that early pedestrian waiting signals aren't lost when transitioning from VehiclesGreen to PedestriansWalk.

### ** 3. Conditional Setting of PedrestrianFlashCtr**

- **Issue:** The pedestrianFlashCtr is reset to 7 every time the PedestriansFlash state is re-entered, causing an infinite loop of resetting the counter.
- **Solution:** Set pedestrianFlashCtr to 7 only when entering the state for the first time after it was last reset to 0. You can use a flag or check if the counter has already been reset before.
---

## **How to Compile and Run**

### **Compilation**

Use the following command to compile the project using g++:

```bash
 g++ -std=c++11 Assignment_3_StateMachine.cpp Assignment_3_Testing.cpp -o pelican_crossing -pthread
```

### **Execution**

Run the executable:

```bash
 ./pelican_crossing
```

This will simulate the Pelican Crossing system, allowing pedestrian button presses at set intervals.

---

## **Expected Output**

- The program will print messages showing traffic light changes and pedestrian crossing states.
- It will indicate when a pedestrian presses the button and when the system responds.

---

## **Notes**

- The program runs in a loop; use **Ctrl+C** to stop execution.
- Timing values (e.g., wait times for signals) can be adjusted in `StateMachine.cpp` for testing.

---

## **Author**

**Ryan Page**\
SYSC 3303A - Winter 2025\
Carleton University



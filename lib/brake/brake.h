#ifndef BRAKE_H
#define BRAKE_H

#define BRAKE_THRESHOLD 0

class brake{
private:
     int pin_one;
     int pin_two;
     int light_pin;
public:
    /**
     * @brief Construct a new brake object
     * 
     * @param pin_one sensor pin in from the left brake sensor 
     * @param pin_two sensor pin in from the right brake sensor
     * @param pin_light sensor pin out for the brake light
     */
    brake(int pin_one, int pin_two, int light_pin);

    /**
    * @brief reads the raw value of the brake
    * 
    * @return int from 0 to 1023
    */
    int get_pressure();

    /**
     * @brief gives the value of the brake as a percentage 0 to 100
     * 
     * @return linearly mapped value from 0-100
     */
    int pressure_percentage();

    /**
     * @brief turns the brake light on if the pad is depressed
     * 
     */
    void brake_light();
};

#endif BRAKE_H
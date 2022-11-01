#ifdef BRAKE_H
#define BRAKE_H

class brake{
public:
    /**
     * @brief Construct a new brake object
     * 
     * @param pin_left sensor pin in from the left brake sensor 
     * @param pin_right sensor pin in from the right brake sensor
     * @param light_pin sensor pin out for the brake light
     */
    brake(int pin_left, int pin_right, int light_pin);

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
}

#endif BRAKE_H
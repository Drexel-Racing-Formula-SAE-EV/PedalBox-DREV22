#ifndef LINEAR_POTENIOMETER_H
#define LINEAR_POTENIOMETER_H

#define LPOT_MIN    84      // Left Potentiometer Lower Bound
#define LPOT_MAX    1023    // Left Potentiometer Upper Bound
#define RPOT_MIN    359     // Right Potentiometer Lower Bound
#define RPOT_MAX    1023    // Right Potentiometer Upper Bound

/**
 * @brief Range of reachable values from poteniometer
 */
struct poten_range {
    int lower_bound;
    int upper_bound;
};

class poten {
public:
    /**
     * @brief Construct a new poten object
     *
     * @param pin Board pin value
     * @param range Poteniometer range
     */
    poten(int pin, poten_range range);

    /**
     * @brief Get the raw analog value of the poteniometer
     * @return Raw poteniometer value
     */
    int read();

    /**
     * @brief Get the percent extension by the potentiometer as defined by the
     * poten_range
     *
     * @return Linearly mapped value from valid range to 0-100
     */
    int read_percent();

private:
    int m_pin;
    poten_range m_range;
};

#endif /* LINEAR_POTENIOMETER_H */
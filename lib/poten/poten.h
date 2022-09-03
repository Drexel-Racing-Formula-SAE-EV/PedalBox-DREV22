#ifndef LINEAR_POTENIOMETER_H
#define LINEAR_POTENIOMETER_H

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
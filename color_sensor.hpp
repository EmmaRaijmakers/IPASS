
//          Copyright Emma Raijmakers 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


//This file contains Doxygen lines

#ifndef COLOR_SENSOR_HPP
#define COLOR_SENSOR_HPP

#include <hwlib.hpp>
#include <array>

/// @file

/// \brief
/// Virtual color sensor class
/// \details
/// This virtual color sensor class is used as a super class
/// to all possible color sensor chips that use I2C communication. 
/// It is initialized with a data and clock pin in order to create 
/// a bus over which I2C communication can be done. This class makes 
/// sure that every chip derived from this class, has functions to setup
/// the chip and read from the chip. Therefore, it is a general
/// class.
class color_sensor
{
protected:
    hwlib::i2c_bus_bit_banged_scl_sda bus;
    
    bool setup_done = false;
    
private:
    /// \brief
    /// Virtual read one color
    /// \details
    /// This function is used to implement a read function for 
    /// one color register in a derived class. It is used to read 
    /// an 8 bit value from a color register of the choosen chip. It
    /// can also be used to read from registers with larger sizes
    /// or to read from more registers, by calling the function
    /// more times. 
    virtual uint8_t read_color(const uint8_t & color_register) = 0;

public:
    /// \brief
    /// Color sensor constructor function
    /// \details
    /// This constructor uses a clock pin and a data pin in order to
    /// create a bus for I2C communication. The created bus is a 
    /// protected member variable, so that all the derived color chip
    /// classes are able to use this bus. 
    color_sensor(hwlib::target::pin_oc & scl, hwlib::target::pin_oc & sda):
        bus(scl, sda)
    {}
    
    /// \brief
    /// Virtual set up chip 
    /// \details
    /// This function is used to set up a color sensor chip of a
    /// derived class. This set up can differ in every chip and 
    /// therefore it is not implemented in the super class but in 
    /// the derived class. Generally, it uses an activate color engine 
    /// byte and an enable register byte. 
    virtual void setup() = 0;
    
    /// \brief
    /// Virtual read chip 
    /// \details
    /// This function is used to implement a read function for an
    /// RGB value in a derived class. It returns 3 16 bit values of 
    /// colors in an array. Usually in the order red, green, blue. 
    /// This function can make use of the read_color function in order 
    /// to read 1 color register and fill the values of the array which is 
    /// returned in this function.
    virtual std::array<uint16_t, 3> read() = 0;
};

#endif // COLOR_SENSOR_HPP


//          Copyright Emma Raijmakers 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


//This file contains Doxygen lines

#ifndef APDS_9960_HPP
#define APDS_9960_HPP

#include "color_sensor.hpp"

/// @file

/// \brief
/// Class for the APDS_9960 color sensor chip
/// \details
/// This class is used for reading RGB color values from
/// the APDS_9960 chip. It uses the chip's minion, enable
/// register and the bus from the super class color_sensor 
/// in order to enable the color engine, to communicate 
/// through I2C and to send the color value.
class apds_9960 : public color_sensor
{
private:
    
    uint8_t minion = 0x39;
    uint8_t enable_register = 0x80;
    uint8_t enable_color_engine = 0x03;
    uint8_t control_register = 0x8F;
    uint8_t red_lower_register = 0x96;
    
    /// \brief
    /// Read one color register
    /// \details
    /// This function is used to read an 8 bit value from the
    /// given color register of the chip. Firstly, it writes 
    /// the color register to the minion with a hwlib write 
    /// transaction. Secondly, the color is read from the color
    /// register with a hwlib read transaction. Lastly, the
    /// color is returned. This funtion is private, because it is
    /// supposed to be used in the read function.
    uint8_t read_color(const uint8_t & color_register) override{
        uint8_t color;
        
        {
        //Write the color address to the minion
        hwlib::i2c_write_transaction w_color = ((hwlib::i2c_bus*)(&bus))->write(minion);
        w_color.write(color_register);
        }
        
        {
        //Read the color from the minion
        hwlib::i2c_read_transaction r_color = ((hwlib::i2c_bus*)(&bus))->read(minion);
        r_color.read(color);    
        }
        
        hwlib::wait_ms(100);
        
        //return the color
        return color;
    }
    
public:

    /// \brief
    /// APDS_9960 color sensor constructor
    /// \details
    /// This constructor uses a clock pin and a data pin to call the
    /// constructor of the super class to create a bus for I2C 
    /// communication. Further specifications of the APDS_9960 chip 
    /// are saved as member variables as they should not be changed by
    /// the user of this class.
    apds_9960(hwlib::target::pin_oc & scl, hwlib::target::pin_oc & sda):
        color_sensor(scl, sda)
    {}
        
    /// \brief
    /// Set up APDS_9960 
    /// \details
    /// This function is used to set up the color sensor chip
    /// APDS_9960. This set up is done by firstly enabling
    /// the PON and AEN bits in the enable register. This 
    /// activates the color engine. Secondly, the sensitivity
    /// of the color engine is set to the maximum value by 
    /// writing 3 to the AGAIN of the control register.
    void setup() override{
        hwlib::cout << "Color sensor setup \n";
        
        {
        //Write the enable register address and enable byte to the minion
        hwlib::i2c_write_transaction w_enable = ((hwlib::i2c_bus*)(&bus))->write(minion);
        w_enable.write(enable_register);
        w_enable.write(enable_color_engine);
        }

        hwlib::wait_ms(7);
        
        {
        //Write the control register and the maximum sensitivity to the minion
        hwlib::i2c_write_transaction w_enable = ((hwlib::i2c_bus*)(&bus))->write(minion);
        w_enable.write(control_register);
        w_enable.write(3);
        }
        
        hwlib::wait_ms(7);
        
        //set setup_done to true
        setup_done = true;
    }
    
    /// \brief
    /// APDS_9960 read chip 
    /// \details
    /// This function is used to read an RGB value from the APDS_9960. 
    /// It returns an array of 3 16 bit values of colors in the order 
    /// red, green, blue. This function uses the read_color function to 
    /// read the upper and lower registers of red, green and blue. The 8 bit
    /// upper and lower registers are combined to form 1 16 bit value for 
    /// each color. The 3 colors are returned in an array. This function 
    /// only works when the chip set up is done.
    std::array<uint16_t, 3> read() override{
        //If set up is not done, print error message and return empty array
        if(!setup_done){
            hwlib::cout << "Use function setup() before reading a color value. \n";
            return {0, 0, 0};
        }
        
        else{
            //Create color array
            unsigned const int color_size = 3;
            std::array<uint16_t, color_size> colors = {0, 0, 0};
            
            //For each color, take the lower and upper register value, combine it and put it in the color array
            for(unsigned int i = 0; i < color_size; i++){       
                uint16_t color_value = (read_color(red_lower_register + (i * 2))) + (read_color(red_lower_register + (i * 2) + 1) << 8);
                colors[i] = color_value;
            }
            
            //Return the color array
            return colors;
        }
    }
};

#endif // APDS_9960_HPP

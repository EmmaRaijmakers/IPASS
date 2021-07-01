
//          Copyright Emma Raijmakers 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


//This file contains Doxygen lines

#ifndef SORTER_HPP
#define SORTER_HPP

//#include "color_sensor.hpp"
#include "apds_9960.hpp"

/// @file

/// \brief
/// Candy sorter class
/// \details
/// This class is used to create a candy sorter that sorts
/// candy based on their color. This is done by a color sensor
/// that is derived from the color_sensor super class. Furthermore,
/// the sorter has 2 RGB LEDs that are set to white light, in 
/// order to better illuminate the color of the candy, and a servo
/// which points a slide in the direction of the right bowl with 
/// the right color. See added picture for an overview of the machine.
class sorter
{
    
private:

    apds_9960 col_sensor;
    
    std::array<unsigned int, 6> col_array;
    
//    hwlib::port_out_from_pins_t motor_pins;
    
    hwlib::servo_background servo_pin; 
    
    hwlib::target::pin_out & rgb_leds;
    
//    //The motor is used in a different sorter design    
//    void motor_on(float rotations){
//        const unsigned int  motor_steps = 512 * rotations;
//        
//        const unsigned int motor_positions = 8;
//        std::array<int, motor_positions> pos = {8, 12, 4, 6, 2, 3, 1, 9};
//    
//        for(unsigned int j = 0; j < motor_steps; j++){
//            for(unsigned int i = 0; i < motor_positions; i++){
//                motor_pins.write(pos[i]);
//                hwlib::wait_ms(1);
//            }
//        }
//    }
    
    /// \brief
    /// Determine color function
    /// \details
    /// This function is used to change the 3 16 bit color values read from 
    /// the color sensor to an actual color. This function can differentiate
    /// between the colors red, green and blue. Red is represented by a 1,
    /// green by a 2 and blue by a 3.
     unsigned int determine_color(const std::array<uint16_t, 3> & color_input){
        //If the red value is the highest, return red
        if(color_input[0] >= color_input[1] && color_input[0] >= color_input[2]){
            return 1;
        }
        //If the green value is the highest, return green
        else if(color_input[1] > color_input[0] && color_input[1] >= color_input[2]){
            return 2;
        }
        //If the blue value is the highest, return blue
        else
        {
            return 3;
        }
    }
    
    /// \brief
    /// Determine position function
    /// \details
    /// This function is used to determine the position of the servo
    /// and which bowl the slide needs to be pointed at for the candy
    /// to end up in the right bowl. This function makes use of the
    /// result of the function determine color as its input parameter,
    /// and the positions of the colors in the color array given 
    /// as input by the user. Each of the 6 bowls is placed 20 degrees 
    /// from another, starting at 0 and ending at 100. If the color is 
    /// not found in the color array, an error message will be printed.
    unsigned int determine_position(const unsigned int & color){
        //For each bowl position in the color array
        for(unsigned int i = 0; i < 6; i++){
            //If the color of the bowl equals the color of the candy
            if(col_array[i] == color){
                //return the position of the bowl
                return i * 20;
            }
        }
        
        //If the color cannot be found in de color array, return the default position
        hwlib::cout << "Unable to find color in color array";
        return 2 * 20;
    }

    
public:

    /// \brief
    /// Sorter constructor
    /// \details
    /// This constructor uses a clock pin and a data pin in order to
    /// create a bus for I2C communication. This bus is created in the
    /// super class of the color sensor. A color array is used to determine
    /// the positions of certain colors in the bowls. To select the colors, 
    /// create an array with the color on the position you want, by using:
    /// - 0 for empty
    /// - 1 for red
    /// - 2 for green
    /// - 3 for blue    
    /// The array must be 6 long. {1, 2, 3, 0, 0, 0} gives red, green, blue,
    /// empty, empty, empty.
    /// The servo is used to move the candy to the right bowl and the LEDs
    /// are used to light the candy, so that the color sensor can make a 
    /// better evaluation of the color.
    sorter( hwlib::target::pin_oc & scl, 
            hwlib::target::pin_oc & sda, 
            std::array<unsigned int, 6> col_array, 
            hwlib::target::pin_out & servo, 
            hwlib::target::pin_out & leds ):
        col_sensor(scl, sda),
        col_array(col_array),
        servo_pin(servo),
        rgb_leds(leds)
    {}
    
    /// \brief
    /// Start candy sorter
    /// \details
    /// This function is used to start a program which will sort candy
    /// by color into the right bowl. The position of each color is 
    /// determined by the color array input by the user. The user is
    /// asked to input a candy into the machine and is given 5 seconds
    /// to do so. After 5 seconds the color is scanned, the servo is moved
    /// to the right position and the user can drop the candy down the slide.
    void start(){
        hwlib::cout << "Starting sorter \n";
        
        //Set up the color sensor
        col_sensor.setup();
        
        //Set the servo in the middle position
        servo_pin.write_angle(40);
        
        hwlib::cout << "Input candy \n";
        
        //Turn on the LEDs
        rgb_leds.write(1);
        
        //Count down from 5
        for(unsigned int i = 0; i < 5; i++){
            hwlib::cout << 5 - i << "\n";
            hwlib::wait_ms(1000);
        }
        
        //Scan the color of the candy
        std::array<uint16_t, 3> color_value = col_sensor.read();
        
        hwlib::wait_ms(1000);
        
        //Turn off the LEDs
        rgb_leds.write(0);
        
        //Print the color values and the actual color
        hwlib::cout << "red: " << color_value[0] << " ";
        hwlib::cout << "green: " << color_value[1] << " ";
        hwlib::cout << "blue: " << color_value[2] << "\n";
        
        unsigned int determined_color = determine_color(color_value);
        hwlib::cout << "Color read: "  << determined_color << "\n";
        
        //Move the servo to the correct position
        unsigned int determined_position = determine_position(determined_color);
        servo_pin.write_angle(determined_position);
        
        hwlib::wait_ms(5000);
        
    }
};

#endif // SORTER_HPP

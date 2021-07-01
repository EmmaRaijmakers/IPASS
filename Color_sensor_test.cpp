#include "sorter.hpp"

//This file is used as test code and gives examples of how the program can be used.

int main(){
    
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::d21);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::d20);
    
    //Different color positions test
    std::array<unsigned int, 6> colors = {1, 2, 3, 0, 0, 0};
//    std::array<unsigned int, 6> colors = {0, 0, 0, 0, 0, 0};
//    std::array<unsigned int, 6> colors = {0, 0, 0, 3, 2, 1};
//    std::array<unsigned int, 6> colors = {0, 2, 0, 1, 0, 0};
    
    auto servo = hwlib::target::pin_out(hwlib::target::pins::d19);
    
    auto leds = hwlib::target::pin_out(hwlib::target::pins::d18);

    hwlib::wait_ms(2000);
    
    //Sorter test
    sorter s1(scl, sda, colors, servo, leds);
    
    s1.start();
    
    
    
//    //APDS-9960 test
//    apds_9960 apds(scl, sda);
//    apds.setup();
//    std::array<uint16_t, 3> color_value = apds.read();
//        
//    hwlib::cout << "red: " << color_value[0] << " ";
//    hwlib::cout << "green: " << color_value[1] << " ";
//    hwlib::cout << "blue: " << color_value[2] << " ";
    


//    //Servo test
//    auto servo_pin = hwlib::target::pin_out( hwlib::target::pins::d19);
//    hwlib::servo_background s1(servo_pin);  
//    
//    //0 tot 110 graden gaat goed, daarna lastig -> 110 max
//    for(unsigned int i = 0; i < 6 ; i++){
//        s1.write_angle(i * 20);
//        hwlib::wait_ms(5000);
//    }


    
//    //LED test
//    while(true){
//        leds.write(1);
//        hwlib::wait_ms(1000);
//        leds.write(0);
//        hwlib::wait_ms(1000);
//    }
    
    
    
//    //Motor test
//    auto in1 = hwlib::target::pin_out(hwlib::target::pins::d8);
//    auto in2 = hwlib::target::pin_out(hwlib::target::pins::d9);
//    auto in3 = hwlib::target::pin_out(hwlib::target::pins::d10);
//    auto in4 = hwlib::target::pin_out(hwlib::target::pins::d11);
//    
//    auto motor = hwlib::port_out_from_pins_t(in1, in2, in3, in4);
//    
//    const unsigned int motor_positions = 8;
//    std::array<int, motor_positions> pos = {8, 12, 4, 6, 2, 3, 1, 9};
//    
//    while(true){
//        for(unsigned int i = 0; i < motor_positions; i++){
//            motor.write(pos[i]);
//            hwlib::wait_ms(1);
//        }
//    }
    
}
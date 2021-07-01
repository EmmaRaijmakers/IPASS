#include "sorter.hpp"

int main(){
    
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::d21);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::d20);
    
    std::array<unsigned int, 6> colors = {1, 2, 3, 0, 0, 0};
    
    auto servo = hwlib::target::pin_out(hwlib::target::pins::d19);
    
    auto leds = hwlib::target::pin_out(hwlib::target::pins::d18);

    hwlib::wait_ms(2000);
    
    sorter s1(scl, sda, colors, servo, leds);
    
    s1.start();
}
 #include "mbed.h"
 #include "bbcar.h"

 DigitalOut led1(LED1);
 PwmOut pin9(D9), pin8(D8);
 DigitalInOut pin10(D10);
 Ticker servo_ticker;
 BBCar car(pin8, pin9, servo_ticker);

 int main() {
     parallax_ping  ping1(pin10);

    double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table0[] = {-25.121, -25.041, -25.599, -25.679, -11.484, 0.000, 11.484, 25.679, 25.599, 25.041, 25.121};
    double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table1[] = {-16.189, -16.029, -15.391, -13.557, -3.908, 0.000, 3.908, 13.557, 15.391, 16.029, 16.189};
    // first and fourth argument : length of table
    car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);

    car.goStraightCalib(20);
    while(1){
        if((float)ping1>15) led1 = 1;
        else{
            led1 = 0;
            car.stop();
            break;
        }
        wait(.01);
    }
    
 }
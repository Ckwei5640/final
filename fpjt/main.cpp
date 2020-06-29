 #include "mbed.h"
 #include "bbcar.h"

 DigitalOut led1(LED1), led2(LED2), led3(LED3);
 PwmOut pin9(D9), pin8(D8);
 DigitalInOut pin10(D10);
 Serial xbee(D12, D11);
 Serial uart(D1,D0); //tx,rx
 Ticker servo_ticker;
 BBCar car(pin8, pin9, servo_ticker);
 int obj = 0;
 float dis[50] = {0};


 void object_identify(){
     if(dis[0]<dis[1] && ((dis[2]-dis[3])>10 || (dis[3]-dis[4])>10)){
         obj = 1; //cube
     }
     else if(dis[0]<dis[1] && dis[1]<dis[2]){
         obj = 2; // isosceles triangle
     }
     else if(dis[1]>dis[2] && dis[11]<dis[12]){
         obj = 3; // valley
     }
     else{
        obj = 4; //right angle triangle
     }
 }

 int main() {
    led2 = 1;
    led3 = 1;
    uart.baud(9600);
    parallax_ping  ping1(pin10);
    char recv = 'n';

    double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table0[] = {-25.121, -25.041, -25.599, -25.679, -11.484, 0.000, 11.484, 25.679, 25.599, 25.041, 25.121};
    double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table1[] = {-16.189, -16.029, -15.391, -13.557, -3.908, 0.000, 3.908, 13.557, 15.391, 16.029, 16.189};
    // first and fourth argument : length of table
    car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);

    xbee.printf("Start!!\r\n");
    car.goStraightCalib(35); //first
    led2 = 0;
    while(1){
        if((float)ping1>19) led1 = 1;
        else{
            led2 = 1;
            led1 = 0;
            car.stop();
            car.turn(50,0.0001);
            wait(3.1);
            car.stop();
            break;
        }
        wait(.01);
    }
    car.goStraightCalib(30);  //enter mission1
    led1 = 1;
    led2 = 0;
    wait(1.5);
    while(1){
        if((float)ping1>32) led1 = 1;
        else{
            led2 = 1;
            led1 = 0;
            car.stop();
            car.turn(50,-0.05);
            wait(1.35);
            car.stop();
            break;
        }
        wait(.01);
    }
    car.goStraight(-40);
    wait(1.5);
    car.stop();
    led1 = 1;
    
    xbee.printf("Mission 1\r\n");
    char s[21];
    sprintf(s,"image_classification");
    uart.puts(s);
    for(int i = 0;i < 10;i++){
        led3 = !led3;
        wait(0.1);
    }
    wait(4); //take photo
    int i = 0;
    while(recv == 'n'){
        if(uart.readable()){
            led1 = 0;
            led2 = 0;
            led3 = 0;
            recv = uart.getc();
            xbee.putc(recv);
            xbee.printf("\r\n");
            wait(1);
        }
        i++;
        wait(0.01);
        if(i == 100) break;
    }
    led3 = 1;
    led2 = 1;
    led1 = 1;

    car.goStraightCalib(30);
    led1 = 1;
    led2 = 0;
    wait(0.6);
    car.stop();
    led2 = 1;
    car.turn(50,-0.05); //right
    led1 = 0;
    wait(1.5);
    car.stop();
    car.goStraightCalib(40);
    led1 = 1;
    led2 = 0;
    wait(1);
    car.stop();
    led2 = 1;
    car.turn(50,0.0001); //left
    led1 = 0;
    wait(2.3);
    car.stop();
    car.goStraight(-35); //backward to garage
    wait(2);
    while(1){
        if((float)ping1<50) led1 = 0;
        else{
            led1 = 1;
            car.stop();
            break;
        }
        wait(.01);
    }
    wait(1);
    car.goStraightCalib(20); // leave garage
    led2 = 0;
    while(1){
        if((float)ping1>19) led1 = 1;
        else{
            led2 = 1;
            led1 = 0;
            car.stop();
            car.turn(50,-0.05);
            wait(1.1);
            car.stop();
            break;
        }
        wait(.01);
    }
    led1 = 1;
    car.goStraightCalib(20); // leave mission1
    led2 = 0;
    wait(2.4);
    car.stop();
    led2 = 1;
    car.turn(50,-0.05);
    led1 = 0;
    wait(1.3);
    car.stop();
    led1 = 1;
    car.goStraightCalib(20); // from mission1 to mission2
    led2 = 0;
    wait(2);
    while(1){
        if((float)ping1>17) led1 = 1;
        else{
            led2 = 1;
            led1 = 0;
            car.stop();
            car.turn(50,-0.05);
            wait(1);
            car.stop();
            break;
        }
        wait(.01);
    }
    car.goStraightCalib(20);
    led1 = 1;
    led2 = 0;
    wait(2.65);
    car.turn(50,-0.05);
    led2 = 1;
    led1 = 0;
    wait(1);
    car.stop();
    car.goStraightCalib(20); //mission 2
    led2 = 0;
    while(1){
        if((float)ping1>35) led1 = 1;
        else{
            led2 = 1;
            led1 = 0;
            car.stop();
            break;
        }
        wait(.01);
    }
    led1 = 1;
    xbee.printf("Mission 2\r\n");
    for(int i = 0;i < 10;i++){
        led3 = !led3;
        wait(0.1);
    }
    car.turn(50,-0.5);
    for(int i = 0;i < 5; i++){
        dis[i] = (float)ping1;
        wait(0.1);
    }
    car.stop();
    wait(1);
    car.turn(50,0.0005);
    for(int i = 5;i < 15; i++){
        dis[i] = (float)ping1;
        wait(0.1);
    }
    car.stop();
    wait(1);
    for(int i = 0;i < 15; i++){
        xbee.printf("%f \r\n",dis[i]);
    }
    car.turn(50,-0.05);
    wait(0.5);
    car.stop();
    object_identify();
    xbee.printf("%d \r\n",obj);
    wait(1);
    led3 = 1;

    car.goStraight(-40); //complete mission 2
    led1 = 0;
    wait(2);
    car.stop();
    car.turn(60,0.0001);
    wait(2.4);
    car.stop();
    car.goStraightCalib(20); //leave mission 2
    led2 = 0;
    while(1){
        if((float)ping1>18) led1 = 1;
        else{
            led2 = 1;
            led1 = 0;
            car.stop();
            car.turn(50,-0.05);
            wait(1);
            car.stop();
            break;
        }
        wait(.01);
    }
    car.goStraightCalib(20);
    xbee.printf("Finish!!\r\n");
    led1 = 1;
    led2 = 0;
 }
#include<math.h>
#include<stdio.h>
#include "pico/stdlib.h"
#include "pico-ssd1306/ssd1306.h"
#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "hardware_i2c.h"
#include "hardware_adc.h"
#include "model.h"

#define RED_LED 20
#define RELAY_PIN 21

#define I2C_PIN_SDA 16
#define I2C_PIN_SCL 17

#define screenWidth 128
#define screenHeight 64

#define drawStartX 0
#define drawStartY 0

#define batteryMinVolt 68.0
#define batteryMaxVolt 82.0

#define fori(i,n) for(int i = 0; i < n; i++)


void drawLineH(pico_ssd1306::SSD1306 thisDisplay, uint8_t startX, uint8_t startY, int16_t length, uint8_t fade){
    if(length > 0){
        for(uint8_t t = 0; t < length;){
            thisDisplay.setPixel(startX+t,startY);
            t = t + 1 + fade;
        }

    }else{
        for(uint8_t t = 0; t < length;){
            thisDisplay.setPixel(startX+t,startY);
            t = t - (1 + fade);
        }
    }
}

void drawLineV(pico_ssd1306::SSD1306 thisDisplay, uint8_t startX, uint8_t startY, int16_t length, uint8_t fade){
    if(length > 0){
        for(uint8_t t = 0; t < length;){
            thisDisplay.setPixel(startX,startY+t);
            t = t + 1 + fade;
        }

    }else{
        for(uint8_t t = 0; t < length;){
            thisDisplay.setPixel(startX,startY+t);
            t = t - (1 + fade);
        }
    }
}


void drawLine(pico_ssd1306::SSD1306 thisDisplay, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
    uint8_t x = x1;
    uint8_t y = y1;

    float slope;
    if(x1 != x2){
        slope = (y2 - y1)/(x2 - x1+0.0);
    }else{
        drawLineV(thisDisplay, x1, y1, (y2 - y1), 0);
        return;
    }

    int8_t xd = 0;  // direction of x end from start
    if(x2 > x1){
        xd = 1;
    }else if(x2 < x1){
        xd = -1;
    }

    int8_t yd = 0;  // direction of y end from start
    if(y2 > y1){
        yd = 1;
    }else if(y2 < y1){
        yd = -1;
    }


    for(int i = 0; x != x2 || y != y2; i++){
        thisDisplay.setPixel(x,y);
        float lhs = ((x2 - x)*slope);
        if(lhs < 0){ lhs = 0 - lhs; }
        float rhs = (y2 - y);
        if(rhs < 0){ rhs = 0 - rhs; }
        if((lhs) >= (rhs)){
            x += xd;
        }else{
            y+= yd;
        }
        // if(abs(x2 - x) > abs(y2 - y)){
        //     x += xd;
        // }else{
        //     y+= yd;
        // }
    }
}


int drawModel(pico_ssd1306::SSD1306 thisDisplay){
    // drawLine(display, 0, 0, 127, 0);
    fori(i, 12){
        drawLine(thisDisplay, vertices[faces[i][0]][0], vertices[faces[i][0]][1], vertices[faces[i][1]][0], vertices[faces[i][1]][1]);
        drawLine(thisDisplay, vertices[faces[i][1]][0], vertices[faces[i][1]][1], vertices[faces[i][2]][0], vertices[faces[i][2]][1]);
        drawLine(thisDisplay, vertices[faces[i][2]][0], vertices[faces[i][2]][1], vertices[faces[i][0]][0], vertices[faces[i][0]][1]);
    }
    return 0;
}



int animTest1(pico_ssd1306::SSD1306 thisDisplay){

    for(int anim = 0; anim < 50; anim++){
    // fori(anim,50){
        drawLine(thisDisplay, 10, 10, 50, anim);
        sleep_ms(100);

        thisDisplay.sendBuffer(); //Send buffer to device and show on screen
        thisDisplay.clear();
    }

    return 0;
}


int main () {

    uint poinX = 0;
    uint poinY = 0;

    int i = 0;    


    const uint LED_PIN = PICO_DEFAULT_LED_PIN;



    i2c_init(i2c0, 1000000); //Use i2c port with baud rate of 1Mhz

    gpio_init(LED_PIN);
    gpio_init(RED_LED);
    gpio_init(RELAY_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);


    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_select_input(1);
    uint16_t result = 0;

    gpio_put(RELAY_PIN, 1);

    //Set pins for I2C operation
    gpio_set_function(I2C_PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_PIN_SDA);
    gpio_pull_up(I2C_PIN_SCL);

    // Address is  0x3C for 128x64
    pico_ssd1306::SSD1306 display = pico_ssd1306::SSD1306(i2c0, 0x3C, pico_ssd1306::Size::W128xH64);

    sleep_ms(500);

    display.setOrientation(0);

    float multiplier = 0;
    float progress = 0;
    float avgFact =  0.05; // 0.05;  // 1;
    float avgProgress = 0; //  value from potentiometer
    float lastAvgProgress = 0; //  last value from potentiometer
    


    while(true){
        // animTest1(display);
        drawModel(display);



        /// retreive Potentiometer Value
        // adc_select_input(1); // confirm ADC is reading from potentiometer
        // result = adc_read();
        // adc_select_input(0);   // keep ready for next read
        // // multiplier = result*0.1;
        // // progress = result * 0.03125; // avg multiplier for X
        // // progress = result * 0.0078125; // true avg multiplier for Y
        // progress = result * 0.01; // compensated avg multiplier for Y
        // progress = (pow(progress,0.5))*20 - 13;
        // avgProgress = (avgProgress*(1-avgFact)) + (progress*avgFact);


        // // detect change in value
        // if(avgProgress > lastAvgProgress + 0.3 || avgProgress < lastAvgProgress - 0.3){
        //     settingLimitModeON = 1;
        //     modeTimer = modeAutoOFFDuration;
        // }else{
        //     if(modeTimer > 0){
        //         modeTimer = modeTimer - 1;
        //     }else{
        //         settingLimitModeON = 0;
        //     }
        // }

        // modeSwitchAnimator = settingLimitModeON*modeSwitchAnimatorSpeed + modeSwitchAnimator*(1-modeSwitchAnimatorSpeed);
        // scrollWidthRatio = 0.25 + (1 - modeSwitchAnimator)*0.5;

        // lastAvgProgress = avgProgress;







        // Read Battery Voltage
        // adc_select_input(0); // confirm battery voltage is selected
        // batteryRAW = adc_read();
        // adc_select_input(2); // keep ready for potentiometer
        // batteryVolt = batteryRAW * batteryVoltConversionFactor;
        // avgBattery = (avgBattery*(1-avgBatteryFact)) + (batteryVolt*avgBatteryFact);


        // //// Calculating Charge Percent based on voltage range.

        // if(avgBattery > 40){ // Actual Battery attached
        //     // convert to charge percentage for 72V Li-ion battery
        //     expectedRatedBatteryVolt = 72;
        //     chargePercent = pow((sin(1.54*(avgBattery - batteryMinVolt)/(batteryMaxVolt - batteryMinVolt))),2)*100;
        // }else if(avgBattery < 14 && avgBattery > 9){ // assuming 11.1V Li-ion Battery
        //     expectedRatedBatteryVolt = 11.1;
        //     chargePercent = (avgBattery - 10.2)/(12.6-10.2)*100;
        // }else{
        //     chargePercent = 0;
        // }




        // if (chargePercent <= avgProgress - 15){ // allow charging for THRESHOLD
        //     gpio_put(RELAY_PIN, 0); // because relay is active low
        //     gpio_put(LED_PIN, 1);

        // }else if(chargePercent > avgProgress + 5){  // Stop charging when charge > limit
        //     gpio_put(RELAY_PIN, 1);
        //     gpio_put(LED_PIN, 0);
        // }


        // adc_select_input(2); // confirm ammeter is selected
        // ammeterRAW = adc_read();
        // adc_select_input(1); // keep ready for potentiometer
        // ammeterAmp = ammeterRAW * ammeterAmpConversionFactor;
        // avgAmp = (avgAmp*(1-avgAmmeterFact)) + (ammeterAmp*avgAmmeterFact);



        






        // // printf("result: %d \n progress: %d \n", result, progress);
        // // printf("result: %d \n multiplier: %4.2f \n", result, multiplier);

        // poinX = 0;
        // poinY = 0;

        // if(avgProgress <= 0){
        //     avgProgress = 0;
        // }
        // if(avgProgress >= 100){
        //     avgProgress = 100;
        // }

        // if(chargePercent <= 0){
        //     chargePercent = 0;
        // }
        // if(chargePercent >= 100){
        //     chargePercent = 100;
        // }


        // int scrollerLeft = screenWidth/2 - modeSwitchAnimator*screenWidth/4;
        // /// RENDERING Battery Charge Scroller TO SCREEN BUFFER

        // for (uint8_t i = 0; i < 100; i++){
        //     startY = 16 + (i) - chargePercent;  // y coordinate on screen where it is going to render.

        //     // startY = 
            
        //     if(startY > 0 and startY < screenHeight){
        //         if(i%5 > 3){

        //             centerOffsetY = - (screenHeight/2) + startY;      // shift coordinate system from center.
        //             padding = abs(lineSize*(1 - cos(centerOffsetY/radCurv)));  // subtract curvature difference using padding
        //             if(centerOffsetY < 10 and centerOffsetY > -10) {
        //                 drawLineH(display, scrollerLeft - lineSize*0.5 + padding, startY, lineSize*scrollWidthRatio - padding*scrollWidthRatio*2, 0);
        //             }else{
        //                 drawLineH(display, scrollerLeft - lineSize*0.5 + padding, startY, lineSize*scrollWidthRatio - padding*scrollWidthRatio*2, 2);
        //             }
        //         }
        //     }
        // }


        // // Render Potentiometer scroller to Buffer

        // // if(settingLimitModeON){
        // //     drawLineH(display, 0, 0, screenWidth, 2);
        // //     drawLineH(display, 0, 31, screenWidth, 2);
        // // }

        // for (uint8_t i = 0; i < 100; i++){
        //     startY = 16 + (i) - avgProgress;  // y coordinate on screen where it is going to render.

        //     // startY = 
            
        //     if(startY > 0 and startY < screenHeight){ // clip content to screen
        //         if(i%5 > 3){

        //             centerOffsetY = - (screenHeight/2) + startY;      // shift coordinate system from center.
        //             padding = abs(lineSize*(1 - cos(centerOffsetY/radCurv)));  // subtract curvature difference using padding
        //             if(centerOffsetY < 10 and centerOffsetY > - 10) { 
        //                 drawLineH(display, (scrollerLeft + lineSize*(-0.5 + scrollWidthRatio) - padding*(-0.5 + scrollWidthRatio)*2 + 1), startY, (lineSize*(1-scrollWidthRatio) - padding), 0);
        //             }else{
        //                 drawLineH(display, (scrollerLeft + lineSize*(-0.5 + scrollWidthRatio) - padding*(-0.5 + scrollWidthRatio)*2 + 1), startY, (lineSize*(1-scrollWidthRatio) - padding), 2);
        //             }
        //         }
        //     }
        // }





        // // Printing voltage
        // int avgBatVolt = avgBattery;
        // char avgVoltStr[3];
        // sprintf(avgVoltStr, "%d", avgBatVolt);


        // int chgPrcntINT = chargePercent;
        // char chgPrcntSTR[6];
        // sprintf(chgPrcntSTR, "%d%% ", chgPrcntINT);
        
        // int voltTxtLeft = 5 - modeSwitchAnimator*48;

        // pico_ssd1306::drawText(&display, font_12x16, const_cast<const char*>(avgVoltStr), voltTxtLeft, 6);  /// , WriteMode::ADD, Rotation::deg270
        // pico_ssd1306::drawText(&display, font_8x8, const_cast<const char*>("V"), voltTxtLeft+24, 14);  /// , WriteMode::ADD, Rotation::deg270
        // pico_ssd1306::drawText(&display, font_8x8, const_cast<const char*>(chgPrcntSTR), voltTxtLeft, 24);  /// , WriteMode::ADD, Rotation::deg270


        // int avgPrINT = avgProgress;
        // char avgPrStr[5];
        // sprintf(avgPrStr, "%d%% ", avgPrINT);

        // int limitTxtLeft = 90 - modeSwitchAnimator*18;
        // // print set Limit
        // pico_ssd1306::drawText(&display, font_5x8, const_cast<const char*>("limit"), limitTxtLeft, 1+modeSwitchAnimator*5);  /// , WriteMode::ADD, Rotation::deg270
        // pico_ssd1306::drawText(&display, font_8x8, const_cast<const char*>(avgPrStr), limitTxtLeft, 12+modeSwitchAnimator*8);  /// , WriteMode::ADD, Rotation::deg270
        
        // // Printing text

        // if(avgAmp > 0.02){
        //     int avgAmpINT = avgAmp*1000;
        //     char avgAmpSTR[6];
        //     sprintf(avgAmpSTR, "%d mA", avgAmpINT);
        //     pico_ssd1306::drawText(&display, font_8x8, const_cast<const char*>(avgAmpSTR), limitTxtLeft, 24+modeSwitchAnimator*8);  /// , WriteMode::ADD, Rotation::deg270
        // }


        // // Printing Dash
        // drawLineH(display, 42 - modeSwitchAnimator*24, 15, 40, 0);
        // drawLineH(display, 42 - modeSwitchAnimator*24, 16, 40, 0);
        // // drawLineH(display, 11, 14, 64, 1);
        // // drawLineH(display, 12, 15, 64, 1);
        // // drawLineH(display, 11, 16, 64, 1);
        // // drawLineH(display, 12, 17, 64, 1);


        // // for(poinY=0; poinY<avgProgress; poinY++){
        // //     for(poinX=40; poinX < 88; poinX++){
        // //         display.setPixel(poinX, poinY);
        // //     }

        // // }


        display.sendBuffer(); //Send buffer to device and show on screen
        display.clear();




        // Animation Complete blink
        sleep_ms(5);
        
    }
}



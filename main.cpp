/*
OMER AYSAL AYTAC
BASARI TEKNOLOJI
*/

#include "mbed.h"
#include "EPD_GDE021A1.h"
#include <string>
#include <stdio.h>

#define EPD_CS       PA_15
#define EPD_DC       PB_11
#define EPD_RESET    PB_2
#define EPD_BUSY     PA_8
#define EPD_POWER    PB_10
#define EPD_SPI_MOSI PB_5
#define EPD_SPI_MISO PB_4
#define EPD_SPI_SCK  PB_3

#define TS_G3_IO2       PB_0
#define TS_G2_IO3       PA_6
#define TS_G1_IO3       PA_2
#define TS_G1_IO4       PA_3
#define TS_G2_IO4       PA_7
#define TS_G3_IO3       PB_1

EPD_GDE021A1 epd(EPD_CS, EPD_DC, EPD_RESET, EPD_BUSY, EPD_POWER, EPD_SPI_MOSI, EPD_SPI_MISO, EPD_SPI_SCK);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
InterruptIn user_button(USER_BUTTON);
InterruptIn touch(PA_2);

int score = 0;
int select = 0;
int highest = 0;
int true_box;

// DRAWING THE SELECTION MARK
static uint8_t myPic[] = {
    0x10,
    0x30,
    0x7e,
    0x30,
    0x10
};

// DRAWING BOXES
static uint8_t myPic2[] = {
    0x03, 0x00, 0x00,
    0x1f, 0x00, 0x00,
    0x7f, 0x00, 0x00,
    0xff, 0x01, 0x00,
    0xff, 0x07, 0x00,
    0xff, 0x1f, 0x00,
    0xff, 0x7f, 0x00,
    0xff, 0xff, 0x01,
    0xff, 0xff, 0x03,
    0xff, 0xff, 0x07,
    0xff, 0xff, 0x0f,
    0xff, 0xff, 0x0f,
    0xff, 0xff, 0x0f,
    0xff, 0xff, 0x07,
    0xff, 0xff, 0x03,
    0xff, 0xff, 0x01,
    0xff, 0x7f, 0x00,
    0xff, 0x1f, 0x00,
    0xff, 0x07, 0x00,
    0xff, 0x01, 0x00,
    0x7f, 0x00, 0x00,
    0x1f, 0x00, 0x00,
    0x07, 0x00, 0x00
};

// THIS FUNCTION FINDS A RANDOM NUMBER IN THE INTERVAL ACCORDING TO THE
// PARAMETERS
int random_number(int min_num, int max_num)
{

    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num) {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    } else {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

// SHOWS THE BUTTON PRESSED BY USING LED
void button_pressed()
{
    led2 = 1;
}

// THIS FUNCTION IS SETTIONG THE SCORE AND HIGHEST SCORE ACCORDING TO
// THE PLAYER'S SCORE
void score_things()
{
    switch (score) {
        case 1: {
            epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:1" , RIGHT_MODE);
            if (score >= highest) {
                highest = score;
            }
            break;
        }
        case 2: {
            epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:2" , RIGHT_MODE);
            if (score >= highest) {
                highest = score;
            }
            break;
        }
        case 3: {
            epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:3" , RIGHT_MODE);
            if (score >= highest) {
                highest = score;
            }
            break;
        }
        case 4: {
            epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:4" , RIGHT_MODE);
            if (score >= highest) {
                highest = score;
            }
            break;
        }
        case 5: {
            epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:5" , RIGHT_MODE);
            if (score >= highest) {
                highest = score;
            }
            break;
        }
        default: {
            epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:0" , RIGHT_MODE);
            break;
        }
    }
    if (highest == 0)
        epd.DisplayStringAtLine(5, (uint8_t*)"HIGHEST SCORE:0" , RIGHT_MODE);
    if (highest == 1)
        epd.DisplayStringAtLine(5, (uint8_t*)"HIGHEST SCORE:1" , RIGHT_MODE);
    if (highest == 2)
        epd.DisplayStringAtLine(5, (uint8_t*)"HIGHEST SCORE:2" , RIGHT_MODE);
    if (highest == 3)
        epd.DisplayStringAtLine(5, (uint8_t*)"HIGHEST SCORE:3" , RIGHT_MODE);
    if (highest == 4)
        epd.DisplayStringAtLine(5, (uint8_t*)"HIGHEST SCORE:4" , RIGHT_MODE);
    if (highest == 5)
        epd.DisplayStringAtLine(5, (uint8_t*)"HIGHEST SCORE:5" , RIGHT_MODE);
    wait(0.1);
}

// THIS FUNCTION DRAWS THE BODY OF THE GAME
void body()
{
    if ( score != 5 ) {
        switch (select) {
            case 0: {
                epd.DrawImage(29, 4, 8, 5, myPic);
                break;
            }
            case 1: {
                epd.DrawImage(69, 4, 8, 5, myPic);
                break;
            }
            case 2: {
                epd.DrawImage(109, 4, 8, 5, myPic);
                break;
            }
            default: {
                break;
            }
        }
        // SCORES
        score_things();
        epd.DrawImage(20, 7, 24, 23, myPic2);
        epd.DrawImage(60, 7, 24, 23, myPic2);
        epd.DrawImage(100, 7, 24, 23, myPic2);
        led1 = 1;
    } else {
        epd.DisplayStringAtLine(4, (uint8_t*)"CONGRATULATIONS" , CENTER_MODE);
        epd.DisplayStringAtLine(2, (uint8_t*)"HIGHEST SCORE:5" , CENTER_MODE);
        highest = 0;
    }
}

// THIS FUNCTION CREATES A RANDOM NUMBER AND COMPARES THE SELECTED BOX AND
// TRUE BOX
void button_released()
{

    true_box = random_number(0,2);
    led2 = 0;
    epd.Clear(EPD_COLOR_WHITE);
    select = 0;
    //SELECTION
    if (true_box == select) {
        score++;
        // SCORES
        if ( score != 5)
            score_things();
    } else {
        score = 0;
        epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:0" , RIGHT_MODE);
        score_things();
    }
    body();
    epd.RefreshDisplay();

}

// THIS FUNCTION IS CHANGING THE SELECTED BOX WITH THE LINEAR TOUCH SENSOR
void risingStar()
{
    led1 = 1;
    if (select < 2)
        select++;
    else
        select = 0;
    epd.Clear(EPD_COLOR_WHITE);
    //BOX ORDER
    body();
    epd.RefreshDisplay();
    led1 = 0;
}

int main()
{
    user_button.rise(&button_pressed);
    user_button.fall(&button_released);
    touch.rise(&risingStar);
    led1 = 0;
    led2 = 0;
    epd.Clear(EPD_COLOR_WHITE);
    epd.DisplayStringAtLine(2, (uint8_t*)"HOSGELDINIZ", CENTER_MODE);
    epd.DisplayStringAtLine(3, (uint8_t*)"OYUNUNA", CENTER_MODE);
    epd.DisplayStringAtLine(4, (uint8_t*)"BUL KARAYI AL PARAYI", CENTER_MODE);
    epd.RefreshDisplay();
    wait(4);
    epd.Clear(EPD_COLOR_WHITE);
    //epd.DisplayStringAtLine(2, (uint8_t*)"OMER", CENTER_MODE);
    //epd.DisplayStringAtLine(1, (uint8_t*)"AYSAL AYTAC", CENTER_MODE);
    epd.DisplayStringAtLine(0, (uint8_t*)"SCORE:0" , RIGHT_MODE);
    epd.DisplayStringAtLine(5, (uint8_t*)"HIGHEST SCORE:0" , RIGHT_MODE);

    epd.DrawImage(20, 7, 24, 23, myPic2);
    epd.DrawImage(60, 7, 24, 23, myPic2);
    epd.DrawImage(100, 7, 24, 23, myPic2);
    epd.DrawImage(29, 4, 8, 5, myPic);
    //epd.DrawRect(50, 4, 60, 10);
    epd.RefreshDisplay();

    while(1) {
        led1 = 0;
    }
}

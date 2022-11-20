#include "pxt.h"
#include <vector>
using namespace pxt;

#ifndef PXT_CREATE_BUFFER
#define PXT_CREATE_BUFFER(data, len) ManagedBuffer(data, len).leakData()
#endif

#define DC uBit.io.P12   // MICROBIT_PIN_P12
#define CS uBit.io.P8    // MICROBIT_PIN_P8
#define RESET uBit.io.P2 // MICROBIT_PIN_P2
#define BUSY uBit.io.P16 // MICROBIT_PIN_P16
#if 0
#define DRIVER_CONTROL 0x01
#define GATE_VOLTAGE 0x03
#define SOURCE_VOLTAGE 0x04
#define DISPLAY_CONTROL 0x07
#define NON_OVERLAP 0x0B
#define BOOSTER_SOFT_START 0x0C
#define GATE_SCAN_START 0x0F
#define DEEP_SLEEP 0x10
#define DATA_MODE 0x11
#define SW_RESET 0x12
#define TEMP_WRITE 0x1A
#define TEMP_READ 0x1B
#define TEMP_CONTROL 0x1C
#define TEMP_LOAD 0x1D
#define MASTER_ACTIVATE 0x20
#define DISP_CTRL1 0x21
#define DISP_CTRL2 0x22
#define WRITE_RAM 0x24
#define WRITE_ALTRAM 0x26
#define READ_RAM 0x25
#define VCOM_SENSE 0x28
#define VCOM_DURATION 0x29
#define WRITE_VCOM 0x2C
#define READ_OTP 0x2D
#define WRITE_LUT 0x32
#define WRITE_DUMMY 0x3A
#define WRITE_GATELINE 0x3B
#define WRITE_BORDER 0x3C
#define SET_RAMXPOS 0x44
#define SET_RAMYPOS 0x45
#define SET_RAMXCOUNT 0x4E
#define SET_RAMYCOUNT 0x4F
#endif

#define CS_ACTIVE 0
#define CS_INACTIVE 1
#define DC_DATA 1
#define DC_COMMAND 0

#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define LUT_FOR_VCOM                                0x20 
#define LUT_WHITE_TO_WHITE                          0x21
#define LUT_BLACK_TO_WHITE                          0x22
#define LUT_WHITE_TO_BLACK                          0x23
#define LUT_BLACK_TO_BLACK                          0x24
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_SENSOR_COMMAND                  0x40
#define TEMPERATURE_SENSOR_SELECTION                0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define RESOLUTION_SETTING                          0x61
#define GSST_SETTING                                0x65
#define GET_STATUS                                  0x71
#define AUTO_MEASUREMENT_VCOM                       0x80
#define READ_VCOM_VALUE                             0x81
#define VCM_DC_SETTING                              0x82
#define PARTIAL_WINDOW                              0x90
#define PARTIAL_IN                                  0x91
#define PARTIAL_OUT                                 0x92
#define PROGRAM_MODE                                0xA0
#define ACTIVE_PROGRAMMING                          0xA1
#define READ_OTP                                    0xA2
#define POWER_SAVING                                0xE3

const unsigned char lut_vcom0[] =
{
  0x00, 0x08, 0x08, 0x00, 0x00, 0x02,  
  0x00, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x00, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00,   

};
const unsigned char lut_ww[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};
const unsigned char lut_bw[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      
};

const unsigned char lut_bb[] ={
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
             
};

const unsigned char lut_wb[] ={
  0x20, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x10, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            
};

constexpr uint8_t WIDTH = 400;
constexpr uint8_t HEIGHT = 300;

constexpr uint8_t COLS = 300;
constexpr uint8_t ROWS = 400;
constexpr uint8_t OFFSET_X = 0;
constexpr uint8_t OFFSET_Y = 6;

uint8_t *buf_b;
//uint8_t *buf_r;

SPI spi(MOSI, MISO, SCK);

bool initialized = false;


namespace inkybit {
    void busyWait() {
        while(BUSY.getDigitalValue()) {
            uBit.sleep(50);
        }
    }

    void spiCommand(uint8_t command, const uint8_t *data, int len) {
        CS.setDigitalValue(CS_ACTIVE);
        DC.setDigitalValue(DC_COMMAND);
        spi.write(command);
        if (len > 0) {
            DC.setDigitalValue(DC_DATA);
            for(auto x = 0; x < len; x++){
                spi.write(data[x]);
            }
        }
        CS.setDigitalValue(CS_INACTIVE);
    }

    void spiCommand(uint8_t command) {
        spiCommand(command, NULL, 0);
    }

    void spiCommand(uint8_t command, std::initializer_list<uint8_t> data) {
        CS.setDigitalValue(CS_ACTIVE);
        DC.setDigitalValue(DC_COMMAND);
        spi.write(command);
        DC.setDigitalValue(DC_DATA);
        for(auto c : data){
            spi.write(c);
        }
        CS.setDigitalValue(CS_INACTIVE);
    }

    void spiData(uint8_t *data, int len) {
        CS.setDigitalValue(CS_ACTIVE);
        DC.setDigitalValue(DC_DATA);
	    for(auto x = 0; x < len; x++){
            spi.write(data[x]);
	    }
        CS.setDigitalValue(CS_INACTIVE);
    }
#if 0
    void clear(void) {
        spiCommand(RESOLUTION_SETTING);
        spiData(COLS >> 8, 1);
        spiData(COLS & 0xff, 1);
        spiData(ROWS >> 8, 1);        
        spiData(ROWS & 0xff, 1);

        spiCommand(DATA_START_TRANSMISSION_1);           
        uBit.sleep(2);
        spiData(0xFF, COLS / 8 * ROWS);  
        uBit.sleep(2);

        spiCommand(DATA_START_TRANSMISSION_2);           
        uBit.sleep(2);
        spiData(0xFF, COLS / 8 * ROWS);  
        uBit.sleep(2);
    }
#else
    //%
    void clear() {
        memset(buf_b, 0xff, (COLS / 8) * ROWS);
        //memset(buf_r, 0x00, (COLS / 8) * ROWS);
    }
#endif
    //%
    void setPixel(int x, int y, int color) {
        if(x >= WIDTH) return;
        if(y >= HEIGHT) return;
        y += OFFSET_Y;
        y = COLS - 1 - y;
        uint8_t shift = 7 - (y % 8);
        y /= 8;
        uint16_t offset = (x * (COLS / 8)) + y;

        uint8_t byte_b = buf_b[offset] | (0b1 << shift);
        //uint8_t byte_r = buf_r[offset] & ~(0b1 << shift);

        //if(color == 2) {
        //    byte_r |= 0b1 << shift;
        //}
        if(color == 1) {
            byte_b &= ~(0b1 << shift);
        }

        buf_b[offset] = byte_b;
        //buf_r[offset] = byte_r;
    }
    
    void setlut(void) {
        unsigned int count;     

        spiCommand(LUT_FOR_VCOM);                            //vcom
        spiData(lut_vcom0, 44);
        
        spiCommand(LUT_WHITE_TO_WHITE);                      //ww --
        spiData(lut_ww, 42); 
        
        spiCommand(LUT_BLACK_TO_WHITE);                      //bw r
        spiData(lut_bw, 42);

        spiCommand(LUT_WHITE_TO_BLACK);                      //wb w
        spiData(lut_bb, 42);

        spiCommand(LUT_BLACK_TO_BLACK);                      //bb b
        spiData(lut_wb, 42);
    }

    //%
    void show() {
        spiCommand(RESOLUTION_SETTING);
        spiData(COLS >> 8, 1);        
        spiData(COLS & 0xff, 1);
        spiData(ROWS >> 8, 1);
        spiData(ROWS & 0xff, 1);

        spiCommand(VCM_DC_SETTING);
        spiData(0x12, 1);                   

        spiCommand(VCOM_AND_DATA_INTERVAL_SETTING);
        spiCommand(0x97);    //VBDF 17|D7 VBDW 97  VBDB 57  VBDF F7  VBDW 77  VBDB 37  VBDR B7

        if (buf_b != NULL) {
            spiCommand(DATA_START_TRANSMISSION_1);
            spiData(0xFF, COLS / 8 * ROWS);      // bit set: white, bit reset: black
            uBit.sleep(2);
            
            spiCommand(DATA_START_TRANSMISSION_2); 
            spiData(buf_b, COLS / 8 * ROWS);
            uBit.sleep(2);                 
        }

        //setlut();
        spiCommand(DISPLAY_REFRESH); 
        uBit.sleep(100);
        busyWait();
    }

    void reset() {
        RESET.setDigitalValue(0);
        uBit.sleep(2);
        RESET.setDigitalValue(1);
        uBit.sleep(20); 

        RESET.setDigitalValue(0);
        uBit.sleep(2);
        RESET.setDigitalValue(1);
        uBit.sleep(20); 

        RESET.setDigitalValue(0);
        uBit.sleep(2);
        RESET.setDigitalValue(1);
        uBit.sleep(20); 
    }

    //%
    void init() {
        if(initialized) return;

        buf_b = (uint8_t *)malloc((COLS / 8) * ROWS);
        //buf_r = (uint8_t *)malloc((COLS / 8) * ROWS);
        clear();

        /* EPD hardware init start */
        reset();
        spiCommand(0x01);
        spiData(0x03, 1);                   // VDS_EN, VDG_EN
        spiData(0x00, 1);                   // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
        spiData(0x2b, 1);                   // VDH
        spiData(0x2b, 1);                   // VDL

        spiCommand(0x06);
        spiData(0x17, 1);
        spiData(0x17, 1);
        spiData(0x17, 1);                   //07 0f 17 1f 27 2F 37 2f
        spiCommand(0x04);
        busyWait();
        spiCommand(0x00);
        spiData(0xbf, 1);                   // KW-BF   KWR-AF  BWROTP 0f

        spiCommand(0x30);
        spiData(0x3c, 1);                   // 3A 100HZ   29 150Hz 39 200HZ  31 171HZ

        spiCommand(0x61);                   // resolution setting
        spiData(0x01, 1);
        spiData(0x90, 1);                   //128
        spiData(0x01, 1);                   //
        spiData(0x2c, 1);

        spiCommand(0x82);                   // vcom_DC setting
        spiData(0x12, 1);

        spiCommand(0X50);                   // VCOM AND DATA INTERVAL SETTING
        spiData(0x97, 1);                   // 97white border 77black border    VBDF 17|D7 VBDW 97 VBDB 57    VBDF F7 VBDW 77 VBDB 37  VBDR B7

        setlut();

        initialized = true;
    }

}

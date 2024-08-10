#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h> 

#include "DisplayHandler.h"

/*Private Variables*/ 

//tasks
TaskHandle_t *HandlerDisplay, *HandlerIncrement;

//eSPI
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

//LVGL
// Screen Resolution
static const uint32_t screenWidth  = 482;
static const uint32_t screenHeight = 320;
// LVGL related stuff
static lv_disp_draw_buf_t draw_buf;
// Declare a buffer for 1/10 screen size
static lv_color_t buf[ screenWidth * 10 ];






// Private Prototypes
static void Display_Flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );
static void Touch_Read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data );

void lv_example_get_started_1(void);

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        if(cnt == 10){
            cnt=0;
            lv_obj_t *act_scr = lv_scr_act(); 
            lv_obj_clean( act_scr );
            delay(1000);
            lv_example_get_started_1();
        }
        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);
}

// Tasks
static void DisplayTask(void *parameter)
{
    while (1)
    {
        //lv_obj_t *act_scr = lv_scr_act(); 
        //lv_obj_clean( act_scr );    
        
        delay(1000);
    }
    vTaskDelete(HandlerDisplay);
}

static void IncrementTask(void *parameter)
{
    while (1)
    {
        lv_timer_handler();
        delay(5);
    }
    vTaskDelete(HandlerIncrement);
}


// Functions
void InitDisplay(void)
{
    //initialize LVGL
    lv_init();

    // Initialize the display buffer
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

    // Initialize the display
    static lv_disp_drv_t disp_drv;    /* Descriptor of a display driver */
    lv_disp_drv_init( &disp_drv );    /* Basic Inialization */
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;   /* Set the horizonral resolution of the display */
    disp_drv.ver_res = screenHeight;  /* Set the vertical resolution of the display */
    disp_drv.flush_cb = Display_Flush;/* driver function to flush the display */
    disp_drv.draw_buf = &draw_buf;    /* Assign the buffer to the display */
    lv_disp_drv_register( &disp_drv );/* Finally register the driver */

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = Touch_Read;
    lv_indev_drv_register( &indev_drv );

    // Initialize TFT
    tft.init();
    // Set Orientation to Landscape
    tft.setRotation(1);

    // Calibration Values for my Setup
    uint16_t calData[5] = { 318, 3464, 369, 3243, 7 };
    tft.setTouch(calData);

    // Clear the screen
    tft.fillScreen(TFT_BLACK);


    lv_example_get_started_1();

    xTaskCreate(IncrementTask, "IncrementTask", 10000, NULL, 1, HandlerIncrement);
    xTaskCreate(DisplayTask, "DisplayTask", 10000, NULL, 1, HandlerDisplay);
    
}

static void Display_Flush(  lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );
  
  tft.startWrite();
  tft.setAddrWindow( area->x1, area->y1, w, h );
  tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  tft.endWrite();
  
  lv_disp_flush_ready( disp );
}

static void Touch_Read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
  uint16_t touchX, touchY;

  bool touched = tft.getTouch( &touchX, &touchY, 600 );

  if( !touched )
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
    // uncomment to debug touch points
    Serial.print( "Data x " );
    Serial.println( touchX );
    Serial.print( "Data y " );
    Serial.println( touchY );
  }
}


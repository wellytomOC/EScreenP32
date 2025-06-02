#include <lvgl.h>
#include <SPI.h>
#include <TFT_eSPI.h> 
#include "main.h"

//LVGL variables
const int32_t hor_res = 480;
const int32_t ver_res = 320;
TaskHandle_t HandlerLvgl;
lv_display_t *display1;
lv_indev_t * indev;

//tft variables
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

//prototypes
static uint32_t my_tick_get_cb(void);
void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map);
void my_input_read(lv_indev_t * indev, lv_indev_data_t * data);
static void btn_event_cb(lv_event_t * e);



/*************TASKS*************/
void LvglLoop(void *parameter) {
    while (1) {
        lv_timer_handler();  // Call LVGL task handler
        delay(5);           // Delay to allow other tasks to run
    }
    vTaskDelete(HandlerLvgl);
}





/*************FUNCTIONS*************/
// Function to initialize the display
void InitDisplay() {
    // Initialize TFT
    tft.init();
    tft.setRotation(1);  // Landscape mode

    //configure touch   
    uint16_t calData[5] = { 318, 3464, 369, 3243, 7 };
    tft.setTouch(calData);

    // Clear screen
    tft.fillScreen(TFT_BLACK);  




    // Initialize LVGL
    lv_init();

    // Create a display object for LVGL
    display1 = lv_display_create(hor_res, ver_res); 

    //Declare buffer for 1/10 screen size; BYTES_PER_PIXEL will be 2 for RGB565. 
    #define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565))
    static uint8_t buf1[hor_res * ver_res / 10 * BYTES_PER_PIXEL];

    // Set display buffer for display `display1`.
    lv_display_set_buffers(display1, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Set the flush callback function for LVGL
    lv_display_set_flush_cb(display1, my_flush_cb);

    //set tick callback function
    lv_tick_set_cb(my_tick_get_cb);

    // Set up touch input device
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_input_read);




    //debug

    // Create a button on the active screen
    lv_obj_t *btn = lv_btn_create(lv_screen_active());
    // Center the button
    lv_obj_center(btn);
    // Set button size
    lv_obj_set_size(btn, 120, 50);

    // Create a label on the button
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click me");
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);





    xTaskCreate(LvglLoop, "LvglLoop", 8192, NULL, 1, &HandlerLvgl);

}

//function to get the current tick count
static uint32_t my_tick_get_cb(void) { 
    return millis(); 
}

// flush callback function
void my_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    uint16_t *color_p = (uint16_t *)px_map;

    int32_t x1 = area->x1;
    int32_t y1 = area->y1;
    int32_t w  = area->x2 - area->x1 + 1;
    int32_t h  = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(x1, y1, w, h);
    tft.pushPixels(color_p, w * h); // TFT_eSPI handles 16-bit to 18-bit
    tft.endWrite();

    lv_display_flush_ready(display); // Tell LVGL flush is done
}

void my_input_read(lv_indev_t * indev, lv_indev_data_t * data){
    // Example with TFT_eSPI touch
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);

    if(touched) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}




/*************EVENTS CALLBACK*************/

static void btn_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        Serial.println("Button clicked!");
    }
}
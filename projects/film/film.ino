#include "unihiker_k10.h"
#include <HTTPClient.h>
#include <WiFi.h>
// #include "USB.h"
// #include "USBMSC.h"

#define BLOCK_SIZE 512

// Create UNIHIKER K10 instance
UNIHIKER_K10 board;
Music music;

// USB Mass Storage object
// USBMSC MSC;

// LVGL
extern SemaphoreHandle_t xLvglMutex;
extern SemaphoreHandle_t xSPIlMutex;

// Font
extern const lv_font_t pixelify;

//
extern char originalImagePath[100];
extern char generatedImagePath[100];

// Common object
lv_obj_t *cameraObject = NULL;
lv_obj_t *label = NULL;
lv_obj_t *container = NULL;
lv_obj_t *images[36];

// Program State
enum class State
{
    NONE,
    BEGIN,
    HOME,
    SHOW_CAMERA_FEED,
    LOADING,
    SHOW_RESPONSE
};

State currentState = State::BEGIN;
State previousState = State::NONE;

void buttonBPressedHandler()
{
    currentState = State::LOADING;
    music.recordSaveToTFCard("S:/input.wav", 5);
}

void buttonBUnPressedHandler()
{
    processAudioWithServer("S:/input.wav");
    currentState = State::SHOW_RESPONSE;
}

// static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
//     xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
//     int32_t result = SD.writeRAW(buffer, lba) ? bufsize : -1;
//     xSemaphoreGive(xSPIlMutex);
//     return result;
// }

// static int32_t onRead(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
//     xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
//     int32_t result = SD.readRAW((uint8_t*)buffer, lba) ? bufsize : -1;
//     xSemaphoreGive(xSPIlMutex);
//     return result;
// }

// static bool onStartStop(uint8_t power_condition, bool start, bool load_eject) {
//   return true;
// }

void setup()
{
    // Setup Serial
    Serial.begin(115200);

    // Initialize screen, setup lvgl semaphore
    board.begin();
    board.initScreen();

    // Make screen go black
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);

    // Make a centered text on screen
    label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello World!");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &pixelify, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // Init SD File System
    board.initSDFile();

    // Initialize Camera Display Task
    initCamera();

    // Init gui container of photos
    setupHomeScreen();

    // Set button callback
    board.buttonA->setPressedCallback(shutterHandler);
    board.buttonB->setPressedCallback(buttonBPressedHandler);
    board.buttonB->setUnPressedCallback(buttonBUnPressedHandler);

    // Start shake detection
    start_shake_detection();

    // Set up wifi
    connectToWiFi();

    // Set up rgb control task
    xTaskCreatePinnedToCore(&rgbControlTask, "RGB Control Task", 4 * 1024, NULL, 4, NULL, 1); // Create a camera display task, task level 5 (general level)

    // Start tinyusb
    // MSC.vendorID("ESP32");
    // MSC.productID("SD_Card");
    // MSC.productRevision("1.0");
    // MSC.onStartStop(onStartStop);
    // MSC.onRead(onRead);
    // MSC.onWrite(onWrite);
    // MSC.mediaPresent(true);
    // MSC.begin(SD.numSectors(), SD.sectorSize());

    // USB.begin();
}

void loop()
{
    // Handle state transitions
    if (currentState != previousState)
    {
        xSemaphoreTake(xLvglMutex, portMAX_DELAY);
        switch (currentState)
        {
        case State::BEGIN:
            sBeginScreen();
            break;
        case State::HOME:
            sHomeScreen();
            break;
        case State::SHOW_CAMERA_FEED:
            sShowCameraFeed();
            break;
        case State::LOADING:
            sLoadingScreen();
            break;
        case State::SHOW_RESPONSE:
            sShowResponseScreen();
            break;
        default:
            break;
        }
        previousState = currentState;
        xSemaphoreGive(xLvglMutex);
    }

    // Handle LVGL tasks
    xSemaphoreTake(xLvglMutex, portMAX_DELAY);
    lv_task_handler();
    xSemaphoreGive(xLvglMutex);
}

void setupHomeScreen()
{
    container = lv_obj_create(lv_scr_act());

    // Set size to whole screen
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));

    // Make screen go black
    lv_obj_set_style_bg_color(container, lv_color_black(), LV_PART_MAIN);

    // Make it unscrollable
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    // Set flex layout with row wrap
    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);

    // Hide border
    lv_obj_set_style_border_width(container, 0, 0);

    // Set padding: y=30px, x=22px
    lv_obj_set_style_pad_top(container, 30, 0);
    lv_obj_set_style_pad_bottom(container, 30, 0);
    lv_obj_set_style_pad_left(container, 22, 0);
    lv_obj_set_style_pad_right(container, 22, 0);

    // Set gap between items to 4px
    lv_obj_set_style_pad_row(container, 6, 0);    // Gap between rows
    lv_obj_set_style_pad_column(container, 4, 0); // Gap between columns

    // Optional: Set alignment (adjust as needed)
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    // Create 36 images with click handling
    for (int i = 0; i < 36; i++)
    {
        images[i] = lv_img_create(container);

        // Set initial state (unfilled)
        lv_img_set_src(images[i], "S:/film/unfilled_photo.jpg");
    }

    // Hide Element
    lv_obj_set_style_opa(container, 0, LV_PART_MAIN);
}

// Screen Switch Function
void sBeginScreen()
{
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    // lv_label_set_text(label, "Hello World");
    lv_img_set_src(cameraObject, "S:/film/begin.jpg");
}

void sHomeScreen()
{
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_opa(cameraObject, 0, LV_PART_MAIN);

    // Get next photo number
    int photo_count = getNextPhotoNumber() - 1;

    // Refresh
    for (int i = 0; i < 36; i++)
    {
        if (i < photo_count)
        {
            // Show filled photo for taken photos
            lv_img_set_src(images[i], "S:/film/filled_photo.jpg");
        }
        else
        {
            // Show unfilled photo for remaining slots
            lv_img_set_src(images[i], "S:/film/unfilled_photo.jpg");
        }
    }

    // Loop through
    lv_obj_set_style_opa(container, 255, LV_PART_MAIN);
}

void sShowCameraFeed()
{
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_opa(container, 0, LV_PART_MAIN);
    lv_obj_set_style_opa(cameraObject, 255, LV_PART_MAIN);
}

void sLoadingScreen()
{
    sHomeScreen();
}

void sShowResponseScreen()
{
    lv_obj_set_style_opa(container, 0, LV_PART_MAIN);
    lv_img_set_src(cameraObject, originalImagePath);
    lv_obj_set_style_opa(cameraObject, 255, LV_PART_MAIN);
}

void rgbControlTask(void *parameter)
{
    uint32_t color = 0;
    uint8_t brightness = 0;

    while (1)
    {
        switch (currentState)
        {
        case State::LOADING:
            static uint16_t t = 0;

            for (int i = 0; i < 3; i++)
            {
                // Offset each LED's phase a bit for more organic feel
                float phase = t / 25.0f + i * 2.0f;
                // Sine value in range [0, 1]
                float v = (sinf(phase) + 1.0f) / 2.0f;

                // Map to hue range, e.g. around a base hue with mild variation
                uint16_t hue = (uint16_t)(30000 + v * 20000); // soft purples/blues/greens
                color = board.rgb->ColorHSV(hue, 200, 100);   // toned down for mystery

                board.rgb->write(i, color);
            }

            t++;
            break;
        default:
            board.rgb->write(-1, 0);
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Fixed delay
    }
}
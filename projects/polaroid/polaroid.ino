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

// Program State
enum class State
{
    NONE,
    BEGIN,
    SHOW_CAMERA_FEED,
    LOADING,
    SHAKE,
    SHOW_GEN_AI_IMAGE,
    SHOW_ORIGINAL_IMAGE
};

State currentState = State::BEGIN;
State previousState = State::NONE;

void buttonBPressedHandler()
{
    if (currentState == State::SHOW_GEN_AI_IMAGE)
    {
        currentState = State::SHOW_ORIGINAL_IMAGE;
    }
}

void buttonBUnPressedHandler()
{
    if (currentState == State::SHOW_ORIGINAL_IMAGE)
    {
        currentState = State::SHOW_GEN_AI_IMAGE;
    }
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
        case State::LOADING:
            sLoadingScreen();
            break;
        case State::SHOW_CAMERA_FEED:
            break;
        case State::SHAKE:
            sShakeScreen();
            break;
        case State::SHOW_GEN_AI_IMAGE:
            sGenImageScreen();
            break;
        case State::SHOW_ORIGINAL_IMAGE:
            sOriginalImageScreen();
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

// Screen Setup Function
void sBeginScreen()
{
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_img_set_src(cameraObject, "S:/polaroid/begin.jpg");
}

void sLoadingScreen()
{
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_img_set_src(cameraObject, "S:/polaroid/loading.jpg");

    // Set sound
    // music.playTFCardAudio("S:/loading.wav");
}

void sShakeScreen()
{
    // lv_label_set_text(label, "[Shake to reveal]");
    // lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    // lv_obj_set_style_opa(cameraObject, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_img_set_src(cameraObject, "S:/polaroid/shake.jpg");

    // Set sound
    // music.stopPlayAudio();
}

void sGenImageScreen()
{
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_img_set_src(cameraObject, generatedImagePath);
}

void sOriginalImageScreen()
{
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_img_set_src(cameraObject, originalImagePath);
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

        case State::SHAKE:
            static uint16_t tick = 0;

            brightness = 0;
            if (tick % 40 < 5)
            {
                brightness = 255; // Short bright burst
            }
            else if (tick % 40 < 10)
            {
                brightness = 100; // Quick fade
            }
            else
            {
                brightness = 20; // Low glow baseline
            }

            color = board.rgb->ColorHSV(0, 0, brightness);
            for (int i = 0; i < 3; i++)
            {
                board.rgb->write(i, color);
            }

            tick++;
            break;
        default:
            board.rgb->write(-1, 0);
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Fixed delay
    }
}
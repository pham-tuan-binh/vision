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

// Common object
lv_obj_t *cameraObject = NULL;
lv_obj_t *questionLabel = NULL;

// Program State
enum class State {
    NONE,
    BEGIN,
    SHOW_OTHER_PICTURE,
    SHOW_CAMERA_FEED
};

// Map
uint8_t currentPhoto = 0;
uint8_t numPhotos = 0;
String photos[20];

State currentState = State::BEGIN;
State previousState = State::NONE;

void buttonBPressedHandler() {
    Serial.println("Button B pressed");
    if (currentState == State::SHOW_OTHER_PICTURE) {
        currentState = State::BEGIN;
        return;
    }
}

void buttonBUnPressedHandler() {
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

    // Init SD File System
    board.initSDFile();

    // Set name label
    questionLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(questionLabel, "Do you remember the first time you bring Enzo home?");
    lv_obj_set_style_text_color(questionLabel, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(questionLabel, &pixelify, LV_PART_MAIN);
    lv_obj_set_width(questionLabel, 200);
    lv_obj_align(questionLabel, LV_ALIGN_BOTTOM_LEFT, 20, -20);

    initCamera();

    // Set button callback
    board.buttonA->setPressedCallback(shutterHandler);
    board.buttonB->setPressedCallback(buttonBPressedHandler);
    board.buttonB->setUnPressedCallback(buttonBUnPressedHandler);
}

void loop()
{
    // Handle state transitions
    if (currentState != previousState) {
        xSemaphoreTake(xLvglMutex, portMAX_DELAY);
        switch(currentState) {
            case State::BEGIN:
                sShowBegin();
                break;
            case State::SHOW_OTHER_PICTURE:
                sShowOtherImageScreen();
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
void sShowBegin() {
    lv_obj_set_style_opa(questionLabel, 255, LV_STATE_DEFAULT);
    lv_obj_set_style_opa(cameraObject, 0, LV_STATE_DEFAULT);
}

void sShowOtherImageScreen() {
    // Create directory if not exists
    createDirectoryIfNotExists("S:/memory-cue");

    // Go into dir
    uint8_t photoCount = 0;
    lv_fs_dir_t dir;
    lv_fs_res_t res;

    // Open the root directory
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    res = lv_fs_dir_open(&dir, "S:/memory-cue");
    xSemaphoreGive(xSPIlMutex);

    if (res != LV_FS_RES_OK)
    {
        printf("Failed to open directory\n");
        return; // Start from 1 if can't read directory
    }

    char fn[256];
    while (true)
    {
        xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
        res = lv_fs_dir_read(&dir, fn);
        xSemaphoreGive(xSPIlMutex);

        if (res != LV_FS_RES_OK || fn[0] == '\0')
        {
            break; // No more entries
        }

        if (strncmp(fn, "._", 2) != 0) {
            Serial.print(fn);
            photos[photoCount] = fn;
            photoCount++;
        }

    }

    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    lv_fs_dir_close(&dir);
    xSemaphoreGive(xSPIlMutex);

    numPhotos = photoCount;
    currentPhoto = 0;


    // Show camera object
    char fullPath[100];
    sprintf(fullPath, "S:/memory-cue/%s", photos[currentPhoto].c_str());


    Serial.println(fullPath);
    lv_img_set_src(cameraObject, fullPath);
    lv_obj_set_style_opa(cameraObject, 255, LV_STATE_DEFAULT);
    lv_obj_set_style_opa(questionLabel, 0, LV_STATE_DEFAULT);
}

// Show next image function
void showNextImage() {
    if (currentPhoto == numPhotos - 1) {
        currentPhoto = 0;
    }
    else {
        currentPhoto++;
    }
    
    Serial.println(currentPhoto);
    char fullPath[100];
    sprintf(fullPath, "S:/memory-cue/%s", photos[currentPhoto].c_str());
    Serial.println(fullPath);
    lv_img_set_src(cameraObject, fullPath);
}
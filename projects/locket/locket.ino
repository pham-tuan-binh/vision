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
lv_obj_t *dateLabel = NULL;
lv_obj_t *nameLabel = NULL;

// Program State
enum class State {
    NONE,
    SHOW_CAMERA_FEED,
    SHOW_TAKEN_PICTURE,
    SHOW_OTHER_PICTURE
};

// Map
uint8_t currentPhoto = 0;
uint8_t numPhotos = 0;
String photos[20];

State currentState = State::SHOW_CAMERA_FEED;
State previousState = State::NONE;

void buttonBPressedHandler() {
    if (currentState == State::SHOW_TAKEN_PICTURE) {
        currentState = State::SHOW_CAMERA_FEED;
    }

    if (currentState == State::SHOW_OTHER_PICTURE) {
        currentState = State::SHOW_CAMERA_FEED;
    }
}

void buttonBUnPressedHandler() {
}

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

    // Initialize Camera Display Task
    initCamera();

    // Set name label
    nameLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(nameLabel, "Binh");
    lv_obj_set_style_text_color(nameLabel, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(nameLabel, &pixelify, LV_PART_MAIN);
    lv_obj_align(nameLabel, LV_ALIGN_BOTTOM_LEFT, 20, -20);

    // Set date label
    dateLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(dateLabel, "15/07/2025");
    lv_obj_set_style_text_color(dateLabel, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(dateLabel, &pixelify, LV_PART_MAIN);
    lv_obj_align(dateLabel, LV_ALIGN_TOP_LEFT, 20, 20);

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
            case State::SHOW_CAMERA_FEED:
                break;
            case State::SHOW_TAKEN_PICTURE:
                sShowTakenImageScreen();
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
void sShowTakenImageScreen() {
    lv_img_set_src(cameraObject, "S:/locket/Binh.jpg");
}

void sShowOtherImageScreen() {
    // Create directory if not exists
    createDirectoryIfNotExists("S:/locket");

    // Go into dir
    uint8_t photoCount = 0;
    lv_fs_dir_t dir;
    lv_fs_res_t res;

    // Open the root directory
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    res = lv_fs_dir_open(&dir, "S:/locket");
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
    sprintf(fullPath, "S:/locket/%s", photos[currentPhoto].c_str());
    lv_img_set_src(cameraObject, fullPath);
    
    int dotIndex = photos[currentPhoto].lastIndexOf('.');
    String nameOnly = photos[currentPhoto].substring(0, dotIndex);  // "taken1"
    lv_label_set_text(nameLabel, nameOnly.c_str());
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
    sprintf(fullPath, "S:/locket/%s", photos[currentPhoto].c_str());
    lv_img_set_src(cameraObject, fullPath);

    int dotIndex = photos[currentPhoto].lastIndexOf('.');
    String nameOnly = photos[currentPhoto].substring(0, dotIndex);  // "taken1"
    lv_label_set_text(nameLabel, nameOnly.c_str());
}
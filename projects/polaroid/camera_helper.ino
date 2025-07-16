// LVGL
extern SemaphoreHandle_t xLvglMutex;
extern SemaphoreHandle_t xSPIlMutex;

// Camera Setup
QueueHandle_t xQueueCamera = NULL;
bool showCameraFeed = false;
TaskHandle_t displayCameraTaskHandle = NULL;

// Latest Image Path (Created)
char originalImagePath[100];
char generatedImagePath[100];

// ------------ Background Camera Tasks ------------
void initCamera()
{
    cameraObject = lv_img_create(lv_scr_act());
    xSemaphoreTake(xLvglMutex, portMAX_DELAY);

    // Set the position and size of the image display.
    lv_obj_set_pos(cameraObject, 0, 0);
    lv_obj_set_size(cameraObject, 240, 320);
    lv_obj_clear_flag(cameraObject, LV_OBJ_FLAG_SCROLLABLE);
    xSemaphoreGive(xLvglMutex);

    // Init camera task
    if (!xQueueCamera)
    {
        xQueueCamera = xQueueCreate(2, sizeof(camera_fb_t *));
        register_camera(PIXFORMAT_RGB565, FRAMESIZE_QVGA, 2, xQueueCamera);                                                                 // FRAMESIZE_HVGA  FRAMESIZE_QVGA FRAMESIZE_240X240
        xTaskCreatePinnedToCore(&displayCameraTask, "Display Camera Task", 4 * 1024, (void *)xQueueCamera, 5, &displayCameraTaskHandle, 1); // Create a camera display task, task level 5 (general level)
    }
}

void displayCameraTask(void *xQueue)
{
    camera_fb_t *frame = NULL;
    QueueHandle_t xCamera = (QueueHandle_t)xQueue;
    lv_img_dsc_t image;

    while (1)
    {
        // If queue hasn't been defined, don't use it
        if (xQueue == nullptr)
        {
            break;
        }

        // Process frames in queue
        if (xQueueReceive(xCamera, &frame, portMAX_DELAY))
        {
            xSemaphoreTake(xLvglMutex, portMAX_DELAY);

            // If show camera feed is enabled
            if (currentState == State::SHOW_CAMERA_FEED)
            {
                image.header.cf = LV_IMG_CF_TRUE_COLOR;
                image.header.always_zero = 0;
                image.header.w = frame->width;
                image.header.h = frame->height;
                image.data_size = frame->width * frame->height * 2;
                image.data = (const uint8_t *)frame->buf;
                lv_img_set_src(cameraObject, &image);
            }
            lv_task_handler();
            xSemaphoreGive(xLvglMutex);
            esp_camera_fb_return(frame);
        }

        vTaskDelay(1);
    }

    vTaskDelete(NULL);
}

// ------------ Fundamental Camera Functions ------------
void saveImage(camera_fb_t *frame, const char *imagePath, int quality)
{
    if (frame == NULL)
    {
        printf("Invalid frame buffer\n");
        return;
    }

    lv_fs_file_t file;
    lv_fs_res_t ret = 0;
    uint8_t *jpeg_buf = NULL;
    size_t jpeg_len = 0;
    bool need_free = false;

    if (frame->format == PIXFORMAT_JPEG)
    {
        jpeg_buf = frame->buf;
        jpeg_len = frame->len;
        need_free = false;
    }
    else
    {
        // Convert RGB565 (or other format) to JPEG
        if (!fmt2jpg(frame->buf, frame->len, frame->width, frame->height,
                     frame->format, quality, &jpeg_buf, &jpeg_len))
        {
            printf("Failed to convert frame to JPEG\n");
            // digital_write(eAmp_Gain, 0);
            return;
        }
        need_free = true;
    }

    // Open file for writing
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    ret = lv_fs_open(&file, imagePath, LV_FS_MODE_WR);
    xSemaphoreGive(xSPIlMutex);

    if (ret != LV_FS_RES_OK)
    {
        printf("Failed to open file for writing\n");
        if (need_free && jpeg_buf)
        {
            free(jpeg_buf);
        }
        // digital_write(eAmp_Gain, 0);
        return;
    }

    // Write JPEG data to file
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    ret = lv_fs_write(&file, jpeg_buf, jpeg_len, NULL);
    xSemaphoreGive(xSPIlMutex);

    if (ret != LV_FS_RES_OK)
    {
        printf("Failed to write JPEG data\n");
    }
    else
    {
        printf("Image saved successfully: %s (%d bytes)\n", imagePath, jpeg_len);
    }

    // Clean up
    lv_fs_close(&file);

    // Free JPEG buffer if it was allocated by fmt2jpg
    if (need_free && jpeg_buf)
    {
        free(jpeg_buf);
    }

    // digital_write(eAmp_Gain, 0);
}

camera_fb_t *takeImage(void)
{
    camera_fb_t *frame = NULL;

    // Enable amplifier gain for camera operation
    // digital_write(eAmp_Gain, 1);

    // Get camera frame from queue
    xSemaphoreTake(xLvglMutex, portMAX_DELAY);
    if (xQueueReceive(xQueueCamera, &frame, portMAX_DELAY))
    {
        xSemaphoreGive(xLvglMutex);
        return frame;
    }
    xSemaphoreGive(xLvglMutex);

    // Disable amplifier gain if failed
    // digital_write(eAmp_Gain, 0);
    return NULL;
}

void captureAndSaveImage(const char *imagePath, int quality)
{
    camera_fb_t *frame = takeImage();

    if (frame != NULL)
    {
        saveImage(frame, imagePath, quality);
    }
    else
    {
        printf("Failed to capture image\n");
    }

    esp_camera_fb_return(frame);
}

// ------------ AI Camera Functions ------------
bool captureAndProcessImage()
{

    // Get next photo number
    int photoNum = getNextPhotoNumber();

    // Create directory path
    char dirPath[100];
    snprintf(dirPath, sizeof(dirPath), "S:/photo-%d", photoNum);
    
    // Create directory first
    if (!createDirectoryIfNotExists(dirPath)) {
        printf("Failed to create directory: %s\n", dirPath);
        return false;
    }
    
    // Create file paths
    char inputPath[100];
    char outputPath[100];
    snprintf(inputPath, sizeof(inputPath), "%s/input.jpg", dirPath);
    snprintf(outputPath, sizeof(outputPath), "%s/output.jpg", dirPath);

    // Capture and save image
    printf("Capturing image\n");
    captureAndSaveImage(inputPath, 90);
    strcpy(originalImagePath, inputPath);

    // Process image with server
    printf("Processing image with server...\n");
    bool success = processImageWithServer(inputPath, outputPath);

    if (success)
    {
        printf("Image processing completed successfully!\n");
        printf("Input: %s\n", inputPath);
        printf("Output: %s\n", outputPath);
        strcpy(generatedImagePath, outputPath);
    }
    else
    {
        printf("Image processing failed\n");
    }

    return success;
}

// Event Handler
void shutterHandler()
{
    if (currentState == State::BEGIN) {
        currentState = State::SHOW_CAMERA_FEED;
        return;
    }
    if (currentState == State::SHOW_GEN_AI_IMAGE) {
        currentState = State::SHOW_CAMERA_FEED;
        return;
    }
    // If hit shutter in any mode except Loading and Shake
    if (currentState == State::SHOW_CAMERA_FEED) {
        currentState = State::LOADING;
        music.playTFCardAudio("S:/shutter.wav");
        captureAndProcessImage();
        currentState = State::SHAKE;
        return;
    }
}
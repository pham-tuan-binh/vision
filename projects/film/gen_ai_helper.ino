// Configuration
const char *SERVER_URL = "http://10.8.35.228:8000/upload_image";
const char *SERVER_URL_AUDIO = "http://10.8.35.228:8000/upload_audio";

// Function to send image to server and get processed result
bool processImageWithServer(const char *inputPath)
{

    // Check WiFi connection
    checkWiFiConnection();
    if (!isWiFiConnected())
    {
        printf("WiFi not connected\n");
        return false;
    }

    HTTPClient http;
    http.begin(SERVER_URL);

    // Read input image file
    lv_fs_file_t file;
    lv_fs_res_t res;

    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    res = lv_fs_open(&file, inputPath, LV_FS_MODE_RD);
    xSemaphoreGive(xSPIlMutex);

    if (res != LV_FS_RES_OK)
    {
        printf("Failed to open input image: %s\n", inputPath);
        http.end();
        return false;
    }

    // Get file size
    uint32_t fileSize = 0;
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    lv_fs_seek(&file, 0, LV_FS_SEEK_END);
    lv_fs_tell(&file, &fileSize);
    lv_fs_seek(&file, 0, LV_FS_SEEK_SET);
    xSemaphoreGive(xSPIlMutex);

    // Allocate buffer for image data
    uint8_t *imageData = (uint8_t *)malloc(fileSize);
    if (!imageData)
    {
        printf("Failed to allocate memory for image data\n");
        xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
        lv_fs_close(&file);
        xSemaphoreGive(xSPIlMutex);
        http.end();
        return false;
    }

    // Read image data
    uint32_t bytesRead = 0;
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    res = lv_fs_read(&file, imageData, fileSize, &bytesRead);
    lv_fs_close(&file);
    xSemaphoreGive(xSPIlMutex);

    if (res != LV_FS_RES_OK || bytesRead != fileSize)
    {
        printf("Failed to read image data\n");
        free(imageData);
        http.end();
        return false;
    }

    // Extract folder name
    const char *start = strchr(inputPath, '/');  // Points to "/photo-1/input.jpg"
    const char *end = strrchr(inputPath, '/');   // Points to "/input.jpg"

    char folderName[64] = {0};
    if (start && end && end > start + 1) {
        size_t len = end - (start + 1);
        strncpy(folderName, start + 1, len);
        folderName[len] = '\0'; // null-terminate
    } else {
        strcpy(folderName, "unknown"); // fallback
    }

    // Prepare multipart form data (image only)
    String boundary = "----ESP32Boundary";
    String contentType = "multipart/form-data; boundary=" + boundary;

    // Build multipart body with only image
    String body = "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"image\"; filename=\"" + String(folderName) + ".jpg\"\r\n";
    body += "Content-Type: image/jpeg\r\n\r\n";

    String bodyEnd = "\r\n--" + boundary + "--\r\n";

    // Calculate total content length
    int totalLength = body.length() + fileSize + bodyEnd.length();

    // Create payload string
    String payload = body;

    // Set headers properly
    http.addHeader("Content-Type", contentType);

    // Send POST request with multipart data
    // We need to build the complete payload
    uint8_t *fullPayload = (uint8_t *)malloc(totalLength);
    if (!fullPayload)
    {
        printf("Failed to allocate memory for payload\n");
        free(imageData);
        http.end();
        return false;
    }

    // Copy body start
    memcpy(fullPayload, body.c_str(), body.length());

    // Copy image data
    memcpy(fullPayload + body.length(), imageData, fileSize);

    // Copy body end
    memcpy(fullPayload + body.length() + fileSize, bodyEnd.c_str(), bodyEnd.length());

    // Set timeout
    http.setTimeout(60000);

    // Send POST request
    int httpResponseCode = http.POST(fullPayload, totalLength);

    // Clean up payload
    free(fullPayload);
    free(imageData);

    if (httpResponseCode == 200)
    {
        printf("Image uploaded successfully. \n");

        http.end();
        return true;
    }
    else
    {
        printf("HTTP request failed with code: %d\n", httpResponseCode);
        printf("Response: %s\n", http.getString().c_str());
        http.end();
        return false;
    }
}

bool processAudioWithServer(const char *inputPath)
{
    // Clear previous result
    memset(originalImagePath, 0, sizeof(originalImagePath));

    // Check WiFi connection
    checkWiFiConnection();
    if (!isWiFiConnected())
    {
        printf("WiFi not connected\n");
        return false;
    }

    HTTPClient http;
    http.begin(SERVER_URL_AUDIO); // You may need to define this as SERVER_URL + "/upload_sound"

    // Read input audio file
    lv_fs_file_t file;
    lv_fs_res_t res;

    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    res = lv_fs_open(&file, inputPath, LV_FS_MODE_RD);
    xSemaphoreGive(xSPIlMutex);

    if (res != LV_FS_RES_OK)
    {
        printf("Failed to open input audio: %s\n", inputPath);
        http.end();
        return false;
    }

    // Get file size
    uint32_t fileSize = 0;
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    lv_fs_seek(&file, 0, LV_FS_SEEK_END);
    lv_fs_tell(&file, &fileSize);
    lv_fs_seek(&file, 0, LV_FS_SEEK_SET);
    xSemaphoreGive(xSPIlMutex);

    // Allocate buffer for audio data
    uint8_t *audioData = (uint8_t *)malloc(fileSize);
    if (!audioData)
    {
        printf("Failed to allocate memory for audio data\n");
        xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
        lv_fs_close(&file);
        xSemaphoreGive(xSPIlMutex);
        http.end();
        return false;
    }

    // Read audio data
    uint32_t bytesRead = 0;
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    res = lv_fs_read(&file, audioData, fileSize, &bytesRead);
    lv_fs_close(&file);
    xSemaphoreGive(xSPIlMutex);

    if (res != LV_FS_RES_OK || bytesRead != fileSize)
    {
        printf("Failed to read audio data\n");
        free(audioData);
        http.end();
        return false;
    }

    // Extract audio filename for form data
    const char *lastSlash = strrchr(inputPath, '/');
    const char *audioFilename = lastSlash ? lastSlash + 1 : inputPath;

    // Prepare multipart form data (audio only)
    String boundary = "----ESP32Boundary";
    String contentType = "multipart/form-data; boundary=" + boundary;

    // Build multipart body with audio file
    String body = "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"file\"; filename=\"" + String(audioFilename) + "\"\r\n";
    body += "Content-Type: audio/wav\r\n\r\n";

    String bodyEnd = "\r\n--" + boundary + "--\r\n";

    // Calculate total content length
    int totalLength = body.length() + fileSize + bodyEnd.length();

    // Create full payload
    uint8_t *fullPayload = (uint8_t *)malloc(totalLength);
    if (!fullPayload)
    {
        printf("Failed to allocate memory for payload\n");
        free(audioData);
        http.end();
        return false;
    }

    // Copy body start
    memcpy(fullPayload, body.c_str(), body.length());

    // Copy audio data
    memcpy(fullPayload + body.length(), audioData, fileSize);

    // Copy body end
    memcpy(fullPayload + body.length() + fileSize, bodyEnd.c_str(), bodyEnd.length());

    // Set headers properly
    http.addHeader("Content-Type", contentType);

    // Set timeout
    http.setTimeout(60000);

    // Send POST request
    int httpResponseCode = http.POST(fullPayload, totalLength);

    // Clean up payload
    free(fullPayload);
    free(audioData);

    if (httpResponseCode == 200)
    {
        // Get the response (raw text with image folder name)
        String response = http.getString();
        response.trim(); // Remove any whitespace/newlines
        
        // Remove quotes if present
        response.replace("\"", "");
        response.replace("'", "");
        
        printf("Audio processed successfully. Matched image: %s\n", response.c_str());

        // Build the originalImagePath: S:/folder_name/input.jpg
        snprintf(originalImagePath, sizeof(originalImagePath), "S:/%s/input.jpg", response.c_str());
        
        printf("Original image path set to: %s\n", originalImagePath);

        http.end();
        return true;
    }
    else
    {
        printf("HTTP request failed with code: %d\n", httpResponseCode);
        printf("Response: %s\n", http.getString().c_str());
        http.end();
        return false;
    }
}
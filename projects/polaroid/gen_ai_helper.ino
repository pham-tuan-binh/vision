// Configuration
const char *SERVER_URL = "http://10.8.35.228:8000/upload_adv";

// Function to send image to server and get processed result
bool processImageWithServer(const char *inputPath, const char *outputPath)
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

    // Prepare multipart form data (image only)
    String boundary = "----ESP32Boundary";
    String contentType = "multipart/form-data; boundary=" + boundary;

    // Build multipart body with only image
    String body = "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"image\"; filename=\"image.jpg\"\r\n";
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
        // Get response payload
        String response = http.getString();

        // Save response to output file
        lv_fs_file_t outFile;
        xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
        res = lv_fs_open(&outFile, outputPath, LV_FS_MODE_WR);
        xSemaphoreGive(xSPIlMutex);

        if (res != LV_FS_RES_OK)
        {
            printf("Failed to open output file: %s\n", outputPath);
            http.end();
            return false;
        }

        // Write response data to file
        xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
        lv_fs_write(&outFile, response.c_str(), response.length(), NULL);
        lv_fs_close(&outFile);
        xSemaphoreGive(xSPIlMutex);

        printf("Output image saved: %s (%d bytes)\n", outputPath, response.length());

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

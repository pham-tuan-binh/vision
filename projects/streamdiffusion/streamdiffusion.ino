#include "unihiker_k10.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <esp_camera.h>

// Server configuration
const char* SERVER_HOST = "10.8.162.58";
const int SERVER_PORT = 7860;
String USER_ID = "550e8400-e29b-41d4-a716-446655440000";

// Wifi Configuration
const char* ssid = "PUT YOUR WIFI SSID HERE";
const char* password = "PUT YOUR WIFI PASSWORD HERE";

// Create UNIHIKER K10 instance
UNIHIKER_K10 board;

// WebSocket and HTTP clients
WebSocketsClient webSocket;
HTTPClient httpClient;

// Connection state
bool serverConnected = false;
bool waitingForFrameRequest = false;

// Task handles
TaskHandle_t websocketTaskHandle = NULL;
TaskHandle_t streamingTaskHandle = NULL;
TaskHandle_t cameraTaskHandle = NULL;

// LVGL
extern SemaphoreHandle_t xLvglMutex;

// Display object
lv_obj_t *diffusedImageObject = NULL;

// Camera queue and global frame storage
QueueHandle_t xQueueCamera = NULL;
camera_fb_t *globalFrame = NULL;
SemaphoreHandle_t frameMutex = NULL;
bool frameReady = false;

// WebSocket event handler
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("WebSocket Disconnected - attempting reconnect...");
            serverConnected = false;
            break;
            
        case WStype_CONNECTED:
            Serial.printf("WebSocket Connected to: %s\n", payload);
            serverConnected = true;
            break;
            
        case WStype_TEXT: {
            Serial.printf("Received: %s\n", payload);
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            
            String status = doc["status"];
            if (status == "send_frame") {
                waitingForFrameRequest = true;
            }
            break;
        }
        
        case WStype_ERROR:
            Serial.printf("WebSocket Error: %s\n", payload);
            break;
            
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            Serial.println("WebSocket Fragment received");
            break;
            
        default:
            Serial.printf("WebSocket event type: %d\n", type);
            break;
    }
}

// Continuous camera task - always consuming frames
void cameraTask(void* parameter) {
    camera_fb_t *frame = NULL;
    
    while (true) {
        // Aggressively consume ALL frames from queue to prevent overflow
        while (xQueueReceive(xQueueCamera, &frame, pdMS_TO_TICKS(1))) {
            
            // Take mutex and update global frame
            if (xSemaphoreTake(frameMutex, pdMS_TO_TICKS(1)) == pdTRUE) {
                // Release previous global frame if exists
                if (globalFrame != NULL) {
                    esp_camera_fb_return(globalFrame);
                }
                
                // Store new frame globally
                globalFrame = frame;
                frameReady = true;
                
                xSemaphoreGive(frameMutex);
                
                // Only print frame info occasionally
                static uint32_t frameCount = 0;
                if (frameCount % 100 == 0) {
                    Serial.printf("Frame #%d: %dx%d, %d bytes\n", frameCount, frame->width, frame->height, frame->len);
                }
                frameCount++;
            } else {
                // If can't get mutex, just return the frame to prevent memory leak
                esp_camera_fb_return(frame);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10)); // Fast consumption rate
    }
}

// WebSocket task
void websocketTask(void* parameter) {
    unsigned long lastSendTime = 0;
    unsigned long lastDebugTime = 0;
    const unsigned long sendInterval = 250; // Send frame every 1000ms (1 second)
    const unsigned long debugInterval = 5000; // Debug print every 5 seconds
    
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            webSocket.loop();
            
            // Always try to send frames when connected and frame is ready
            if (serverConnected && frameReady && waitingForFrameRequest) {
                Serial.println("Sending frame to server...");
                sendCameraFrame();
                waitingForFrameRequest = false;
                lastSendTime = millis();
            } else if (!serverConnected && (millis() - lastDebugTime > debugInterval)) {
                Serial.println("DEBUG: Server not connected");
                lastDebugTime = millis();
            }
        } else {
            Serial.println("WiFi disconnected, attempting reconnect...");
            connectToWiFi();
        }
        
        vTaskDelay(pdMS_TO_TICKS(50)); // Check more frequently
    }
}

// Send camera frame to server
void sendCameraFrame() {
    if (xSemaphoreTake(frameMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        if (globalFrame != NULL && frameReady) {
            // Send next_frame status
            DynamicJsonDocument statusDoc(256);
            statusDoc["status"] = "next_frame";
            String statusJson;
            serializeJson(statusDoc, statusJson);
            webSocket.sendTXT(statusJson);
            
            // Send parameters for diffusion
            DynamicJsonDocument paramsDoc(512);
            paramsDoc["strength"] = 0.8;
            paramsDoc["guidance_scale"] = 7.5;
            paramsDoc["prompt"] = "Portrait of The Joker halloween costume, face painting, with , glare pose, detailed, intricate, full of colour, cinematic lighting, trending on artstation, 8k, hyperrealistic, focused, extreme details, unreal engine 5 cinematic, masterpiece";
            String paramsJson;
            serializeJson(paramsDoc, paramsJson);
            webSocket.sendTXT(paramsJson);
            
            // Convert RGB565 to JPEG if needed
            uint8_t *jpeg_buf = NULL;
            size_t jpeg_len = 0;
            bool conversion_success = false;
            
            if (globalFrame->format == PIXFORMAT_RGB565) {
                // Convert RGB565 to JPEG
                conversion_success = fmt2jpg(globalFrame->buf, globalFrame->len, globalFrame->width, globalFrame->height, 
                                           PIXFORMAT_RGB565, 80, &jpeg_buf, &jpeg_len);
                if (conversion_success) {
                    webSocket.sendBIN(jpeg_buf, jpeg_len);
                    free(jpeg_buf);  // Free the allocated JPEG buffer
                }
            } else {
                // Send raw data if already JPEG
                webSocket.sendBIN(globalFrame->buf, globalFrame->len);
                conversion_success = true;
            }
            
            if (conversion_success) {
                Serial.printf("Frame sent successfully (%d bytes)\n", jpeg_len > 0 ? jpeg_len : globalFrame->len);
            } else {
                Serial.println("ERROR: Failed to convert frame to JPEG");
            }
        }
        
        xSemaphoreGive(frameMutex);
    }
}

// Streaming task to receive diffused images
void streamingTask(void* parameter) {
    while (true) {
        if (WiFi.status() == WL_CONNECTED && serverConnected) {
            receiveDiffusedImage();
        }
        
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// Receive diffused image from server
void receiveDiffusedImage() {
    String streamUrl = "http://" + String(SERVER_HOST) + ":" + String(SERVER_PORT) + "/api/stream/" + USER_ID;
    
    httpClient.begin(streamUrl);
    httpClient.setTimeout(2000);
    
    int httpCode = httpClient.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        WiFiClient* stream = httpClient.getStreamPtr();
        String boundary = "--frame";
        String line;
        
        while (httpClient.connected()) {
            if (stream->available()) {
                line = stream->readStringUntil('\n');
                
                if (line.indexOf(boundary) >= 0) {
                    // Skip headers
                    while (stream->available()) {
                        line = stream->readStringUntil('\n');
                        if (line.length() <= 2) break;
                    }
                    
                    // Read diffused image data
                    if (stream->available()) {
                        processDiffusedImage(stream);
                    }
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
    
    httpClient.end();
}

// Process diffused image
void processDiffusedImage(WiFiClient* stream) {
    static uint8_t imageBuffer[50000];
    size_t bytesRead = 0;
    
    while (stream->available() && bytesRead < sizeof(imageBuffer)) {
        int byte = stream->read();
        if (byte == -1) break;
        imageBuffer[bytesRead++] = byte;
    }
    
    if (bytesRead > 0) {
        displayDiffusedImage(imageBuffer, bytesRead);
    }
}

// Display diffused image on screen
void displayDiffusedImage(uint8_t* imageData, size_t dataSize) {
    xSemaphoreTake(xLvglMutex, portMAX_DELAY);
    
    static lv_img_dsc_t diffused_img;
    diffused_img.header.cf = LV_IMG_CF_TRUE_COLOR;
    diffused_img.header.always_zero = 0;
    diffused_img.header.w = 240;
    diffused_img.header.h = 320;
    diffused_img.data_size = dataSize;
    diffused_img.data = imageData;
    
    lv_img_set_src(diffusedImageObject, &diffused_img);
    
    xSemaphoreGive(xLvglMutex);
}

// WiFi connection function
void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
}

// Camera initialization function
void initCamera() {
    // Create frame mutex
    frameMutex = xSemaphoreCreateMutex();
    
    // Initialize camera using UNIHIKER K10 system with RGB565 format
    if (!xQueueCamera) {
        xQueueCamera = xQueueCreate(5, sizeof(camera_fb_t *)); // Larger queue to prevent overflow
        register_camera(PIXFORMAT_RGB565, FRAMESIZE_QVGA, 2, xQueueCamera);
        Serial.println("Camera initialized successfully");
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize board and screen
    board.begin();
    board.initScreen();
    
    // Black background
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);

    // Create image display object
    diffusedImageObject = lv_img_create(lv_scr_act());
    lv_obj_set_pos(diffusedImageObject, 0, 0);
    lv_obj_set_size(diffusedImageObject, 240, 320);
    
    // Initialize camera
    initCamera();

    // Connect to WiFi
    connectToWiFi();
    
    // Wait a bit for WiFi to stabilize
    delay(2000);
    
    // Test server connectivity first
    Serial.printf("Testing server connectivity to %s:%d\n", SERVER_HOST, SERVER_PORT);
    HTTPClient testClient;
    testClient.begin("http://" + String(SERVER_HOST) + ":" + String(SERVER_PORT) + "/api/queue");
    int httpCode = testClient.GET();
    if (httpCode > 0) {
        String response = testClient.getString();
        Serial.printf("Server test response: %d - %s\n", httpCode, response.c_str());
    } else {
        Serial.printf("Server test failed: %d\n", httpCode);
    }
    testClient.end();
    
    Serial.printf("Connecting to WebSocket: ws://%s:%d/api/ws/%s\n", SERVER_HOST, SERVER_PORT, USER_ID.c_str());
    
    // Initialize WebSocket connection
    webSocket.begin(SERVER_HOST, SERVER_PORT, "/api/ws/" + USER_ID);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
    webSocket.enableHeartbeat(15000, 3000, 2);  // Enable heartbeat to keep connection alive
    
    // Start tasks
    xTaskCreatePinnedToCore(cameraTask, "Camera", 6144, NULL, 4, &cameraTaskHandle, 1);      // Even higher priority camera task
    xTaskCreatePinnedToCore(websocketTask, "WebSocket", 8192, NULL, 2, &websocketTaskHandle, 0);
    xTaskCreatePinnedToCore(streamingTask, "Streaming", 10240, NULL, 1, &streamingTaskHandle, 1); // Lower priority for streaming
    
    Serial.println("Setup complete!");
}

void loop() {
    // Handle LVGL
    xSemaphoreTake(xLvglMutex, portMAX_DELAY);
    lv_task_handler();
    xSemaphoreGive(xLvglMutex);
    
    vTaskDelay(pdMS_TO_TICKS(5));
}
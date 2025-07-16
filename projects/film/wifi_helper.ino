
#include <WiFi.h>

// WiFi Configuration
const char *WIFI_SSID = "PUT YOUR WIFI SSID HERE";
const char *WIFI_PASSWORD = "PUT YOUR WIFI PASSWORD HERE";

// Function to connect to WiFi
void connectToWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    printf("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        printf(".");
    }

    printf("\nWiFi connected successfully!\n");
    printf("IP address: %s\n", WiFi.localIP().toString().c_str());
    printf("Signal strength: %d dBm\n", WiFi.RSSI());
}

// Function to check WiFi status
bool isWiFiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

// Function to reconnect WiFi if disconnected
void checkWiFiConnection()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        printf("WiFi disconnected, reconnecting...\n");
        connectToWiFi();
    }
}
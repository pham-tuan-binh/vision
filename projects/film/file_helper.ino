// LVGL
extern SemaphoreHandle_t xLvglMutex;
extern SemaphoreHandle_t xSPIlMutex;

// Function to get next photo number by counting photo directories
int getNextPhotoNumber()
{
    int photoCount = 0;
    lv_fs_dir_t dir;
    lv_fs_res_t res;

    // Open the root directory
    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    res = lv_fs_dir_open(&dir, "S:/");
    xSemaphoreGive(xSPIlMutex);

    if (res != LV_FS_RES_OK)
    {
        printf("Failed to open directory\n");
        return 1; // Start from 1 if can't read directory
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

        // Check if it starts with "photo-"
        if (strncmp(fn, "/photo-", 6) == 0)
        {
            // Verify it's a directory by trying to open it
            char fullPath[300];
            snprintf(fullPath, sizeof(fullPath), "S:/%s", fn);
            
            lv_fs_dir_t testDir;
            xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
            lv_fs_res_t testRes = lv_fs_dir_open(&testDir, fullPath);
            if (testRes == LV_FS_RES_OK) {
                lv_fs_dir_close(&testDir);
                xSemaphoreGive(xSPIlMutex);
                
                photoCount++;
            } else {
                xSemaphoreGive(xSPIlMutex);
            }
        }
    }

    xSemaphoreTake(xSPIlMutex, portMAX_DELAY);
    lv_fs_dir_close(&dir);
    xSemaphoreGive(xSPIlMutex);

    printf("Total photo directories found: %d, returning next number: %d\n", photoCount, photoCount + 1);
    return photoCount + 1;
}

// Helper function to create directory using SD library
bool createDirectoryIfNotExists(const char* path) {
    // Convert LVGL path (S:/) to SD path
    const char* sd_path = path;
    if (path[0] == 'S' && path[1] == ':') {
        sd_path = path + 2; // Skip "S:" prefix
    }
    
    // Check if directory exists
    if (SD.exists(sd_path)) {
        return true;
    }
    
    // Create directory
    if (SD.mkdir(sd_path)) {
        printf("Created directory: %s\n", path);
        return true;
    } else {
        printf("Failed to create directory: %s\n", path);
        return false;
    }
}
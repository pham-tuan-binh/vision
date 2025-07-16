# Project Vision

!! Put image here

Project Vision is an interactive polaroid image built on top of the Unihiker K10. With this device, hobbyists can try different interactive paradigms:

- **A GenAI Camera**: Take a picture, get a GenAI photo.
- **A Voice Photo Gallery**: Interact with your photo gallery through voice e.g. when was the last time I was happy.
- **A Portal**: Stream diffused camera stream to show you an alternate world.
- **A Locket**: Take a picture, share instantly with friends (Mockup Stage).
- **A Memory Reminder Device**: Reminds you of past memories every day (Mockup Stage).

To understand more about the device, you're welcome to read our experiment article here.

!! Put image here

To have an overview of the device functioning, you're welcome to watch our introduction video here.

!! Put image here

# Project Structure

This repository contains 4 directories:

- **Models** contains 3D models of the device. You can also reference and export these directly from our OnShape design [here](https://cad.onshape.com/documents/d583c9ba23d9ff332b713a2b/w/55b93e8969db0e898803f818/e/b88d03c7973e5a4312ce2e82?renderMode=0&uiState=6877e7c6374a463b3179bf6e).
- **Projects** contains the Arduino projects for each of the aforementioned functions of the device. Inside each project, you can also find the server directory which contains the backend for corresponding application.
- **Storage** contains the artifacts used by the device for UI/UX. You'll need to put the content of this directory on an SD card and plug it into the device for it to work properly.
- **Images** contains additional documentation and reference images.

# Requirements

## Hardware Requirements

!! Put image here

This project is sponsored by DFRobot. To build this project, you'll need:

- A Unihiker K10 (You can buy it [here](https://www.unihiker.com/products/k10))
- Access to 3D printer of choice
- An SD card (minimum 512MB recommended)
- A computer with Arduino IDE installed

Unihiker K10 is an AI learning device designed for education. It integrates a 2.8-inch color screen, Wi-Fi and Bluetooth, a camera, microphone, speaker, RGB light, temperature sensor, humidity sensor, light sensor, and accelerometer. It also features edge connectors and Gravity IO interfaces, enabling the connection and control of various digital, analog, I2C, and UART sensors and actuators.

We're proud to be one of the first hobbyists to develop use cases for the device.

## Software Requirements

Each project contains 2 software components:

- **Arduino Firmware**: Controls the hardware and user interface
- **FastAPI Backend**: Handles AI processing and external API calls (for projects that need it)

Furthermore, for certain projects, you'll need extra dependencies:

- **Polaroid**: You'll need an API Key to [Google Gemini API](https://aistudio.google.com/) and [ImageRouter](https://imagerouter.io/).
- **Film**: You'll need an API Key to [Google Gemini API](https://aistudio.google.com/).
- **Memory-cue, Locket, Streamdiffusion**: No external API keys required.

These project-specific notes will be mentioned later in the instructions.

# Arduino IDE Setup and Deployment

## Initial Setup

To set up the development environment for the Unihiker K10, it is recommended to go through this [official document first](https://www.unihiker.com/wiki/K10/get-started/#unihiker-k10) and set up one of their examples code.

**Note**: At the point of writing, Unihiker documentation may contain some bugs which might force you to edit the board package platform. If you encounter this problem, please submit an issue request and we'll provide specific instructions if the Unihiker team hasn't resolved this problem yet.

## Installing a Project

1. Open Arduino IDE
2. Go to **File** → **Open**
3. Navigate to the project folder you want to install (e.g., `projects/polaroid/`)
4. Select the main `.ino` file (e.g., `polaroid.ino`)
5. Arduino IDE will automatically open all the helper files in the same folder

## Compiling and Uploading

1. Make sure your Unihiker K10 is connected to your computer via USB
2. In Arduino IDE, select the correct board:
   - Go to **Tools** → **Board** → **Unihiker K10**
3. Select the correct port:
   - Go to **Tools** → **Port** → Select the port where your Unihiker K10 appears
4. Click the **Upload** button (→ arrow icon) or press **Ctrl+U** (Windows/Linux) or **Cmd+U** (Mac)
5. Wait for the compilation and upload to complete
6. The device will automatically restart and run your code

## WiFi Setup

If a project requires WiFi connection, a file called `wifi_helper.ino` will be available.

To configure WiFi:

1. Open the `wifi_helper.ino` file in your project
2. Find these lines:
   ```cpp
   const char *WIFI_SSID = "PUT YOUR WIFI SSID HERE";
   const char *WIFI_PASSWORD = "PUT YOUR WIFI PASSWORD HERE";
   ```
3. Replace `"PUT YOUR WIFI SSID HERE"` with your WiFi network name
4. Replace `"PUT YOUR WIFI PASSWORD HERE"` with your WiFi password
5. Save the file and upload the project again

**Example:**

```cpp
const char *WIFI_SSID = "MyHomeWiFi";
const char *WIFI_PASSWORD = "mypassword123";
```

## Backend Connection Setup

If a project requires backend connection, a file called `gen_ai_helper.ino` will be available.

To configure the server IP:

1. Open the `gen_ai_helper.ino` file in your project
2. Find this line:
   ```cpp
   const char *SERVER_URL = "http://10.8.35.228:8000/upload_adv";
   ```
3. Replace the IP address with your computer's IP address where the server will run
4. Save the file and upload the project again

**To find your computer's IP address:**

- **Windows**: Open Command Prompt and type `ipconfig`
- **Mac/Linux**: Open Terminal and type `ifconfig` or `ip addr`

Look for your local network IP (usually starts with `192.168.` or `10.`)

# Server Setup

If a project requires a backend server to work, you can find a `server` directory in such project.

## Installing uv Package Manager

To run this server, you'll need to install uv package manager for Python environment. Further instructions can be found [here](https://docs.astral.sh/uv/getting-started/installation/).

**Quick installation:**

- **Windows**: `powershell -c "irm https://astral.sh/uv/install.ps1 | iex"`
- **macOS/Linux**: `curl -LsSf https://astral.sh/uv/install.sh | sh`

## Running the Server

1. Open a terminal/command prompt
2. Navigate to the server directory of your project:
   ```bash
   cd projects/polaroid/server
   ```
3. Run the server:
   ```bash
   uv run main.py
   ```
4. The server will start and display the IP address and port (usually `http://0.0.0.0:8000`)

## API Key Setup

As mentioned before, you need to set up some services for the backend to work. For each service, you'll need an API key. You'll need to write a `.env` file with the API keys of each service.

1. In the server directory, copy the example environment file:
   ```bash
   cp .example.env .env
   ```
2. Open the `.env` file in a text editor
3. Replace the placeholder values with your actual API keys:

**For Polaroid project:**

```
GEMINI_API_KEY=your_actual_gemini_api_key_here
IMAGEROUTER_API_KEY=your_actual_imagerouter_api_key_here
```

**For Film project:**

```
GEMINI_API_KEY=your_actual_gemini_api_key_here
```

### Getting API Keys

- **Google Gemini API**:

  1. Go to [Google AI Studio](https://aistudio.google.com/)
  2. Sign in with your Google account
  3. Click "Get API key" or go to the API keys section
  4. Create a new API key
  5. Copy the key and paste it in your `.env` file

- **ImageRouter API**:
  1. Go to [ImageRouter](https://imagerouter.io/)
  2. Sign up for an account
  3. Navigate to your API keys section
  4. Generate a new API key
  5. Copy the key and paste it in your `.env` file

## Testing the Server

After setting up the API keys and running the server, you can test if it's working by opening your web browser and going to:

```
http://localhost:8000/docs
```

This will show you the FastAPI documentation interface where you can test the endpoints.

# SD Card Setup

The device requires certain files to be stored on an SD card for proper operation.

## Preparing the SD Card

1. Format your SD card as FAT32
2. Copy the entire contents of the `storage/` directory to the root of your SD card
3. Make sure the folder structure is preserved

**Important**: The SD card must be inserted into the Unihiker K10 before powering it on.

## Project-Specific Files

Each project uses different files from the storage directory:

- **Polaroid**: Uses files from `storage/polaroid/` and `storage/shutter.wav`, `storage/loading.wav`
- **Film**: Uses files from `storage/film/` and `storage/shutter.wav`, `storage/loading.wav`
- **Memory-cue**: Uses files from `storage/memory-cue/`
- **Locket**: Uses files from `storage/locket/`
- **Streamdiffusion**: No additional files required

# 3D Enclosure

The device's enclosure is divided into 3 parts:

- **Body**: Main housing for the Unihiker K10
- **Back Plate**: Covers the back of the device
- **Button**: Interactive button component

The design philosophy is that you'll have a device where you can easily customize. In this case, it's the back plate. There were multiple back plates that were designed in our experiment. However, there are only 2 options now: one without screws and one with screws.

The one without screws simply fits by friction and therefore the fit can vary with different 3D printers.

## Assembly Instructions

1. **Print the 3D models**: Print the Body, Back Plate (choose with or without screws), and Button
2. **Insert the button**: Put the buttons into the holes on the side first
3. **Insert the device**: Place the Unihiker K10 into the body, orienting it so that the Type-C charger port aligns with its hole
4. **Attach the back plate**: Place the back plate on the back of the device
5. **Insert the SD card**: Make sure the SD card is properly inserted
6. **Power on**: Connect the device to power or insert batteries

## Printing Recommendations

- **Material**: PLA or PETG recommended
- **Infill**: 15% infill for everything.
- **Support**: Enable supports for overhangs
- **Layer height**: 0.2mm for good quality
- **Print orientation**: Print the body with the large flat side down

# Project-Specific Instructions

## Polaroid Project

!! Put image here

- **Features**: Takes photos and applies AI-generated effects
- **Requirements**: WiFi connection, server running, Gemini API key, ImageRouter API key
- **Usage**: Shake the device to take a photo, wait for AI processing

## Film Project

!! Put image here

- **Features**: Creates vintage film-style photos with AI enhancement
- **Requirements**: WiFi connection, server running, Gemini API key
- **Usage**: Shake the device to take a photo, wait for processing

## Memory-cue Project

!! Put image here

- **Features**: Displays pre-loaded memories at scheduled times
- **Requirements**: No WiFi or server needed
- **Usage**: Device automatically shows memories, shake to cycle through them

## Locket Project

!! Put image here

- **Features**: Takes photos and displays them (mockup stage)
- **Requirements**: No WiFi or server needed
- **Usage**: Shake to take a photo, view stored photos

## Streamdiffusion Project

!! Put image here

- **Features**: Real-time AI image processing stream
- **Requirements**: Set up the realtime-img2img demo from the [Stream Diffusion project](https://github.com/cumulo-autumn/StreamDiffusion) before using this feature.
- **Usage**: Point camera at objects to see AI-processed versions

# Troubleshooting

## Common Issues

**Device won't turn on:**

- Check if SD card is properly inserted
- Ensure power connection is secure
- Try a different USB cable

**WiFi connection fails:**

- Double-check SSID and password in `wifi_helper.ino`
- Ensure your WiFi network is 2.4GHz (5GHz may not work)
- Check if the device is within range of your router

**Server connection fails:**

- Verify the IP address in `gen_ai_helper.ino` matches your computer's IP
- Ensure the server is running (`uv run main.py`)
- Check if your firewall is blocking the connection
- Make sure both device and computer are on the same network

**Photos not saving:**

- Check if SD card has enough free space
- Ensure SD card is formatted as FAT32
- Verify all required files are copied to SD card

**3D printed parts don't fit:**

- Check your printer's calibration
- Try the back plate with screws if friction fit is too loose
- Adjust print settings for better dimensional accuracy

## Getting Help

If you encounter issues not covered here:

1. Check the [Unihiker K10 documentation](https://www.unihiker.com/wiki/K10/)
2. Submit an issue on this repository
3. Check the Arduino IDE serial monitor for error messages

# License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

This means you are free to:

- Use this software for any purpose
- Modify the software
- Distribute the software
- Distribute modified versions

However, if you distribute this software or modified versions, you must also distribute the source code under the same license terms.

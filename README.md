# 553 Final Project - ImageProcessingApp

GitHub Repository:  
[https://github.com/Yi-WenLin/553-final-project](https://github.com/Yi-WenLin/553-final-project)

## Prerequisites 
1. This project uses **OpenCV** and **ONNX Runtime**, which can be installed via [Homebrew](https://brew.sh/) on macOS:

   ```camke
   brew install opencv
   brew install onnxruntime
   ```
3. Then, modify the `CMakeLists.txt` to include the correct ONNX Runtime path:

   ```bash
   # Run this in terminal to get the Homebrew prefix, and ensure the path contains the header files.
   brew --prefix onnxruntime

   # Then set ONNXRUNTIME_DIR in CMakeLists.txt accordingly:
   set(ONNXRUNTIME_DIR "/usr/local/opt/onnxruntime")
   ```
> **Note:** Do not clone or copy the build/ folder from the repository. Please create it manually before building the project. As CMake doesn’t always automatically detect all changes in `CMakeLists.txt`. The `build/` directory contains a `CMakeCache.txt` file and internal config files. These may conflict with new settings or prevent changes from being applied properly.



## Note
1. Test images are located in the `input/` folder. Please use the full local path when running the program, e.g.:
   
   ```bash
   Enter image path: /Users/Desktop/Milestone/input/test_img.jpg
   ```
2. Before running, please **update the ONNX model path** for style transfer in **case 6** inside `main.cpp` to match your local environment. 
3. Output images will be saved to the `build/` directory.
4. Main Menu Functionality:
   - Load Photo  
   - List Loaded Photos  
   - Select Current Photo  
   - Show Current Photo  
   - Apply Artistic Filter  
   - Apply Style Transfer  
   - View Metadata  
   - Save Current Photo  
   - Delete a Photo  
   - Add Tag  
   - Exit
6. The style transfer models are pre-trained. (GitHub model source link: https://github.com/yakhyo/fast-neural-style-transfer)
7. Style transfer may take a few seconds to process. Thanks for your patience.
8. We have silenced ONNX schema warnings, and hope that works in different environments.

## Folder Structure
```
ImageProcessingApp/
│
├── build/
│
├── include/
│   ├── Photo.h
│   ├── FilteredPhoto.h
│   ├── TransferredPhoto.h
│   └── ArtisticFilter.h
│
├── src/
│   ├── Photo.cpp
│   ├── FilteredPhoto.cpp
│   ├── TransferredPhoto.cpp
│   └── ArtisticFilter.cpp
│
├── input/
│   └── test_img.jpg
│
├── models/
│   ├── candy.onnx
│   └── mosaic.onnx
│
├── main.cpp
├── CMakeLists.txt
└── README.md
```

## How to Build and Run

1. Open a terminal.
2. Navigate to the project directory.
3. Build and run the program:

```bash
# Step 1. Create build directory
mkdir build
cd build

# Step 2. Run CMake
Cmake ..

# Step 3. Compile
make

# Step 4. Execute the program
./ImageProcessingApp
```

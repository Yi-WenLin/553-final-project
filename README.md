# 553 Final Project - ImageProcessingApp
Here is the GitHub link of this project: 
https://github.com/Yi-WenLin/553-final-project

## Note
1. The test image is placed in the `input/` folder. Please use the image path in your local environment while running the program, e.g.:
   
   ```
   Enter image path: /Users/Desktop/Milestone/input/test_img.jpg
   ```
2. Before running, please modifying the model path in your local envoronment in case 6 in `main.cpp`. 
3.	The output image will be stored in the `build/` folder.
4.	Functions in main Menu:
   ```
	 • Load Photo
	 • List Loaded Photos
	 • Select Current Photo
	 • Show Current Photo
	 • Apply Artistic Filter
	 • Apply Style Transfer
	 • View Metadata
	 • Save Current Photo
	 • Delete a Photo
     • Add Tag
	 • Exit
   ```
5. We are using the trained model that provided by (github link - Still finding).
6. When applying the Style Transfer, it might take few seconds to run. Thanks for patience. I have muted the schema error, hope that works in different environment.

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

## How to Run the Program

1. Open a terminal.
2. Navigate to the project directory.
3. Build and run the program:

```bash
cd build
Cmake ..
make
./ImageProcessingApp
```

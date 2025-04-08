# 553 Final Project - ImageProcessingApp
Here is the GitHub link of this project: 
## Test in milestone
1. The test image is placed in the `input/` folder. Please use the image path in your local environment while running the program, e.g.:
   ```
   Enter image path: /Users/Desktop/Milestone/input/test_img.jpg
   ```
2.	The output image will be stored in the build/ folder.
3.	The following functions are available in this milestone:
   ```
	 • Load Image
	 • Apply Artistic Filter
	 • Show Image (note: may not work successfully on macOS)
	 • Save Image
	 • Pencil Sketch
	 • Cartoon Sketch
	 • Oil Painting
	 • Add Date
	 • Back
	 • Exit
   ```

## Folder Structure
```
ImageProcessingApp/
│
├── build/
│
├── include/
│   ├── ImageLoader.h
│   ├── ArtisticFilter.h
│   ├── [Under Construction] FaceProcessor.h
│   ├── [Under Construction] Inpainter.h
│   └── Menu.h
│
├── src/
│   ├── ImageLoader.cpp
│   ├── ArtisticFilter.cpp
│   ├── [Under Construction] FaceProcessor.cpp
│   ├── [Under Construction] Inpainter.cpp
│   └── Menu.cpp
│
├── input/
│   └── test_img.jpg
│
├── output/
│   └── output.jpg
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


## Some problems need to be solved after the milestone:
1. Add error handling: throw an exception or show a warning if the user tries to apply a filter without uploading an image.
2. Improve the stability of imshow() on macOS. Add option to preview processed image externally if imshow() fails. (Try on other OS)
3. Implement FaceProcessor and Inpainter classes.

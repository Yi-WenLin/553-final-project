# 553 Final Project - ImageProcessingApp

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

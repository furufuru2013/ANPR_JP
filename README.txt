******************************************************************************
*   Automatic Number Plate Recognition using SVM and Neural Networks
******************************************************************************
*   modified by @furufuru , 16th Jan 2014 for adapting Japanese Number Plate
*                                                     (now only 4 numbers)
*   ORIGINAL:
*	by David Millán Escrivá, 5th Dec 2012
*	http://blog.damiles.com
******************************************************************************
*
*   Ch5 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
******************************************************************************

This code is sample code to understand how automatic license plate recognition (ANPR) works. It is not for production tasks.
You can use this code as a sample & guide to create your own custom ANPR or OCR applications.


----------------------------------------------------------
Building the project using CMake from the command-line:
----------------------------------------------------------
Linux: for using default library path (/usr/local/lib)
    mkdir build
    cd build
    cmake ..
    make 

Linux: for using original OpenCV directory
    export OpenCV_DIR="~/OpenCV/build"
    mkdir build
    cd build
    cmake -D OpenCV_DIR=$OpenCV_DIR ..
    make 

MacOSX (Xcode):
    export OpenCV_DIR="~/OpenCV/build"
    mkdir build
    cd build
    cmake -G Xcode -D OpenCV_DIR=$OpenCV_DIR ..
    open ANPR.xcodeproj

Windows (MS Visual Studio):
    set OpenCV_DIR="C:\OpenCV\build"
    mkdir build
    cd build
    cmake -G "Visual Studio 9 2008" -D OpenCV_DIR=%OpenCV_DIR% ..
    start ANPR.sln 

    
----------------------------------------------------------
Running the project:
----------------------------------------------------------
(1) Recognition
Usage:
        ./ANPR [-w|-i|-d|-s|-c|-h] image
  -w : Web mode (simple output)
  -i : save Result Image to result folder
  -d : display Result Image to screen
  -s : show step on Detect Plate
  -c : OCR debug mode
  -h : This help

    ./ANPR test/SDIM0366-800x533.jpg

You can choose other images that are in the test folder or other images that contain a Japanese license plate taken from 2 to 3 meters. And 800 x 600 Jpeg size is good for recognition.
There are also some UNIX Bash scripts in the "utils" folder for Linux or Mac, that need Cygwin to run on Windows.

If you make 'tmp' directory in the execution environment, you can obtain a license plate image the program recognizes.
This file can be used in machine learning, which is described on 'machine-learning/README.txt'.
In addition, if you make 'tmpChars' directory in the execution environment, you can obtain a number of image this program has detected.
They can be used in machine learning, which is described on 'machine-learning/README.txt', too.

(2) Machine learning --- Number Plate Detection (SVM.xml)
 Please refer to machine-learning/README.txt

(3) Machine learning --- Number Recognision (OCR.xml)
 Please refer to machine-learning/README.txt


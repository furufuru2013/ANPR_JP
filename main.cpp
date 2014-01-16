/*****************************************************************************
*   Number Plate Recognition using SVM and Neural Networks
******************************************************************************
*   by David Millán Escrivá, 5th Dec 2012
*   http://blog.damiles.com
******************************************************************************
*   Ch5 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

// Main entry code OpenCV

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <ml.h>

#include <iostream>
#include <vector>

#include "DetectRegions.h"
#include "OCR.h"

using namespace std;
using namespace cv;

string getFilename(string s) {

    char sep = '/';
    char sepExt='.';

    #ifdef _WIN32
        sep = '\\';
    #endif

    size_t i = s.rfind(sep, s.length( ));
    if (i != string::npos) {
        string fn= (s.substr(i+1, s.length( ) - i));
        size_t j = fn.rfind(sepExt, fn.length( ));
        if (i != string::npos) {
            return fn.substr(0,j);
        }else{
            return fn;
        }
    }else{
        return "";
    }
}

int main ( int argc, char** argv )
{
    int result;
    char* filename;
    Mat input_image;
    bool  web_mode = false;
    bool  result_image_mode = false;
    bool  display_mode = false;
    bool  show_step = false;
    bool  ocr_debug_mode = false;

    while ((result = getopt(argc, argv, "widhsc")) != (-1)) {
      switch (result) {
	case 'w': // web mode
		web_mode = true;
		break;
	case 'i': //create result image
		result_image_mode = true;
		break;
	case 'd': // display result image
		display_mode = true;
		break;
	case 's': // DetectRegion.showStep = true
		show_step = true;
		break;
	case 'c': // DetectRegion.showStep = true
		ocr_debug_mode = true;
		break;
        case 'h':
		printf("Usage:\n\t%s [-w|-i|-d|-s|-c|-h] image\n", argv[0]);
		printf("  -w : Web mode (simple output)\n");
		printf("  -i : save Result Image to result folder\n");
		printf("  -d : display Result Image to screen\n");
		printf("  -s : show step on Detect Plate\n");
		printf("  -c : OCR debug mode\n");
		printf("  -h : This help\n");
		exit(0);
      }
    }

    if (optind >= argc) {
	printf("Usage:\n\t%s [-w|-i|-d|-s|-c|-h] image\n", argv[0]);
	exit(0);
    }

    for (;optind < argc; optind++) {
        filename = argv[optind];
        input_image=imread(filename,1);
    }

    if (web_mode != true)
        cout << "OpenCV Automatic Number Plate Recognition\n";

/*
    //Check if user specify image to process
    if(argc >= 2 )
    {
        filename= argv[1];
        //load image  in gray level
        input_image=imread(filename,1);
    }else{
        printf("Use:\n\t%s image\n",argv[0]);
        return 0;
    }        
*/

    string filename_whithoutExt=getFilename(filename);
    if (web_mode != true)
    	cout << "working with file: "<< filename_whithoutExt << "\n";
    //Detect posibles plate regions
    DetectRegions detectRegions;    
    detectRegions.setFilename(filename_whithoutExt);
    // detectRegions.saveRegions=false;
    // detectRegions.showSteps=false;
    detectRegions.saveRegions=true;
    // detectRegions.showSteps=true;
    if (show_step == true)
	detectRegions.showSteps=true;

    vector<Plate> posible_regions= detectRegions.run( input_image );    

    //SVM for each plate region to get valid car plates
    //Read file storage.
    FileStorage fs;
    fs.open("SVM.xml", FileStorage::READ);
    Mat SVM_TrainingData;
    Mat SVM_Classes;
    fs["TrainingData"] >> SVM_TrainingData;
    fs["classes"] >> SVM_Classes;
    //Set SVM params
    CvSVMParams SVM_params;
    SVM_params.svm_type = CvSVM::C_SVC;
    SVM_params.kernel_type = CvSVM::LINEAR; //CvSVM::LINEAR;
    SVM_params.degree = 0;
    SVM_params.gamma = 1;
    SVM_params.coef0 = 0;
    SVM_params.C = 1;
    SVM_params.nu = 0;
    SVM_params.p = 0;
    SVM_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
    //Train SVM
    CvSVM svmClassifier(SVM_TrainingData, SVM_Classes, Mat(), Mat(), SVM_params);

    //For each possible plate, classify with svm if it's a plate or no
    vector<Plate> plates;
    for(int i=0; i< posible_regions.size(); i++)
    {
        Mat img=posible_regions[i].plateImg;
        Mat p= img.reshape(1, 1);
        p.convertTo(p, CV_32FC1);

        int response = (int)svmClassifier.predict( p );
        if(response==1)
            plates.push_back(posible_regions[i]);
    }

    if (web_mode != true)
    	cout << "Num plates detected: " << plates.size() << "\n";
    //For each plate detected, recognize it with OCR
    OCR ocr("OCR.xml");    
    ocr.saveSegments=true;
    ocr.DEBUG=ocr_debug_mode; // @f
    ocr.filename=filename_whithoutExt;
// cout << "---- plates.size() = " << plates.size() << "\n";	// @f
    for(int i=0; i< plates.size(); i++){
        Plate plate=plates[i];
        
        string plateNumber=ocr.run(&plate);
// cout << "---- platesNumber() = " << plateNumber << "\n";	// @f
        string licensePlate= plate.str();
    	if (web_mode != true) {
            cout << "================================================\n";
            cout << "License plate number: "<< licensePlate << "\n";
            cout << "================================================\n";
	} else {
            cout << licensePlate << "\n";
	}
        rectangle(input_image, plate.position, Scalar(0,0,200));
        putText(input_image, licensePlate, Point(plate.position.x, plate.position.y), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,200),2);
        if(false){
            imshow("Plate Detected seg", plate.plateImg);
            cvWaitKey(0);
        }

    }

    if (result_image_mode == true) {
	string wrt_filename = "result/";
	wrt_filename += (filename_whithoutExt + ".jpg");
//cout << "--- create image file " << wrt_filename << "\n";
	imwrite(wrt_filename, input_image);
    }

    if (display_mode == true) {
    	imshow("Plate Detected", input_image);
        cvWaitKey(0);
/*
	for(;;) {
		int c;
		c = cvWaitKey(10) & 0xff;
		if( (char) c == 27)
		break;
	}
*/
    }
    return 0;
}

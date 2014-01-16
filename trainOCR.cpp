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

#include "OCR.h"

#include <dirent.h>     // @f
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

//    const char OCR::strCharacters[] = {'0','1','2','3','4','5','6','7','8','9'};
// const int numFilesChars[]=           { 7,  5, 25,  7,  7, 12,  5, 22, 22,  4};

int main ( int argc, char** argv )
{
    cout << "OpenCV Training OCR Automatic Number Plate Recognition\n";
    cout << "\n";

    char* path;

    // definition for Directory List  by @f
    DIR			*dir;
    struct dirent	*entry;
    
    //Check if user specify image to process
    if(argc >= 1 )
    {
        path= argv[1];
    
    }else{
        cout << "Usage:\n" << argv[0] << " <path to chars folders files>/ \n";
        return 0;
    }        






    Mat classes;
    Mat trainingDataf5;
    Mat trainingDataf10;
    Mat trainingDataf15;
    Mat trainingDataf20;

    vector<int> trainingLabels;
    OCR ocr;
    char	path_num[256];			// @f

    for(int i=0; i< OCR::numCharacters; i++)
    {
	sprintf(path_num, "%s%d", path, i);	// @f

	// get directory informations @f
	if ((dir = opendir(path_num)) == NULL) {
		perror(argv[0]);
		return(-1);
	}

        // int numFiles=numFilesChars[i];
        // for(int j=0; j< numFiles; j++){
	while ((entry = readdir(dir)) != NULL) {
	    if (strlen(entry->d_name) < 5 || strcmp(entry->d_name + strlen(entry->d_name) - 4, ".jpg")) continue;

            // cout << "Character "<< OCR::strCharacters[i] << " file: " << j << "\n"; // @f
            cout << "Character "<< OCR::strCharacters[i] << " file: " << entry->d_name << "\n";	// @f
            stringstream ss(stringstream::in | stringstream::out);
            // ss << path << OCR::strCharacters[i] << "/" << j << ".jpg";	// @f
	    ss << path_num << "/" << entry->d_name;	// @f
            cout << "---- Reading " << ss.str() << "\n";
            Mat img=imread(ss.str(), 0);
            Mat f5=ocr.features(img, 5);
            Mat f10=ocr.features(img, 10);
            Mat f15=ocr.features(img, 15);
            Mat f20=ocr.features(img, 20);

            trainingDataf5.push_back(f5);
            trainingDataf10.push_back(f10);
            trainingDataf15.push_back(f15);
            trainingDataf20.push_back(f20);
            trainingLabels.push_back(i);
        }
    }

    
    trainingDataf5.convertTo(trainingDataf5, CV_32FC1);
    trainingDataf10.convertTo(trainingDataf10, CV_32FC1);
    trainingDataf15.convertTo(trainingDataf15, CV_32FC1);
    trainingDataf20.convertTo(trainingDataf20, CV_32FC1);
    Mat(trainingLabels).copyTo(classes);

    FileStorage fs("OCR.xml", FileStorage::WRITE);
    fs << "TrainingDataF5" << trainingDataf5;
    fs << "TrainingDataF10" << trainingDataf10;
    fs << "TrainingDataF15" << trainingDataf15;
    fs << "TrainingDataF20" << trainingDataf20;
    fs << "classes" << classes;
    fs.release();

    return 0;
}

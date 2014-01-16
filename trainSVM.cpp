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

// modified by @f 2013/8/25
// modified by @f 2013/9/06 support variable file name

// Main entry code OpenCV

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include <dirent.h>	// @f

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main ( int argc, char** argv )
{
    cout << "OpenCV Training SVM Automatic Number Plate Recognition\n";
    cout << "modified by @f at 2013/8/25\n";
    cout << "\n";

    char* path_Plates;
    char* path_NoPlates;
//    int numPlates = -1;		// @f 
//    int numNoPlates = -1;	// @f 
    int imageWidth=144;
    int imageHeight=72; // original is 33

    // definition for Directory List
    DIR 		*dir;
    struct dirent	*entry;

    //Check if user specify image to process
    if(argc >= 3 )	// @f 
    {
        path_Plates= argv[1];	// @f 
        path_NoPlates= argv[2];	// @f 

    }else{
        cout << "Usage:\n" << argv[0] << " <path to plate folder files> <path to non plate files> \n";	// @f
        return 0;
    }        


    // get number of files
//    numPlates= -1;
//    numNoPlates= -1;

    Mat classes;//(numPlates+numNoPlates, 1, CV_32FC1);
    Mat trainingData;//(numPlates+numNoPlates, imageWidth*imageHeight, CV_32FC1 );

    Mat trainingImages;
    vector<int> trainingLabels;

    // Training Plates ----------------------------------
    if ((dir = opendir(path_Plates)) == NULL) {
	perror(argv[0]);
	return	(-1);
    }
//    for(int i=0; i< numPlates; i++)
    while ((entry = readdir(dir)) != NULL)
    {
	if (strlen(entry->d_name) < 5 || strcmp(entry->d_name + strlen(entry->d_name) - 4, ".jpg")) continue;

        stringstream ss(stringstream::in | stringstream::out);
	cout << "--- open \"" << path_Plates << entry->d_name << "\"\n";
        ss << path_Plates << entry->d_name;
        Mat img=imread(ss.str(), 0);
        img= img.reshape(1, 1);
        trainingImages.push_back(img);
        trainingLabels.push_back(1);
    }

    // Training NoPlates ----------------------------------
    if ((dir = opendir(path_NoPlates)) == NULL) {
	perror(argv[0]);
	return	(-1);
    }
//    for(int i=0; i< numNoPlates; i++)
    while ((entry = readdir(dir)) != NULL)
    {
	if (strlen(entry->d_name) < 5 || strcmp(entry->d_name + strlen(entry->d_name) - 4, ".jpg")) continue;

        stringstream ss(stringstream::in | stringstream::out);
	cout << "--- open \"" << path_NoPlates << entry->d_name << "\"\n";
        ss << path_NoPlates << entry->d_name;
        Mat img=imread(ss.str(), 0);
        img= img.reshape(1, 1);
        trainingImages.push_back(img);
        trainingLabels.push_back(0);

    }

    Mat(trainingImages).copyTo(trainingData);
    //trainingData = trainingData.reshape(1,trainingData.rows);
    trainingData.convertTo(trainingData, CV_32FC1);
    Mat(trainingLabels).copyTo(classes);

    FileStorage fs("SVM.xml", FileStorage::WRITE);
    fs << "TrainingData" << trainingData;
    fs << "classes" << classes;
    fs.release();

    return 0;
}

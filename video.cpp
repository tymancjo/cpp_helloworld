#include <iostream>
#include <fstream>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include "opencv4/opencv2/opencv.hpp"


//////////////////////////////
// the function definitions //
//////////////////////////////

struct results {
    bool sucess;
    std::vector<std::vector<std::string>> strMatrix;
    std::vector<std::vector<float>> fMatrix;
};

results readFileAsText(std::string sFilename,  char deliminator = ';')
	{
        results output;
        
        std::ifstream f(sFilename);
        if (!f.is_open()){
            output.sucess = false;
            return output;
        }

		while (!f.eof())
		{   std::string line;
            std::getline(f, line);
            
            if (!line.empty()){
                if (line.at(0) != '#'){
                    std::istringstream iss(line);
                    std::string token;
                    
                    std::vector < std::string> lins;
                    while(std::getline(iss, token, deliminator)){
                        boost::trim(token);
                        boost::replace_all(token,",",".");
                        lins.push_back(token);
                    }

                    output.strMatrix.push_back(lins);
	    }}}
        output.sucess = true;
        return output;
    };


results parseCsvData(std::vector<std::vector<std::string>> inputText, int startRow=9){
    results output;

    // first I need to figure out how many columns in data

    // so the idea is to make the size first
    // so I'm adding 0 to each column vector
    for (int col=0; col < inputText[startRow].size(); col++){
        std::vector<float> C;
        C.push_back(0);
        output.fMatrix.push_back(C);
        C.pop_back();
    }

    for (int row=startRow; row < inputText.size(); row++){
        for (int col=0; col < inputText[row].size()-1; col++){
            std::stringstream iss( inputText[row][col] );
            float val;
            if (iss >> val) {
                output.fMatrix[col].push_back(val);
            } else {
                output.sucess = false;
                return output;
            }
        }
    }
    output.sucess = true;
    return output;
}

std::vector<float> normalizeFloatVec(std::vector<float> inputVector){
    std::vector<float> M;
    float max = 0;

    M.push_back(0.0);

    for (int el=0; el < inputVector.size(); el++){
        max = (abs(inputVector[el]) > max)? abs(inputVector[el]) : max;
    }

    for (int el=0; el < inputVector.size(); el++){
        M.push_back(inputVector[el] / max);
    }
    return M;
}

///////////////////
// the main loop //
///////////////////

int main(int argc, char *argv[])
{
    std::string input_file = "/home/tymancjo/LocalGit/ABB/ptviewer/video/15122020_Jacek_M_GErapid_11419.mp4";
    cv::VideoCapture cap(input_file);

    if(!cap.isOpened())  // check if we succeeded
        return -1;

    // the csv file load drill
    std::vector <std::vector<std::string>> fileData;
    std::vector <std::vector<float>> fileFloatData;
    std::string file_to_load = "/home/tymancjo/LocalGit/ABB/ptviewer/csv_data/fileRecording11419.txt";

    results fOutput;
    fOutput = readFileAsText(file_to_load, ';' );

    if(!fOutput.sucess){
        std::cout << "[ERROR] issue with data file..." << std::endl;
        return -2;
    }

    fileData = fOutput.strMatrix;

    fOutput = parseCsvData(fileData, 9);
    if(!fOutput.sucess){
        std::cout << "[ERROR] issue with data parse..." << std::endl;
        return -3;
    }
    
    fileFloatData = fOutput.fMatrix;

    //for (int col=0; col < fileFloatData.size(); col++){
        //std::vector<float> V;
        //V = normalizeFloatVec(fileFloatData[col]);
        //fileFloatData[col] = V;
    //}

    // Data plot just to check if this worked well
    //for(int row=0; row < fileFloatData[0].size(); row++){
    for(int row=0; row < 5; row++){
        for(int col=0; col < fileFloatData.size(); col++){
            std::cout << fileFloatData[col][row] << ' ';
        }
        std::cout << std::endl;
    }

     //The experimental idea to read all frames as jpg to array
    //std::vector <cv::Mat> all_frames;

    //int start_frame = 500;
    //int end_frame = 1500;

    //for(int frameNum = 0; frameNum < cap.get(cv::CAP_PROP_FRAME_COUNT);frameNum++)
        //{
            //cv::Mat frame;
            //cap >> frame;  get the next frame from video
            //if (frameNum > start_frame)
                //all_frames.push_back(frame);
            //if (frameNum > end_frame)
                //break;
        //}

    //cv::namedWindow("Video",1);

    //int klatka = 0;
    //int step = 1;

    //while (true){
        //cv::putText(all_frames[klatka], std::to_string(klatka), cv::Point(10,10), cv::FONT_HERSHEY_SIMPLEX,0.4,cv::Scalar(200,200,200),1,cv::LINE_AA);
        //cv::imshow("Video", all_frames[klatka]);
        //klatka += step;

        //if (klatka == all_frames.size()){
            //step = -1;
            //klatka = all_frames.size()-1;
        //}
        //if (klatka < 0){
            //step = 1;
            //klatka = 0;
        //}
        //if(cv::waitKey(1) == 27) break;
    //}


    //for(;;)
    //{
        //cv::Mat frame;
        //cap >> frame; // get a new frame from camera        
        //cv::imshow("Video", frame);
        //if(cv::waitKey(30) == 27) break;
    //}
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

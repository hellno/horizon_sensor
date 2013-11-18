#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct pixel
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
};

ifstream inputPic;
ofstream outputPic;

const int width = 1024;
const int height = 1024;

pixel pxlMatrix[width][height];
bool bwMatrix[width][height];

/* returns file size in kb
*/
int getFileSize(string filename){
	long begin,end;
	inputPic.close();
	inputPic.open(filename);
  	begin = inputPic.tellg();
  	inputPic.seekg(0,ios::end); 
  	end = inputPic.tellg();
  	inputPic.close();
  	return (end-begin);
}

int pixelColorAvg(pixel p){
	return (p.r + p.g + p.b)/3;
}

void printPixel(pixel p){
	printf("RGB: %d|%d|%d     \r",p.r,p.g,p.b);
	printf("AVG: %d", pixelColorAvg(p));
}

string appendToFilename(string filename, string addition){
	int pos = filename.find_first_of(".");
	return filename.insert(pos, addition);
}

void importImageFile(string filename, int imageStart){
	inputPic.close();
	inputPic.open(filename);
	inputPic.seekg(0,ios_base::beg);
	for (int i=0; i < 10; i++){
		inputPic.get();
	}
	for(int x=0;x < width; x++){
		for(int y = 0;y<height;y++){
			pixel p;
			p.r = inputPic.get();
			p.g = inputPic.get();
			p.b = inputPic.get();
			printPixel(p);
			pxlMatrix[x][y] = p;
		}
	}
	printf("\n");
	inputPic.close();
}

void writePixelWithValue(unsigned int val){
	outputPic.put(val);
	outputPic.put(val);
	outputPic.put(val);
}

void writeBlackPixel(){
	writePixelWithValue((unsigned int)0);
}

void writeWhitePixel(){
	writePixelWithValue((unsigned int)255);
}
/*
	1 or 0 if bigger/smaller than threshold
*/
void createBinaryImage(int threshold){
	for(int x=0;x < width; x++){
		for(int y = 0;y<height;y++){
			bwMatrix[x][y] = (pixelColorAvg(pxlMatrix[x][y])  > threshold);
			printf("BW: %d  \r", bwMatrix[x][y]);
		}
	}
}

void writeBWImageToFile(string filename, int imageStart){
	string outputFilename = appendToFilename(filename, "BW");
	printf("imgStart: %d\nin: %s, out: %s\n", 
		imageStart, filename.c_str(), outputFilename.c_str());
	int cnt = 0;
	inputPic.close();
	outputPic.close();
	
	outputPic.open(outputFilename);
	inputPic.open(filename);
	inputPic.seekg(0,ios_base::beg);
	
	for(int i = 0; i < imageStart; i++){
		outputPic.put(inputPic.get());
	}
	for(int x = 0;x < width; x++){
		for(int y = 0;y < height; y++){
			if(bwMatrix[x][y]){
				//writeWhitePixel();
				outputPic.put(255);
				outputPic.put(255);
				outputPic.put(255);
				cnt++;
			} else{
				//writeBlackPixel();
				outputPic.put(0);
				outputPic.put(0);
				outputPic.put(0);
			}
		}
	}
	printf("%d white pixels written\n", cnt);
	outputPic.close();
	inputPic.close();
}

void erodeImage(int threshold, int pixelCount){

}

void subtractImages(string filenameA, string filenameB, string outputFilename){

}

int findCircleCenter(){
	return 0;
}

int calcEarthVector(int coordinates){

	return 0;
}

int imageStartInBMP(string filename){
	inputPic.close();
	inputPic.open(filename);
	for(int a=0; a<10; a++)
    {
        inputPic.get();
    }
    return inputPic.get();
}

int main(){
	string filename = "Mars.bmp";
	int imageStart = imageStartInBMP(filename);
	inputPic.close();

	printf("imageStart: %d\n", imageStart);
	int size = getFileSize(filename);
	printf("%s: %dkb\n", filename.c_str(), size/1000);
	importImageFile(filename, imageStart);
	createBinaryImage(8);
	writeBWImageToFile(filename, imageStart);
	return 0;
}

// void printImageByteValues(string filename){
// 	inputPic.close();
	
// 	char c;
// 	int cnt=0;

// 	inputPic.open(filename);
// 	long start = inputPic.tellg();
// 	inputPic.close();
// 	int filesize = getFileSize(filename);
// 	inputPic.open(filename);
// 	while (inputPic.good()){
//         cnt++;
//         bool val = (cnt) > start && (cnt)<filesize-1;
//         c = inputPic.get();
//         if(val) {
//         	printf("CurCnt: %d %d ", cnt, val);
//            	std::cout << std::hex << (short unsigned int)c << '\n';
//            	//printf("%d\n",(short unsigned int)c);	
//         }
//     }

//     printf("Anzahl Bytes: %d\n", cnt);
//     printf("start: %ld\n", start);
// 	printf("filesize: %d\n", filesize);
// }

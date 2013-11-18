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
bool erodeMatrix[width][height];
bool circleMatrix[width][height];

/* returns file size in kb
*/
int getFileSize(string filename){
	long begin,end;
	// inputPic.close();
	inputPic.open(filename);
  	begin = inputPic.tellg();
  	inputPic.seekg(0,ios::end); 
  	end = inputPic.tellg();
  	// inputPic.close();
  	return (end-begin);
}

int pixelColorAvg(pixel p){
	return (p.r + p.g + p.b)/3;
}

void printPixel(pixel p){
	printf("RGB: %d|%d|%d     \r",p.r,p.g,p.b);
	//printf("AVG: %d", pixelColorAvg(p));
}

string appendToFilename(string filename, string addition){
	int pos = filename.find_first_of(".");
	return filename.insert(pos, addition);
}

void importImageFile(string filename, int imageStart){
	inputPic.close();
	inputPic.open(filename);
	inputPic.seekg(0,ios_base::beg);
	for (int i=0; i < imageStart; i++){
		inputPic.get();
	}
	for(int x=0;x < width; x++){
		for(int y = 0;y<height;y++){
			pixel p;
			p.r = inputPic.get();
			p.g = inputPic.get();
			p.b = inputPic.get();
			//printPixel(p);
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

int surroundingWhitePixels(int width , int height){
	int sum = 0;
	for(int i=-1; i<= 1;i++){
		for(int j=-1; j<= 1;j++){
			sum += bwMatrix[width + i][height + j];
		}
	}
	if(bwMatrix[width][height])
		sum -= 1;
	return sum;
}

void writeBWImageToFile(string filename, int imageStart){
	string outputFilename = appendToFilename(filename, "BW");
	printf("imgStart: %d\nin: %s, out: %s\n", 
		imageStart, filename.c_str(), outputFilename.c_str());
	int cnt = 0;

	outputPic.open(outputFilename);
	inputPic.open(filename);
	inputPic.seekg(0,ios_base::beg);
	
	for(int i = 0; i < imageStart; i++){
		outputPic.put(inputPic.get());
	}
	for(int x = 0;x < width; x++){
		for(int y = 0;y < height; y++){
			if(bwMatrix[x][y]){
				writeWhitePixel();
				cnt++;
			} else{
				writeBlackPixel();
			}
		}
	}
	printf("%d white pixels written\n", cnt);
	outputPic.close();
	inputPic.close();
}

void writeErodedImageToFile(string filename, int imageStart){
	string outputFilename = appendToFilename(filename, "Erod");
	printf("imgStart: %d\nin: %s, out: %s\n", 
		imageStart, filename.c_str(), outputFilename.c_str());
	int cnt = 0;

	outputPic.open(outputFilename);
	inputPic.open(filename);
	inputPic.seekg(0,ios_base::beg);
	
	for(int i = 0; i < imageStart; i++){
		outputPic.put(inputPic.get());
	}
	for(int x = 0;x < width; x++){
		for(int y = 0;y < height; y++){
			if(erodeMatrix[x][y]){
				writeWhitePixel();
				cnt++;
			} else{
				writeBlackPixel();
			}
		}
	}
	printf("%d white pixels written\n", cnt);
	outputPic.close();
	inputPic.close();
}

void writeCircleImageToFile(string filename, int imageStart){
	string outputFilename = appendToFilename(filename, "Circle");
	printf("imgStart: %d\nin: %s, out: %s\n", 
		imageStart, filename.c_str(), outputFilename.c_str());
	int cnt = 0;

	outputPic.open(outputFilename);
	inputPic.open(filename);
	inputPic.seekg(0,ios_base::beg);
	
	for(int i = 0; i < imageStart; i++){
		outputPic.put(inputPic.get());
	}
	for(int x = 0;x < width; x++){
		for(int y = 0;y < height; y++){
			if(circleMatrix[x][y]){
				writeWhitePixel();
				cnt++;
			} else{
				writeBlackPixel();
			}
		}
	}
	printf("%d white pixels written\n", cnt);
	outputPic.close();
	inputPic.close();
}

/*
	1 or 0 if bigger/smaller than threshold
*/
void createBinaryImage(int threshold){
	int cnt = 0;
	for(int x=0;x < width; x++){
		for(int y = 0;y<height;y++){
			bwMatrix[x][y] = (pixelColorAvg(pxlMatrix[x][y])  > threshold);
			if(bwMatrix[x][y]){
				cnt++;
			}
			//printf("BW: %d  \r", bwMatrix[x][y]);
		}
	}
	printf("%d write pixels saved\n", cnt);
}

void erodeImage(int threshold){
	int whiteCount = 0;
	for(int x = 1;x < width - 1; x++){
		for(int y = 1;y < height - 1; y++){
			if(bwMatrix[x][y]){
				whiteCount = surroundingWhitePixels(x,y);
				erodeMatrix[x][y] = (whiteCount > threshold);
			}else{
				erodeMatrix[x][y] = false;
			}
		}
	}
}

void subtractErodedFromBW(void){
	for(int x = 0;x < width; x++){
		for(int y = 0;y < height; y++){
			circleMatrix[x][y] = bwMatrix[x][y] - erodeMatrix[x][y];
		}
	}
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
	erodeImage(7);
	//writeErodedImageToFile(filename, imageStart);
	subtractErodedFromBW();
	writeCircleImageToFile(filename, imageStart);
	return 0;
}
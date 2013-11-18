#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <math.h>

using namespace std;

struct pixel
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
};

struct coordinates
{
	int x;
	int y;
};

ifstream inputPic;
ofstream outputPic;

const int width = 1024;
const int height = 1024;

const double pxSize = 1e-5;
const long int periapsis = 36000000;
const long int marsDiameter = 6779000;
const long int earthDiameter  = 12742000;

double radius;

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

void printCoord(coordinates c){
	printf("(%d|%d)\n",c.x,c.y);
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
	printf("imgStart: %d in: %s, out: %s\n", 
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
	printf("imgStart: %d in: %s, out: %s\n", 
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

void writeCircleImageToFileWithCenter(string filename, int imageStart, int centerX=0, int centerY=0){
	string outputFilename = appendToFilename(filename, "Circle");
	printf("imgStart: %d in: %s, out: %s\n", 
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
			if(x == centerX && y == centerY){
				outputPic.put(0);
				outputPic.put(0);
				outputPic.put(255);
			}
			else if(circleMatrix[x][y]){
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

int findWhitePixelInRow(int row, int start = 0){
	if(row < 0 || row > height){
		return 0;
	}
	int i=1;
	while(!(circleMatrix[row][start + i] &&(i > 20))){
		i++;
	}
	return start + i;
}

int findWhitePixelInCol(int col, int start = 0){
	if(col < 0 || col > width){
		return 0;
	}
	int i=1;
	while(!(circleMatrix[start + i][col] && (i > 20))){
		i++;
	}
	return start + i;
}

int centerOfTwoPixels(int first, int second){
	return abs(first + second)/2;
}

double radiusWithCenter(coordinates center){
	int i=0;
	double radius = 0;
	while(!circleMatrix[center.x - i][center.y]){
		i++;
	}
	radius = i;
	i = 0;
	while(!circleMatrix[center.x][center.y + i]){
		i++;
	}
	radius = (radius+i)/2.0;
	printf("Radius: %f\n",radius);
	return radius;
}

vector<coordinates> createEdgeList(void){
	vector<coordinates> edgeVector;
	int cnt = 0;
	for(int x = 0;x < width; x++){
		for(int y = 0;y < height; y++){
			if(circleMatrix[x][y]){
				coordinates newC;
				newC.x = x;
				newC.y = y;
				edgeVector.push_back(newC);
				cnt++;
			}
		}
	}
	printf("Edge contains %d px\n", cnt);
	return edgeVector;
}

coordinates randomCoordinatesAroundCenter(coordinates center){
	coordinates randCoord;
	//center +- 1 in every direction 
	randCoord.x = center.x + 16.0 * rand() /((double) RAND_MAX) - 8.0;
	randCoord.y = center.y + 16.0 * rand() /((double) RAND_MAX) - 8.0;
	//printCoord(randCoord);
	return randCoord;
}

double leastSquare(int radiusSquared, coordinates testCenter, coordinates edgeCoord){
	return pow(sqrt(pow(edgeCoord.y-testCenter.y,2)+pow(edgeCoord.x-testCenter.x,2)),2);
}

void createBinaryImage(int threshold){
	int cnt = 0;
	for(int x=0;x < width; x++){
		for(int y = 0;y<height;y++){
			bwMatrix[x][y] = (pixelColorAvg(pxlMatrix[x][y])  > threshold);
			if(bwMatrix[x][y]){
				cnt++;
			}
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

coordinates findCircleCenter(){
	coordinates center;
	center.x = -1;
	center.y = -1;
	int firstPixel = findWhitePixelInCol(height / 2);
	int secondPixel = findWhitePixelInCol(height / 2, firstPixel);
	center.x = centerOfTwoPixels(firstPixel, secondPixel);
	printf("_X First: %d, Center: %d, Second: %d\n", firstPixel, center.x, secondPixel);
	firstPixel = findWhitePixelInRow(width / 2);
	secondPixel = findWhitePixelInRow(width / 2, firstPixel);
	center.y = centerOfTwoPixels(firstPixel, secondPixel);
	printf("_Y First: %d, Center: %d, Second: %d\n", firstPixel, center.y, secondPixel);
	radius = radiusWithCenter(center);
	double radiusSquared = pow(radius, 2);
	vector<coordinates> edgeList = createEdgeList();

	coordinates newCenter;
	newCenter.x = center.x;
	newCenter.y = center.y;
	double currMinSum = std::numeric_limits<double>::max();
	srand( time( NULL ) ); 
	for(int i = 0; i < 1000; i++){
		double distSum = 0;
		coordinates testCenter = randomCoordinatesAroundCenter(newCenter);
		for(int j = 0; j < edgeList.size(); j++){
			distSum += leastSquare(radiusSquared, testCenter, edgeList[j]);
		}
		if(distSum < currMinSum){
			currMinSum = distSum;
			newCenter.x = testCenter.x;
			newCenter.y = testCenter.y;
			printf("new center");
			printCoord(newCenter);
		}
	}
	return newCenter;
}

int calcEarthVector(coordinates coord, double radius){
	double diameter = 2.0 * radius;
	double imageDiameter = diameter * pxSize;

	double focalDistance = (imageDiameter * periapsis) / earthDiameter;
	double fov = 2 * atan2(pxSize * height, focalDistance * 2);
	printf("Brennweite: %f\n", focalDistance);
	printf("Sichtfeld: %f\n", (fov / M_PI) * 180);
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
	string filename = "mars.bmp";
	int imageStart = imageStartInBMP(filename);
	inputPic.close();

	printf("imageStart: %d\n", imageStart);
	
	int size = getFileSize(filename);
	printf("%s: %dkb\n", filename.c_str(), size/1000);
	
	importImageFile(filename, imageStart);
	createBinaryImage(8);
	writeBWImageToFile(filename, imageStart);
	erodeImage(7);
	writeErodedImageToFile(filename, imageStart);
	subtractErodedFromBW();
	writeCircleImageToFileWithCenter(filename, imageStart);
	coordinates center = findCircleCenter();
	writeCircleImageToFileWithCenter(filename, imageStart, center.x, center.y);
	calcEarthVector(center, radius);
	return 0;
}
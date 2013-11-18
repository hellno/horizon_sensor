//Uebung 2 LRD Beispiel

#include <stdio.h>
#include <fstream>
#include <iostream>
using namespace std;
int main() {
    char c;
    int cnt=0;
    ifstream mypic;
    ofstream myoutpic;
    mypic.open("Erde.bmp");
    myoutpic.open("ErdeOut.bmp");
    /****Ermittlung der Bildgröße***/
    for(int a=0; a<10; a++)
    {
        mypic.get();
    }
    int start=mypic.get();	   			//Offset der Bilddaten in Byte vom Beginn der Datei an
    mypic.seekg(0,ios::end);   				//Suche Ende der Datei
    int filesize=mypic.tellg();				//Dateigröße
    mypic.close();

    /****Bildbearbeitung****/
    mypic.open("Erde.bmp");
    while (mypic.good())    				// Solange extrahierte Daten gültig
    {
        cnt++;
        c = mypic.get();       				// lese Byte aus der Datei
        if (mypic.good())
            //printf("%d\n\r",(short unsigned int)c);		// Gebe Bytewert aus
            if((cnt)>start&&(cnt)<filesize-1)   		// Solange wir im Bildbereich sind
            {
                myoutpic.put(c+150);			// Addiere Offset auf jeden Pixelwert
            }
            else
            {
                myoutpic.put(c);
            }
    }
    printf("C:%d\n",sizeof c);
    mypic.close();           // schließe Datei
    myoutpic.close();
    printf("Anzahl Bytes: %d\n\rFilesize:%d\n\rStart:%d\n\r",cnt,filesize,start);
    return 0;



}


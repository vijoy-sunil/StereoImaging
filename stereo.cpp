

#define IMAGE_W     320
#define IMAGE_H     240

#include <fstream>
using namespace std;
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Stereo functions in Stereo.cu.

extern "C" void stereoInit( int w, int h );
extern "C" void stereoUpload( const unsigned char *left, const unsigned char *right );
extern "C" void stereoProcess();
extern "C" void stereoDownload( float *disparityLeft, float *disparityRight );


bool readPGM( const char *filename, unsigned char *im, int w, int h )
{
    std::ifstream in;
    int state;
    char token[1024];
    int ww,hh,r;
    
    in.open(filename,std::ios_base::in|std::ios_base::binary);
    if(!in.is_open())
        return false;

    state = 0;
    while(!in.eof() && state<4) {
        in >> token;
        if(token[0]=='#')
            in.getline(token,sizeof(token));
        else {
            switch(state) {
                case 0:
                    if(strcmp(token,"P5")!=0)
                        return false;
                    state++;
                    break;
                case 1:
                    ww = atoi(token);
                    state++;
                    break;
                case 2:
                    hh = atoi(token);
                    state++;
                    break;
                case 3:
                    r = atoi(token);
                    state++;
                    break;
            }
        }
    }
    if(w!=ww || h!=hh)
        return false;

    in.read((char*)im,1);
    in.read((char*)im,w*h);
    return true;
}

bool writeDisparityPPM( const char *filename, const float *disp, int w, int h )
{
    std::ofstream out;
    float dmin,dmax,d;
    int x,y;

    dmin = FLT_MAX;
    dmax = -FLT_MAX;
    for(y=0; y<h; y++) {
        for(x=0; x<w; x++) {
            d = disp[y*w+x];
            if(d==255)
                continue;
            if(d<dmin)
                dmin = d;
            if(d>dmax)
                dmax = d;
        }
    }

    out.open(filename,std::ios_base::out|std::ios_base::binary);
    if(!out.is_open())
        return false;
    out << "P6\n" << w << " " << h << "\n255\n";
    for(y=0; y<h; y++) {
        for(x=0; x<w; x++) {
            unsigned char r,g,b;
            d = disp[y*w+x];
            if(d==255) {
                r = 255;
                g = 0;
                b = 0;
            } else {
                r = (unsigned char)(255*d/(dmax-dmin));
                g = b = r;
            }
            out.write((char*)&r,1);
            out.write((char*)&g,1);
            out.write((char*)&b,1);
        }
    }
    return true;
}

int main()
{
    unsigned char *imLeft, *imRight;
    float *dispLeft, *dispRight;
           
    imLeft = new unsigned char[IMAGE_W*IMAGE_H];
    imRight = new unsigned char[IMAGE_W*IMAGE_H];
    dispLeft = new float[IMAGE_W*IMAGE_H];
    dispRight = new float[IMAGE_W*IMAGE_H];
    
    readPGM("./left.pgm",imLeft,IMAGE_W,IMAGE_H);
    readPGM("./right.pgm",imRight,IMAGE_W,IMAGE_H);

    stereoInit(IMAGE_W,IMAGE_H);
    stereoUpload(imLeft,imRight);
    stereoProcess();
    stereoDownload(dispLeft,dispRight);


    writeDisparityPPM("./left-disparity.ppm",dispLeft,IMAGE_W,IMAGE_H);
    writeDisparityPPM("./right-disparity.ppm",dispRight,IMAGE_W,IMAGE_H);

    return 0;
}

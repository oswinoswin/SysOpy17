#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BUF 20
/*Mandelbrot code is taken from rhosettacode.org*/

int main(int argc, char *argv[])
{

    if(argc != 4){
        fprintf(stderr, "Usage [filename][N][K]\n");
        EXIT_FAILURE;
    }
    int fd;
    char * myfifo = argv[1];



    int N = atoi(argv[2]);
    int K = atoi(argv[3]);

    char buffer[MAX_BUF];

    fd = open(myfifo, O_WRONLY);


    ///Mandelbrot////////////////////////////////////////////////
    /* screen ( integer) coordinate */
    int iX,iY;
    const int iXmax = K;//800;
    const int iYmax = K;//800;
    /* world ( double) coordinate = parameter plane*/
    double Cx,Cy;
    const double CxMin=-2.5;
    const double CxMax=1.5;
    const double CyMin=-2.0;
    const double CyMax=2.0;
    /* */
    double PixelWidth=(CxMax-CxMin)/iXmax;
    double PixelHeight=(CyMax-CyMin)/iYmax;
    /* Z=Zx+Zy*i  ;   Z0 = 0 */
    double Zx, Zy;
    double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */

    int Iteration;
    const int IterationMax=K;
    /* bail-out value , radius of circle ;  */
    const double EscapeRadius=2;
    double ER2=EscapeRadius*EscapeRadius;
    /*create new file,give it a name and open it in binary mode  */
    srand(time(NULL));

    for(int i = 0; i<N; i++){

        //generate random points
        iX = rand()%K;
        iY = rand()%K;

        Cy=CyMin + iY*PixelHeight;
        if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
        Cx=CxMin + iX*PixelWidth;
        /* initial value of orbit = critical point Z= 0 */
        Zx=0.0;
        Zy=0.0;
        Zx2=Zx*Zx;
        Zy2=Zy*Zy;

        for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
        {
            Zy=2*Zx*Zy + Cy;
            Zx=Zx2-Zy2 +Cx;
            Zx2=Zx*Zx;
            Zy2=Zy*Zy;
        };



        sprintf(buffer,"%d %d %d\n",iX, iY, Iteration);
        write(fd, buffer, MAX_BUF);

    }




    /* compute and write image data bytes to the file*/

    for(iY=0;iY<iYmax;iY++)
    {

        for(iX=0;iX<iXmax;iX++)
        {
            Cy=CyMin + iY*PixelHeight;
            if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
            Cx=CxMin + iX*PixelWidth;
            /* initial value of orbit = critical point Z= 0 */
            Zx=0.0;
            Zy=0.0;
            Zx2=Zx*Zx;
            Zy2=Zy*Zy;
            /* */
            for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
            {
                Zy=2*Zx*Zy + Cy;
                Zx=Zx2-Zy2 +Cx;
                Zx2=Zx*Zx;
                Zy2=Zy*Zy;
            };



            sprintf(buffer,"%d %d %d\n",iX, iY, Iteration);
            write(fd, buffer, MAX_BUF);
            //printf("%s\n",buffer);

        }

    }

    close(fd);



    return 0;
}



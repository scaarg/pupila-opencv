// PROGRAMA PARA MEDIR EL DIAMETRO DE UNA PUPILA CON OPENCV 
// Desarrollado por Oscar Garcia Oviedo, estudiante de la
// Universidad Autonoma de Ciudad Juarez en Mexico. 
// Profesor: M.I. Hiram Madero.
// Visto en http://www.acentoenlao.com


#include <iostream>

/// OPENCV LIBS /// -----------------------------------

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
    /// IMAGEN  /// -----------------------------------

    Mat imagen;     // Matriz que guardara la imagen original

    imagen = imread("DIRRECCION-DE-LA-IMAGEN-ORIGINAL");    // CAMBIAR DIRECCION!!

    // imshow("Original",imagen);

    /// EFECTOS /// -----------------------------------

    Mat imagen_aperture;    // Matriz que guardara la imagen procesada
    Mat elemento = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(0, 0));   // Estructura del kernel

    dilate(imagen, imagen_aperture, elemento);
    erode(imagen_aperture, imagen_aperture, elemento);

    // imshow("Apertura", imagen_aperture);


    Mat imagen_aperture2;

    erode(imagen_aperture, imagen_aperture2, elemento);
    dilate(imagen_aperture2, imagen_aperture2, elemento);

    // imshow("Cierre", imagen_aperture2);
    

    /// THRESHOLD /// -----------------------------------

    Mat imagen_thr;

    threshold(imagen_aperture2, imagen_thr, 30, 255, CV_THRESH_BINARY);

    // imshow("Threshold", imagen_thr);


    /// CIRCULOS  /// ------------------------------------

    Mat imagen_gris;

    cvtColor(imagen_thr, imagen_gris, CV_RGB2GRAY, 1); // Conversion a escala de grises

    // imshow("Grises", imagen_gris);

    Mat imagen_blur;

    GaussianBlur(imagen_gris, imagen_blur, Size(5, 5), 1.5); // Blur gaussiano

    // imshow("GaussianBlur", imagen_blur);

    vector<Vec3f> circulos;

    HoughCircles(imagen_blur, circulos, CV_HOUGH_GRADIENT,
                     2,
                     50,        // Distancia minima entre dos circulos
                     100,       // Canny Thr
                     100,
                     30, 50);   // Radio minimo y maximo

        vector<Vec3f>::const_iterator itc = circulos.begin();

        while(itc != circulos.end()){

            circle(imagen_blur,
                   Point((*itc)[0], (*itc)[1]),     // Centro del circulo
                    (*itc)[2],                      // Radio del circulo
                    Scalar(127),                    // Color del circulo
                    2);                             // Grosor del circulo

            cout << "Diametro: " << (*itc)[2] * 2 << endl;
            ++itc;

        }


    /// RESULTADO /// -----------------------------------

    imshow("Circulos", imagen_blur);    // Mostrar imagen resultado

    cvWaitKey(0);

}

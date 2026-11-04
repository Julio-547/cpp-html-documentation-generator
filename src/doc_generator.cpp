#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main() {
    bool bloque=false;
    string Nombre, Nombre_ArchEnt, Nombre_ArchSal, linea, tipoDoc;
    ifstream ArchEnt;
    ofstream ArchSal;

    cout << "Ingresa el nombre del archivo a documentar "; /*saca el nombre del archivo .cpp, lo abre y se lo asigna al html tambien*/
    cin >> Nombre;
    Nombre_ArchEnt = Nombre + ".cpp";
    Nombre_ArchSal = Nombre + ".html";
    ArchEnt.open(Nombre_ArchEnt);
    if (!ArchEnt.is_open()) {    //Te avisa si no pudiste abrir el archivo
        cout << "No se pudo abrir el archivo" << endl;
        return 1; //Detiene el programa si no se abre el archivo y no continua con la ejecucion del while
    }
    ArchSal.open(Nombre_ArchSal);
    
    ArchSal << "<!DOCTYPE html>" << endl; //Plantilla principal del HTML
    ArchSal << "<html>" << endl;
    ArchSal << "<head>" << endl;
    ArchSal << "<title>" << endl;
    ArchSal << "Documentaci&oacute;n del archivo " << Nombre_ArchEnt << endl;
    ArchSal << "</title>" << endl;
    ArchSal << "</head>" << endl;
    ArchSal << "<body>" << endl;

    while (getline(ArchEnt, linea)){
        int inicio = linea.find("/**");
        int final = linea.find("*/");
        if (inicio != -1){
            bloque = true; //Variable de control para empezar la lectura de documentacion
        } 
        else if(final != -1){ //Else if para detener la lectura de documentacion solo cuando se llega al fin del bloque
            bloque = false;
        }    

        if (bloque==true){

            int posParametro=linea.find("@")+1; // +1 para decir despues del arroba
            int posEspacio=linea.find(" ");
            tipoDoc = linea.substr(posParametro, posEspacio-posParametro); //Consigue el string del parametro sin el @  
            string cuerpo = linea.substr(posEspacio+1);

            if (tipoDoc == "progName"){
                ArchSal << "<h2>" << endl;
                ArchSal << "Programa: " << cuerpo << "<br>" << endl;
                ArchSal << "</h2>" << endl;
            }
            else if (tipoDoc == "desc"){
                ArchSal << "<strong> Descripci&oacute;n: </strong> " << cuerpo << "<br>" << endl;
            }
            else if (tipoDoc == "author"){
                ArchSal << "<strong> Autor: </strong> " << cuerpo << "<br>" << endl;
            }
            else if (tipoDoc == "date"){
                ArchSal << "<strong> Fecha de elaboraci&oacute;n </strong> " << cuerpo << "<br>" << endl;
            }
            else if (tipoDoc == "funcName"){
                ArchSal << "<h3>" << endl;
                ArchSal << "<hr><br>Funci&oacute;n: " << cuerpo << "<br>" << endl;
                ArchSal << "</h3>" << endl;
            }
            else if (tipoDoc == "param"){
                ArchSal << "<strong> Parametro: </strong> " << cuerpo << "<br>" << endl;
            }
            else if (tipoDoc == "return"){
                ArchSal << "<strong> Valor de retorno: </strong> " << cuerpo << "<br>" << endl;
            }
        }
    }
    
    ArchSal << "</body>" << endl;
    ArchSal << "</html>" << endl;

    ArchEnt.close();
    ArchSal.close();
    return 0;
}
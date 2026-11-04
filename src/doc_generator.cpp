/**
@progName Generador de documentacion HTML
@desc Lee un archivo .cpp, extrae sus bloques de documentacion y genera un archivo .html con el mismo nombre
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const string MARCA_INICIO = "/**";
const string MARCA_FIN = "*/";

/**
@funcName escaparHtml
@desc Reemplaza los caracteres especiales de HTML para que no rompan la pagina
@param texto cadena original
@return cadena con &, < y > convertidos a entidades HTML
*/
string escaparHtml(const string& texto) {
    string resultado;
    for (char c : texto) {
        if (c == '&')      resultado += "&amp;";
        else if (c == '<') resultado += "&lt;";
        else if (c == '>') resultado += "&gt;";
        else               resultado += c;
    }
    return resultado;
}

/**
@funcName recortar
@desc Quita espacios, tabuladores y saltos de linea al inicio y al final de una cadena
@param texto cadena original
@return cadena sin espacios en los extremos
*/
string recortar(const string& texto) {
    const string blancos = " \t\r\n";
    size_t inicio = texto.find_first_not_of(blancos);
    if (inicio == string::npos) {
        return "";
    }
    size_t fin = texto.find_last_not_of(blancos);
    return texto.substr(inicio, fin - inicio + 1);
}

/**
@funcName escribirEtiqueta
@desc Escribe en el HTML la etiqueta indicada. Las etiquetas desconocidas se ignoran
@param salida archivo HTML abierto para escritura
@param etiqueta nombre de la etiqueta sin el arroba (por ejemplo desc)
@param cuerpo texto que sigue a la etiqueta
*/
void escribirEtiqueta(ofstream& salida, const string& etiqueta, const string& cuerpo) {
    string texto = escaparHtml(cuerpo);

    if (etiqueta == "progName") {
        salida << "<h2>Programa: " << texto << "</h2>" << endl;
    } else if (etiqueta == "desc") {
        salida << "<strong>Descripci&oacute;n:</strong> " << texto << "<br>" << endl;
    } else if (etiqueta == "author") {
        salida << "<strong>Autor:</strong> " << texto << "<br>" << endl;
    } else if (etiqueta == "date") {
        salida << "<strong>Fecha de elaboraci&oacute;n:</strong> " << texto << "<br>" << endl;
    } else if (etiqueta == "funcName") {
        salida << "<hr>" << endl;
        salida << "<h3>Funci&oacute;n: " << texto << "</h3>" << endl;
    } else if (etiqueta == "param") {
        salida << "<strong>Par&aacute;metro:</strong> " << texto << "<br>" << endl;
    } else if (etiqueta == "return") {
        salida << "<strong>Valor de retorno:</strong> " << texto << "<br>" << endl;
    }
}

/**
@funcName procesarLinea
@desc Interpreta una linea de un bloque de documentacion. Acepta sangria y asterisco inicial (" * @desc ...")
@param salida archivo HTML abierto para escritura
@param contenido texto de la linea dentro del bloque
*/
void procesarLinea(ofstream& salida, const string& contenido) {
    string texto = recortar(contenido);

    // Quita el asterisco decorativo de los bloques estilo " * @desc ..."
    if (!texto.empty() && texto[0] == '*') {
        texto = recortar(texto.substr(1));
    }

    // Solo se procesan las lineas que empiezan con @
    if (texto.empty() || texto[0] != '@') {
        return;
    }

    size_t posEspacio = texto.find_first_of(" \t");
    string etiqueta;
    string cuerpo;
    if (posEspacio == string::npos) {
        etiqueta = texto.substr(1);   // etiqueta sin cuerpo, por ejemplo "@return"
    } else {
        etiqueta = texto.substr(1, posEspacio - 1);
        cuerpo = recortar(texto.substr(posEspacio));
    }
    escribirEtiqueta(salida, etiqueta, cuerpo);
}

/**
@funcName main
@desc Pide el nombre del archivo, lo recorre linea por linea y escribe el HTML
*/
int main() {
    string nombreBase;
    cout << "Ingresa el nombre del archivo a documentar (sin .cpp): ";
    getline(cin, nombreBase);
    nombreBase = recortar(nombreBase);

    // Permite escribir el nombre con o sin la extension
    const string extension = ".cpp";
    if (nombreBase.size() > extension.size() &&
        nombreBase.compare(nombreBase.size() - extension.size(), extension.size(), extension) == 0) {
        nombreBase.erase(nombreBase.size() - extension.size());
    }

    const string nombreEntrada = nombreBase + ".cpp";
    const string nombreSalida = nombreBase + ".html";

    // Se valida la entrada antes de crear la salida para no dejar HTML incompleto
    ifstream archivoEntrada(nombreEntrada);
    if (!archivoEntrada.is_open()) {
        cout << "No se pudo abrir el archivo " << nombreEntrada << endl;
        return 1;
    }

    ofstream archivoSalida(nombreSalida);
    if (!archivoSalida.is_open()) {
        cout << "No se pudo crear el archivo " << nombreSalida << endl;
        return 1;
    }

    archivoSalida << "<!DOCTYPE html>" << endl;
    archivoSalida << "<html>" << endl;
    archivoSalida << "<head>" << endl;
    archivoSalida << "<meta charset=\"UTF-8\">" << endl;
    archivoSalida << "<title>Documentaci&oacute;n del archivo " << escaparHtml(nombreEntrada) << "</title>" << endl;
    archivoSalida << "</head>" << endl;
    archivoSalida << "<body>" << endl;

    bool dentroDeBloque = false;
    string linea;
    while (getline(archivoEntrada, linea)) {
        string contenido = linea;

        // Un bloque puede abrir, cerrar o ambas cosas en la misma linea
        if (!dentroDeBloque) {
            size_t posInicio = linea.find(MARCA_INICIO);
            if (posInicio != string::npos) {
                dentroDeBloque = true;
                contenido = linea.substr(posInicio + MARCA_INICIO.size());
            }
        }

        if (dentroDeBloque) {
            size_t posFin = contenido.find(MARCA_FIN);
            bool cierraBloque = (posFin != string::npos);
            if (cierraBloque) {
                contenido = contenido.substr(0, posFin);
            }
            procesarLinea(archivoSalida, contenido);
            if (cierraBloque) {
                dentroDeBloque = false;
            }
        }
    }

    archivoSalida << "</body>" << endl;
    archivoSalida << "</html>" << endl;

    cout << "Documentacion generada: " << nombreSalida << endl;
    return 0;
}

// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica4.hpp"
#include "grafo-escena.hpp"


using namespace std ;

static unsigned objetoActivo4 = 0 ;
static constexpr int numObjetos4 = 1;
static NodoGrafoEscenaParam * objetos4[numObjetos4] = { nullptr };
static EscenaP4 * escena = NULL;
static unsigned int anguloActivo = 0; // Alpha: 0, Beta: 1
static ColeccionFuentesP4 * fuentes = NULL;
static unsigned fuenteActiva = 8;
static constexpr float diferencia = 3.0;



// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   fuentes = new ColeccionFuentesP4();
   objetos4[0] = escena = new EscenaP4();

   // Se asume que hay almenos una direccional
   for (unsigned int i = 0; i < fuentes->size() && fuenteActiva == 8; ++i)
      if (fuentes->ptrFuente(i)->esDir())
        fuenteActiva = i;
   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto: es true solo cuando se ha procesado alguna tecla
   unsigned antiguaFuente = fuenteActiva;
   FuenteLuz* fDir;

   switch ( toupper( tecla ) )
   {
      case 'G' :
          anguloActivo = (anguloActivo + 1) % 2;
          cout << "P4: Ángulo activo: " << (anguloActivo ? "Beta" : "Alpha") << endl;
          res = true;
         break ;

      case 'F' :
          for (unsigned int i = fuenteActiva + 1; i < fuentes->size() && fuenteActiva == antiguaFuente; ++i)
            if (fuentes->ptrFuente(i)->esDir())
              fuenteActiva = i;
          if (fuenteActiva == antiguaFuente)
            for (unsigned int i = 0; i < fuenteActiva + 1 && fuenteActiva == antiguaFuente; ++i)
              if (fuentes->ptrFuente(i)->esDir())
                fuenteActiva = i;
           cout << "P4: Fuente de luz direccional activa nº: " << fuenteActiva << endl;
           res = true;
           break;

      case '>' :
         fDir = fuentes->ptrFuente(fuenteActiva);
         fDir->variarAngulo(anguloActivo, diferencia);
         cout << "P4: Fuente nº " << fuenteActiva << ", angulo "
              << (anguloActivo ? "beta" : "alpha") << " vale ahora "
              << fDir->getAngulo(anguloActivo)  << endl;
         res = true;
         break ;

      case '<' :
          fDir = fuentes->ptrFuente(fuenteActiva);
          fDir->variarAngulo(anguloActivo, -diferencia);
          cout << "P4: Fuente nº " << fuenteActiva << ", angulo "
               << (anguloActivo ? "beta" : "alpha") << " vale ahora "
               << fDir->getAngulo(anguloActivo) << endl;
         res = true;
         break ;

     case 'O' :
        objetoActivo4 = (objetoActivo4 + 1) % numObjetos4;
        cout << "P4: nuevo objeto activo es: " << objetoActivo4;
        if (objetos4[objetoActivo4] != nullptr)
          cout << " (" << objetos4[objetoActivo4]->leerNombre() << ")." << endl;
        else
          cout << " (objeto no creado)" << endl;
        res = true;
        break ;

      default :
         break ;
   }
   return res ;

}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
  fuentes->activar(4);
  objetos4[objetoActivo4]->visualizarGL(cv);
}

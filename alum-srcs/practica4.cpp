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


// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   fuentes = new ColeccionFuentesP4();
   objetos4[0] = escena = new EscenaP4();

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

   switch ( toupper( tecla ) )
   {
      case 'G' :
          anguloActivo = (anguloActivo + 1) % 2;
          cout << "P4: Ángulo activo: " << (anguloActivo ? "Beta" : "Alpha") << endl;
         break ;

      case '>' :
         // COMPLETAR: práctica 4: incrementar el ángulo activo
         // ....

         break ;

      case '<' :
         // COMPLETAR: práctica 4: decrementar el ángulo activo
         // ....

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

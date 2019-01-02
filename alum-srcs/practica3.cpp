// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "grafo-escena.hpp"
#include "practicas.hpp"
#include "practica3.hpp"

using namespace std ;

static unsigned objetoActivo3 = 0 ;
static constexpr int numObjetos3 = 1;
static NodoGrafoEscenaParam * objetos3[numObjetos3] = { nullptr };
static bool animacionesOn = false;
static unsigned gradoActual = 0;

static Snowman * snowman = NULL;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;
    objetos3[0] = snowman = new Snowman;
   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto (true si se ha procesado tecla)
   float faux; // auxiliar para guardar valor actual para imprimirlo por pantalla
   Parametro * p = NULL; // puntero al parametro

   switch ( toupper( tecla ) )
   {
      case 'O' :
        animacionesOn = false;
        cout << "práctica 3: animaciones desactivadas" << endl;
        objetoActivo3 = (objetoActivo3 + 1) % numObjetos3;
        cout << "práctica 3: nuevo objeto activo es: " << objetoActivo3;
        if (objetos3[objetoActivo3] != nullptr)
           cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl;
        else
           cout << " (objeto no creado)" << endl;
        gradoActual = 0;
        cout << "práctica 3: nº grado de libertad actual: " << 0 << endl;
        res = true;
        break ;

      case 'A' :
         animacionesOn = !animacionesOn;
         if (animacionesOn)
          FijarFuncDesocupado( FGE_Desocupado );
         cout << "práctica 3: las animaciones están: " << animacionesOn << endl;
         res = true;
         break ;

      case 'G' :
         gradoActual = (gradoActual + 1) % objetos3[objetoActivo3]->numParametros();
         cout << "práctica 3: nº grado de libertad actual: " << gradoActual << endl;
         cout << "práctica 3: el grado de libertad es: "
          << objetos3[objetoActivo3]->leerPtrParametro(gradoActual)->leer_descripcion() << endl;
         res = true;
         break ;

      case 'R' :
         animacionesOn = false;
         cout << "práctica 3: animaciones desactivadas." << endl;
         for (int i = 0; i < objetos3[objetoActivo3]->numParametros(); ++i)
          objetos3[objetoActivo3]->leerPtrParametro(i)->reset();
         cout << "práctica 3: parámetros reseteados." << endl;
         res = true;
         break ;

      case '>' :
         p = objetos3[objetoActivo3]->leerPtrParametro(gradoActual);
         if (animacionesOn) {
           p->acelerar();
           faux = p->leer_velocidad_actual();
         } else {
           p->incrementar();
           faux = p->leer_valor_actual();
         }
         cout << "práctica 3: el grado de libertad " << p->leer_descripcion()
              << ", vale ahora: " << faux << endl;
         res = true;
         break ;

      case '<' :
        p = objetos3[objetoActivo3]->leerPtrParametro(gradoActual);
        if (animacionesOn) {
          p->decelerar();
          faux = p->leer_velocidad_actual();
        }
        else {
          p->decrementar();
          faux = p->leer_valor_actual();
        }
        cout << "práctica 3: el grado de libertad " << p->leer_descripcion()
             << ", vale ahora: " << faux << endl;
        res = true;

        break ;
      default :
        break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
  objetos3[objetoActivo3]->visualizarGL(cv);
}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
   if (!animacionesOn)
      return false;

    objetos3[objetoActivo3]->siguienteCuadro();
    redibujar_ventana = true;
    return true;
}

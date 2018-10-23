// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica2.hpp"
#include "Objeto3D.hpp"
#include "MallaPLY.hpp"
#include "MallaRevol.hpp"

using namespace std ;

static unsigned objetoActivo2 = 0 ; // objeto activo: malla ply (0), malla revol (1)
static constexpr int numObjetos2 = 5 ;

static Objeto3D * objetos2[numObjetos2] = { nullptr, nullptr, nullptr, nullptr, nullptr };

static MallaPLY * mp = NULL;
static MallaRevol * mr = NULL;
static Cilindro * cilindro = NULL;
static Esfera * esfera = NULL;
static Cono * cono = NULL;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P2_Inicializar(int argc, char * argv[])
{
   cout << "Creando objetos de la práctica 2 .... " << flush ;
   // COMPLETAR: práctica 2: inicializar objeto de revolución y objeto PLY (en 'objeto2')

   if (argc == 3) {
     mp = new MallaPLY(argv[1]);
     //mr = new MallaRevol(argv[2]);
   } else if (argc == 2)
    mp = new MallaPLY(argv[1]);
   else {
    mp = new MallaPLY("../plys/ant.ply");
    mr = new MallaRevol("../plys/peon.ply", 100, true, true);
   }

   objetos2[0] = mp;
   objetos2[1] = mr;
   objetos2[2] = new Cilindro(1.0, 1.0, 100, 100, true, true);
   objetos2[3] = new Esfera(1.0, 100, 100, true, true);
   objetos2[4] = new Cono(1.0, 1.0, 100, 100, true, true);

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el cubo, el tetraedro u otros objetos (cambia el valor de
//    'objeto_activo').
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P2_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   if ( toupper(tecla) != 'O')
      return false ;

   objetoActivo2 = (objetoActivo2+1) % numObjetos2 ;
   cout << "práctica 1: nuevo objeto activo es: " << objetoActivo2 ;
   if ( objetos2[objetoActivo2] != nullptr )
      cout << " (" << objetos2[objetoActivo2]->leerNombre() << ")." << endl ;
   else
      cout << " (objeto no creado)" << endl ;
   return true ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P2_DibujarObjetos( ContextoVis & cv )
{
   // COMPLETAR: práctica 2: invocar a 'visualizarGL' para el objeto activo actual en 'objetos2'
   objetos2[objetoActivo2]->visualizarGL(cv);

}

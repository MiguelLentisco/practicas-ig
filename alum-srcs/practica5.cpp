// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"

#include "CamaraInter.hpp"

using namespace std ;

// COMPLETAR: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar)
// ......

static unsigned camaraActiva = 0 ;
static constexpr int numCamaras = 1;
static CamaraInteractiva * camaras[numCamaras] = { nullptr };
static CamaraInteractiva * camara = NULL;

// viewport actual (se recalcula al inicializar y al fijar las matrices)
Viewport viewport ;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;


// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
   cout << "Creando objetos de la práctica 5 .... " << flush ;
   // COMPLETAR: práctica 5: inicializar las variables de la práctica 5 (incluyendo el viewport)
   // .......
   //P5_FijarMVPOpenGL(vp_ancho, vp_alto);
   //camaras[0] = camara = new CamaraInteractiva();

   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
   // COMPLETAR: práctica 5: actualizar viewport, actualizar y activar la camara actual
   //     (en base a las dimensiones del viewport)
   // .......
   viewport.ancho = vp_ancho;
   viewport.alto = vp_alto;
   viewport.ratio_yx = float(viewport.alto) / float(viewport.ancho) ;
   viewport.matrizVp    = MAT_Viewport( viewport.org_x, viewport.org_y, viewport.ancho, viewport.alto );
   viewport.matrizVpInv = MAT_Viewport_inv( viewport.org_x, viewport.org_y, viewport.ancho, viewport.alto );

   camaras[camaraActiva]->ratio_yx_vp = 1.0 * vp_alto / vp_ancho;
   camaras[camaraActiva]->calcularViewfrustum();
   camaras[camaraActiva]->activar();
}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{

   // COMPLETAR: práctica 5: activar las fuentes de luz y visualizar la escena
   //      (se supone que la camara actual ya está activada)
   // .......

}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{

   bool result = true ;

   switch ( toupper( tecla ) )
   {
      case 'C':
         // COMPLETAR: práctica 5: activar siguiente camara
         // .....

         break ;

      case 'V':
         // COMPLETAR: práctica 5: conmutar la cámara actual entre modo examinar y el modo primera persona
         // .....

         break ;

      case '-':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (positivo) (desplaZ)
         // .....

         break;

      case '+':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (negativo) (desplaZ)
         // .....

         break;

      default:
         result = false ;
         break ;
	}
   return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{

   bool result = true ;


   switch ( tecla )
   {
      case GLFW_KEY_LEFT:
         // COMPLETAR: práctica 5: desplazamiento/rotacion hacia la izquierda (moverHV)
         // .....

         break;
      case GLFW_KEY_RIGHT:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia la derecha (moverHV)
         // .....

         break;
      case GLFW_KEY_UP:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia arriba (moverHV)
         // .....

         break;
      case GLFW_KEY_DOWN:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia abajo (moverHV)
         // .....

         break;
      default:
         result = false ;
         break ;
	}

   return result ;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y )
{

   // COMPLETAR: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)


   // 1. crear un 'contextovis' apropiado
   // .....

   // 2. visualizar en modo selección (sobre el backbuffer)
   // ....

   // 3. leer el color del pixel, si es 0 no se hace nada
   // .....

   // 4. buscar el objeto en el grafo de escena e informar del mismo
   // .....

}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
   // COMPLETAR: práctica 5: activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
   // .....

}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   // COMPLETAR: práctica 5: desactivar bool del modo arrastrar
   // .....

}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
   // COMPLETAR: práctica 5: calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)
   // .....


}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }
   else
      return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction )
{
   // COMPLETAR: práctica 5: acercar/alejar la camara (desplaZ)
   // .....

   return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   // .....

}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....

}
//---------------

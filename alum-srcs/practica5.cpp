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
#include "grafo-escena.hpp"

using namespace std ;

// COMPLETAR: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar)
// ......

static unsigned camaraActiva = 0 ;
static unsigned objetoActivo = 0;
static constexpr int numCamaras = 2;
static constexpr int numObjetos = 1;
static CamaraInteractiva * camaras[numCamaras] = { nullptr, nullptr };
static NodoGrafoEscenaParam * objetos[numObjetos] = { nullptr };
static CamaraInteractiva * camara = NULL;
static EscenaP4 * escenaLataPeones = NULL;
//static Snowman * snowman = NULL;
static ColeccionFuentesP4 * fuentes = NULL;
static CamaraInteractiva * camaraOrtogonal = NULL;

// viewport actual (se recalcula al inicializar y al fijar las matrices)
Viewport viewport ;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;

static int xant, yant;


// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
   cout << "Creando objetos de la práctica 5 .... " << flush ;
   // COMPLETAR: práctica 5: inicializar las variables de la práctica 5 (incluyendo el viewport)
   // .......
   camaras[0] = camara = new CamaraInteractiva(false, vp_alto * 1.0 / vp_ancho,
                            0, 0, Tupla3f(0.0, 0.0, 0.0), true);
   camaras[1] = camaraOrtogonal = new CamaraInteractiva(false, vp_alto * 1.0 / vp_ancho,
                                    0, 0, Tupla3f(0.0, 0.0, 0.0), false);
   objetos[0] = escenaLataPeones = new EscenaP4();
   //objetos[1] = snowman = new Snowman();
   fuentes = new ColeccionFuentesP4();
   P5_FijarMVPOpenGL(vp_ancho, vp_alto);

   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
   // COMPLETAR: práctica 5: actualizar viewport, actualizar y activar la camara actual
   //     (en base a las dimensiones del viewport)
   // .......
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   glViewport( viewport.org_x, viewport.org_y, viewport.ancho, viewport.alto );

   camaras[camaraActiva]->ratio_yx_vp =  viewport.ratio_yx;
   camaras[camaraActiva]->calcularViewfrustum();
}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{

   // COMPLETAR: práctica 5: activar las fuentes de luz y visualizar la escena
   //      (se supone que la camara actual ya está activada)
   // .......
   fuentes->activar(5);
   objetos[objetoActivo]->visualizarGL(cv);

}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{

   bool result = true ;
   const float valor = 2.0;

   switch ( toupper( tecla ) )
   {
      case 'C':
         // COMPLETAR: práctica 5: activar siguiente camara
         // .....
         camaraActiva = (camaraActiva + 1) % numCamaras;
         cout << "P5: Cámara activa nº " << camaraActiva << " , es "
              << (camaras[camaraActiva]->perspectiva ? "perspectiva" : "ortogonal") << endl;
         camaras[camaraActiva]->ratio_yx_vp = 1.0 * viewport.alto / viewport.ancho;
         camaras[camaraActiva]->calcularViewfrustum();

         break ;

      case 'V':
         // COMPLETAR: práctica 5: conmutar la cámara actual entre modo examinar y el modo primera persona
         // .....
         if (camaras[camaraActiva]->examinar)
          camaras[camaraActiva]->modoPrimeraPersona();
         else
          camaras[camaraActiva]->modoExaminar();
         cout << "P5: Modo de cámara cambiado a "
              << (camaras[camaraActiva]->examinar ? "examinar. " :
                  "primera persona") << endl;
         break ;

      case '-':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (positivo) (desplaZ)
         // .....
         camaras[camaraActiva]->desplaZ( valor );

         break;

      case '+':
         // COMPLETAR: práctica 5: desplazamiento en Z de la cámara actual (negativo) (desplaZ)
         // .....
         camaras[camaraActiva]->desplaZ( -valor );

         break;

      case 'O' :
         objetoActivo = (objetoActivo + 1) % numObjetos;
         cout << "P5: nuevo objeto activo es: " << objetoActivo;
         if (objetos[objetoActivo] != nullptr)
           cout << " (" << objetos[objetoActivo]->leerNombre() << ")." << endl;
         else
           cout << " (objeto no creado)" << endl;
         result = true;
         break ;

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
   const float valor = 2.0;


   switch ( tecla )
   {
      case GLFW_KEY_LEFT:
         // COMPLETAR: práctica 5: desplazamiento/rotacion hacia la izquierda (moverHV)
         // .....
         camaras[camaraActiva]->moverHV( -valor, 0 );

         break;
      case GLFW_KEY_RIGHT:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia la derecha (moverHV)
         // .....
         camaras[camaraActiva]->moverHV( valor, 0 );
         break;
      case GLFW_KEY_UP:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia arriba (moverHV)
         // .....
         camaras[camaraActiva]->moverHV( 0, valor );

         break;
      case GLFW_KEY_DOWN:
         // COMPLETAR: práctica 5: desplazamiento/rotación hacia abajo (moverHV)
         // .....
         camaras[camaraActiva]->moverHV( 0, -valor );
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
   glDisable( GL_LIGHTING );
   glDisable( GL_TEXTURE_2D );
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glColor3ub(0.0, 0.0, 0.0);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // 1. crear un 'contextovis' apropiado
   // .....
   ContextoVis cv;
   cv.modoSeleccionFBO = true;

   // 2. visualizar en modo selección (sobre el backbuffer)
   objetos[objetoActivo]->visualizarGL(cv);

   // ....

   // 3. leer el color del pixel, si es 0 no se hace nada
   // .....
   int ident = LeerIdentEnPixel(x, y);
   // 4. buscar el objeto en el grafo de escena e informar del mismo
   // .....
   if (ident != 0) {
     Objeto3D* bus_objeto = NULL;
     Tupla3f centro_wc(0.0, 0.0, 0.0);
     if (objetos[objetoActivo]->buscarObjeto(ident, MAT_Ident(), &bus_objeto, centro_wc)) {
       camaras[camaraActiva]->modoExaminar(centro_wc);
       cout << "P5: Se ha seleccionado el objeto: " << bus_objeto->leerNombre()
            << " con centro " << centro_wc << endl;
     } else
       cout << "P5: Error al seleccionar, no se ha encontrado" << endl;
   } else
    cout << "P5: No se ha seleccionado nada" << endl;

}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
   // COMPLETAR: práctica 5: activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
   modo_arrastrar = true;
   xant = x;
   yant = y;

}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   // COMPLETAR: práctica 5: desactivar bool del modo arrastrar
   // .....
   modo_arrastrar = false;

}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
   // COMPLETAR: práctica 5: calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)
   // .....
   camaras[camaraActiva]->moverHV(x - xant, y - yant);
   xant = x;
   yant = y;

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
   camaras[camaraActiva]->desplaZ(direction * 2.0);

   return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   // .....
   const unsigned char byteR = ident % 0x100U,
                       byteG = (ident / 0x100U) % 0x100U,
                       byteB = (ident / 0x10000U) % 0x100U;

   glColor3ub( byteR, byteG, byteB );

}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....

   unsigned char bytes[3];
   glReadPixels( xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *) bytes);
   return bytes[0] + (0x100U * bytes[1]) + (0x10000U * bytes[2]);

}
//---------------

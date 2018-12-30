// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

const bool trazam = false ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual )
   {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  )
{
   if ( pila.size() == 0 )
   {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila[pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
}

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
  imagen = NULL;
  imagen = new jpg::Imagen(nombreArchivoJPG);
  enviada = false;
  modo_gen_ct = mgct_desactivada;
  glGenTextures( 1, &ident_textura );
}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar() {
   unsigned int ancho = imagen->tamX();
   unsigned int alto = imagen->tamY();
   unsigned char * texels = imagen->leerPixels();
   gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, ancho, alto, GL_RGB,
     GL_UNSIGNED_BYTE, texels);
}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  )
{
  glBindTexture( GL_TEXTURE_2D, ident_textura );
  if (!enviada)
    enviar();
}
// *********************************************************************

Material::Material()
{
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}
// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG )
{
   iluminacion    = false ;
   tex            = new Textura( nombreArchivoJPG ) ;

   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
:  Material()
{
   // COMPLETAR: práctica 4: inicializar material usando text,ka,kd,ks,exp
   // .....

   ponerNombre("material con textura e iluminación") ;

 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp )
{
   // COMPLETAR: práctica 4: inicializar material usando colorAmbDif,ka,kd,ks,exp
   // .....

   ponerNombre("material color plano, ilum.") ;
}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
   // COMPLETAR: práctica 4: inicializar material usando un color plano sin iluminación
   // .....

}

//----------------------------------------------------------------------

void Material::coloresCero()
{
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar(  )
{
   // COMPLETAR: práctica 4: activar un material
   // .....

}

//**********************************************************************

// Para fuente de luz direccional
FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   esDireccional = true;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi = longi_ini ;
   lati  = lati_ini ;

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}

//----------------------------------------------------------------------

FuenteLuz::FuenteLuz( const Tupla3f& posicion, const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   esDireccional = false;

   // inicializar parámetros de la fuente de luz
   posvec = posicion;

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}

//----------------------------------------------------------------------

void FuenteLuz::activar()
{
   // Activamos la fuente
   glEnable (GL_LIGHT0 + ind_fuente);
   // Configuramos los colores
   glLightfv (GL_LIGHT0 + ind_fuente, GL_AMBIENT, col_ambiente);
   glLightfv (GL_LIGHT0 + ind_fuente, GL_DIFFUSE, col_difuso);
   glLightfv (GL_LIGHT0 + ind_fuente, GL_SPECULAR, col_especular);
   // Configuramos la posición o dirección segun toque
   if (esDireccional) {
     const GLfloat ejeZ[4] = {0.0, 0.0, 1.0, 0.0};
     glMatrixMode( GL_MODELVIEW );
     glPushMatrix();
     glLoadIdentity();
     glRotatef( longi, 0.0, 1.0, 0.0 );
     glRotatef( lati, -1.0, 0.0, 0.0);
     glLightfv( GL_LIGHT0 + ind_fuente, GL_POSITION, ejeZ );
     glPopMatrix();
   } else {
     const GLfloat posf[4] = {posvec(0), posvec(1), posvec(2), 1.0 };
     glLightfv( GL_LIGHT0 + ind_fuente, GL_POSITION, posf);
   }
}

//----------------------------------------------------------------------

bool FuenteLuz::gestionarEventoTeclaEspecial( int key )
{
   bool actualizar = true ;
   const float incr = 3.0f ;

   switch( key )
   {
      case GLFW_KEY_RIGHT:
         longi = longi+incr ;
         break ;
      case GLFW_KEY_LEFT:
         longi = longi-incr ;
         break ;
      case GLFW_KEY_UP:
         lati = std::min( lati+incr, 90.0f) ;
         break ;
      case GLFW_KEY_DOWN:
         lati = std::max( lati-incr, -90.0f ) ;
         break ;
      case GLFW_KEY_HOME:
         lati  = lati_ini ;
         longi = longi_ini ;
         break ;
      default :
         actualizar = false ;
         cout << "tecla no usable para la fuente de luz." << endl << flush ;
   }

   //if ( actualizar )
   //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
   return actualizar ;
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = 8 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   if (vpf.size() < max_num_fuentes) {
     pf->ind_fuente = vpf.size() ;
     vpf.push_back( pf ) ;
   }
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( unsigned id_prog )
{
   glEnable(GL_LIGHTING);

   for (unsigned int i = 0; i < vpf.size(); ++i)
    vpf[i]->activar();
   for (unsigned int i = vpf.size(); i < max_num_fuentes; ++i)
    glDisable(GL_LIGHT0 + i);

}
//----------------------------------------------------------------------
FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i )
{
   assert(i < vpf.size()) ;
   return vpf[i] ;
}
//----------------------------------------------------------------------
ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}

//**********************************************************************

FuenteDireccional::FuenteDireccional( float alpha_inicial, float beta_inicial )
: FuenteLuz(alpha_inicial, beta_inicial, Tupla4f(0.0, 0.5, 0.0, 1.0)) {}

//----------------------------------------------------------------------

void FuenteDireccional::variarAngulo(unsigned angulo, float incremento)
{
  switch (angulo) {
    case 0:
      longi = longi + incremento;
      break;
    case 1:
      lati = std::min(lati + incremento, 90.0f);
      break;
  }
}
//**********************************************************************

FuentePosicional::FuentePosicional( const Tupla3f& posicion )
  : FuenteLuz(posicion, Tupla4f(0.0, 0.4, 0.0, 1.0)) {}

//**********************************************************************

ColeccionFuentesP4::ColeccionFuentesP4() {

}

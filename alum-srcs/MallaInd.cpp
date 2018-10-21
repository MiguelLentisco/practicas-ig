// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

#define USE_GL_DRAWARRAYS

// *****************************************************************************
// funciones auxiliares


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");
   sinVBO = true;

}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
   sinVBO = true;

}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......


}

// -----------------------------------------------------------------------------

void MallaInd::visualizarBE_MI( ContextoVis & cv) {
  glBegin( GL_TRIANGLES );
  for (unsigned int i = 0; i < n_triangulos; ++i)
    for (unsigned int j = 0; j < 3; ++j) {
      unsigned int ind_ver = tablaTriangulos[i](j);
      glVertex3fv(tablaVertices[ind_ver]);
    }
  glEnd();
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
   // COMPLETAR: en la práctica 1: visualizar en modo inmediato (glDrawElements)

  // Cambiar el color
  //glColor3f(1, 0, 0);

  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, &(tablaVertices[0]) );
  glDrawElements( GL_TRIANGLES, 3 * n_triangulos, GL_UNSIGNED_INT, tablaTriangulos.data() );
  glDisableClientState( GL_VERTEX_ARRAY );
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
  // COMPLETAR: práctica 1: visualizar en modo diferido,
  //                        usando VBOs (Vertex Buffer Objects)

  if (sinVBO)
    crearVBOs();


  glBindBuffer( GL_ARRAY_BUFFER, id_vbo_ver );
  glVertexPointer( 3, GL_FLOAT, 0, 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0);
  glEnableClientState( GL_VERTEX_ARRAY );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri );
  glDrawElements( GL_TRIANGLES, 3L* n_triangulos, GL_UNSIGNED_INT, NULL);
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState( GL_VERTEX_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: visualizar en modo inmediato o en modo diferido (VBOs),
   // (tener en cuenta el modo de visualización en 'cv' (alambre, sólido, etc..))
   //

   // Cambiar modo de visualización
   GLenum modoVisualizacion;

   switch (cv.modoVis) {
     case modoPuntos:
      modoVisualizacion = GL_POINT;
      break;
     case modoAlambre:
      modoVisualizacion = GL_LINE;
      break;
     case modoSolido:
      modoVisualizacion = GL_FILL;
      break;
     default:
      modoVisualizacion = GL_FILL;
   }

  // Cambiar modo de visualización
  glPolygonMode( GL_FRONT_AND_BACK, modoVisualizacion);

   if (cv.usarVBOs)
    visualizarDE_VBOs(cv);
   else
    #ifdef USE_GL_DRAWARRAYS
      visualizarDE_MI(cv);
    #elif
      visualizarBE_MI(cv);
    #endif

}
// *****************************************************************************

// *****************************************************************************

// Constructor Cubo
// *****************************************************************************

Cubo::Cubo() : MallaInd( "malla cubo" )
{
  n_vertices = 8;
  Tupla3f puntos[n_vertices] = { Tupla3f(0.0, 0.0, 0.0), Tupla3f(1.0, 0.0, 0.0),
    Tupla3f(0.0, 1.0, 0.0), Tupla3f(1.0, 1.0, 0.0), Tupla3f(0.0, 0.0, 1.0),
    Tupla3f(1.0, 0.0, 1.0), Tupla3f(0.0, 1.0, 1.0), Tupla3f(1.0, 1.0, 1.0) };
  Tupla3f vectorTraslacion(-0.5, -0.5, -0.5);
  for (unsigned int i = 0; i < n_vertices; ++i)
    tablaVertices.push_back(puntos[i] + vectorTraslacion);
  n_triangulos = 12;
  Tupla3i triangulos[n_triangulos] = { Tupla3i(2, 0 ,1), Tupla3i(2, 1, 3),
    Tupla3i(6, 2, 3), Tupla3i(6, 3, 7), Tupla3i(6, 4, 5), Tupla3i(6, 5, 7),
    Tupla3i(2, 0, 4), Tupla3i(2, 4, 6), Tupla3i(3, 1, 5), Tupla3i(3, 5, 7),
    Tupla3i(4, 0, 1), Tupla3i(4, 1, 5) };
  for (unsigned int i = 0; i < n_triangulos; ++i)
    tablaTriangulos.push_back(triangulos[i]);
  //sinVBO = true;
}

// Constructor Tetraedro
// *****************************************************************************

Tetraedro::Tetraedro() : MallaInd( "malla tetraedro")
{
  n_vertices = 4;
  Tupla3f puntos[n_vertices] = { Tupla3f(-0.5, -0.204, -0.289),
    Tupla3f(0.5, -0.204, -0.289), Tupla3f(0.0, -0.204, 0.58),
    Tupla3f(0.0, 0.6124, 0.0) };
  for (unsigned int i = 0; i < n_vertices; ++i)
    tablaVertices.push_back(puntos[i]);
  n_triangulos = 4;
  Tupla3i triangulos[n_triangulos] = { Tupla3i(2, 0, 1), Tupla3i(3, 0, 1),
    Tupla3i(3, 1, 2), Tupla3i(3, 0, 2) };
  for (unsigned int i = 0; i < n_triangulos; ++i)
    tablaTriangulos.push_back(triangulos[i]);
//  sinVBO = true;
}

// **************************************************************************

GLuint VBO_Crear(GLuint tipo, GLuint tamanio, GLvoid * puntero) {
  assert( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER );
  GLuint id_vbo;
  glGenBuffers( 1, & id_vbo );
  glBindBuffer( tipo, id_vbo );
  glBufferData( tipo, tamanio, puntero, GL_STATIC_DRAW );
  glBindBuffer( tipo, 0 );
  return id_vbo;
}

// **************************************************************************

void MallaInd::crearVBOs() {
  id_vbo_ver = VBO_Crear( GL_ARRAY_BUFFER, sizeof(float) * 3L * n_vertices, tablaVertices.data() );
  id_vbo_tri = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * 3L * n_triangulos, tablaTriangulos.data() );
  sinVBO = false;
}

// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <stdlib.h>  // rand
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
   n_vertices = n_triangulos = 0;
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
   sinVBO = true;
   n_vertices = n_triangulos = 0;
}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
  // Primero calculamos las normales de las caras
  for (unsigned int i = 0; i < n_triangulos; ++i) {
    Tupla3f p, q, r, a, b, v;
    // Los vertices de la cara
    p = tablaVertices[tablaTriangulos[i](0)];
    q = tablaVertices[tablaTriangulos[i](1)];
    r = tablaVertices[tablaTriangulos[i](2)];
    // Tomamos dos lados de la cara
    a = q - p;
    b = r - p;
    // El producto vectorial de las caras es el perpendicular
    v = a.cross(b);
    // Normalizamos
    v = v.normalized();
    normalesCaras.push_back(v);
  }
  // Y ahora calculamos las normales de los vertices
  // Aqui vamos guardando la suma de cada vertice
  vector<Tupla3f> sumaVectores;
  for (unsigned int i = 0; i < n_vertices; ++i)
    sumaVectores.push_back(Tupla3f(0.0, 0.0, 0.0));
  // Por cada cara añadimos su normal a la de los vertices de la cara
  for (unsigned int i = 0; i < n_triangulos; ++i) {
    Tupla3f norCara = normalesCaras[i];
    sumaVectores[tablaTriangulos[i](0)] += norCara;
    sumaVectores[tablaTriangulos[i](1)] += norCara;
    sumaVectores[tablaTriangulos[i](2)] += norCara;
  // Normalizamos los vectores y añadimos a la la lista de normales
  for (unsigned int i = 0; i < n_vertices; ++i)
    normalesVertices[i] = sumaVectores[i].normalized();
  }

}
// -----------------------------------------------------------------------------

void MallaInd::visualizarBE_MI( ContextoVis & cv ) {
  glBegin( GL_TRIANGLES );
  for (unsigned int i = 0; i < n_triangulos; ++i)
    for (unsigned int j = 0; j < 3; ++j) {
      unsigned int ind_ver = tablaTriangulos[i](j);
      if (colVertices.size() > 0)
        glColor3fv(colVertices[ind_ver]);
      glVertex3fv(tablaVertices[ind_ver]);
    }
  glEnd();
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
  // Cambiar el color
  //glColor3f(1, 0, 0);

  if (colVertices.size() > 0 ) {
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 3, GL_FLOAT, 0, colVertices.data() );
  }

  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, &(tablaVertices[0]) );
  glDrawElements( GL_TRIANGLES, 3 * n_triangulos, GL_UNSIGNED_INT, tablaTriangulos.data() );
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
  if (sinVBO)
    crearVBOs();

  if (colVertices.size() > 0 ) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_col );
    glColorPointer( 3, GL_FLOAT, 0, 0 );
    glEnableClientState( GL_COLOR_ARRAY );
  }

  glBindBuffer( GL_ARRAY_BUFFER, id_vbo_ver );
  glVertexPointer( 3, GL_FLOAT, 0, 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0);
  glEnableClientState( GL_VERTEX_ARRAY );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri );
  glDrawElements( GL_TRIANGLES, 3L* n_triangulos, GL_UNSIGNED_INT, NULL);
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
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

// **************************************************************************

void MallaInd::fijarColorNodo(const Tupla3f & color) {
  colVertices.clear();
  for (unsigned i = 0; i < n_vertices; ++i)
    colVertices.push_back(color);
}

void MallaInd::fijarColorNodo() {
  float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  Tupla3f color = Tupla3f(r, g, b);
  for (unsigned i = 0; i < n_vertices; ++i)
    colVertices.push_back(color);
}

void MallaInd::colorearEntero(const std::vector<Tupla3f>& colores) {
  colVertices.clear();
  for (unsigned i = 0; i < n_vertices; ++i)
    colVertices.push_back(colores[i]);
}


// *****************************************************************************

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
  if (colVertices.size() > 0)
    id_vbo_col = VBO_Crear( GL_ARRAY_BUFFER, sizeof(float) * 3L * n_vertices, colVertices.data() );
  id_vbo_ver = VBO_Crear( GL_ARRAY_BUFFER, sizeof(float) * 3L * n_vertices, tablaVertices.data() );
  id_vbo_tri = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * 3L * n_triangulos, tablaTriangulos.data() );
  sinVBO = false;
}


// Constructor Cubo
// *****************************************************************************

Cubo::Cubo(float lado, Tupla3f centro) : MallaInd( "malla cubo" )
{
  this->lado = lado;
  this->centro = centro;
  n_vertices = 8;
  Tupla3f puntos[n_vertices] = { Tupla3f(0.0, 0.0, 0.0), Tupla3f(lado, 0.0, 0.0),
    Tupla3f(0.0, lado, 0.0), Tupla3f(lado, lado, 0.0), Tupla3f(0.0, 0.0, lado),
    Tupla3f(lado, 0.0, lado), Tupla3f(0.0, lado, lado), Tupla3f(lado, lado, lado) };
  Tupla3f vectorTraslacion(- lado / 2.0, - lado / 2.0, - lado / 2.0);
  for (unsigned int i = 0; i < n_vertices; ++i)
    tablaVertices.push_back(puntos[i] + vectorTraslacion + centro);
  n_triangulos = 12;
  Tupla3i triangulos[n_triangulos] = { Tupla3i(2, 0 ,1), Tupla3i(2, 1, 3),
    Tupla3i(6, 2, 3), Tupla3i(6, 3, 7), Tupla3i(6, 4, 5), Tupla3i(6, 5, 7),
    Tupla3i(2, 0, 4), Tupla3i(2, 4, 6), Tupla3i(3, 1, 5), Tupla3i(3, 5, 7),
    Tupla3i(4, 0, 1), Tupla3i(4, 1, 5) };
  for (unsigned int i = 0; i < n_triangulos; ++i)
    tablaTriangulos.push_back(triangulos[i]);
   fijarColorNodo();
}

Cubo::Cubo(Tupla3f centro) : Cubo(1.0, centro) {
}

Cubo::Cubo(float lado) : Cubo(lado, Tupla3f(0.0, 0.0, 0.0)) {
}

Cubo::Cubo() : Cubo(1.0, Tupla3f(0.0, 0.0, 0.0)) {
}

// Constructor Tetraedro
// *****************************************************************************

Tetraedro::Tetraedro(float lado, Tupla3f centro) : MallaInd( "malla tetraedro" )
{
  this->lado = lado;
  this->centro = centro;
  n_vertices = 4;
  Tupla3f puntos[n_vertices] = { Tupla3f(0, 0, 0),
    Tupla3f(lado, 0, 0), Tupla3f(lado / 2.0, 0, sqrt(3) * lado / 2.0),
    Tupla3f(lado / 2.0, sqrt(6) * lado / 3.0, sqrt(3) * lado / 6.0) };
  Tupla3f vectorTraslacion(-lado / 2.0, -sqrt(6) * lado / 12.0, -sqrt(3) * lado / 6.0);
  for (unsigned int i = 0; i < n_vertices; ++i)
    tablaVertices.push_back(puntos[i] + vectorTraslacion + centro);
  n_triangulos = 4;
  Tupla3i triangulos[n_triangulos] = { Tupla3i(2, 0, 1), Tupla3i(3, 0, 1),
    Tupla3i(3, 1, 2), Tupla3i(3, 0, 2) };
  for (unsigned int i = 0; i < n_triangulos; ++i)
    tablaTriangulos.push_back(triangulos[i]);
  fijarColorNodo();
}

Tetraedro::Tetraedro(Tupla3f centro) : Tetraedro(1.0, centro) {
}

Tetraedro::Tetraedro(float lado) : Tetraedro(lado, Tupla3f(0.0, 0.0, 0.0)) {
}

Tetraedro::Tetraedro() : Tetraedro(1.0, Tupla3f(0.0, 0.0, 0.0)) {
}

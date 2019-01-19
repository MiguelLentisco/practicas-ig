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

// *****************************************************************************
// funciones auxiliares

// -----------------------------------------------------------------------------
Tupla3f centroCajaEnglobante(std::vector<Tupla3f> vertices) {
  Tupla3f max = vertices[0], min = vertices[0];
  for (unsigned int i = 0; i < vertices.size(); ++i) {
    for (unsigned j = 0; i < 3; ++i) {
      if (vertices[i](j) < min(j))
        min(j) = vertices[i](j);
      if (vertices[i](j) > max(j))
        max(j) = vertices[i](j);
    }
  }
  return (max + min) / 2.0;
}


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");
   sinVBO = true;
   n_vertices = n_triangulos = 0;
   centroCalculado = false;
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
   sinVBO = true;
   centroCalculado = false;
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
     if (v.lengthSq() != 0)
       v = v.normalized();
     normalesCaras.push_back(v);
   }
   // Y ahora calculamos las normales de los vertices
   // Vamos guardando la suma de cada vertice
   for (unsigned int i = 0; i < n_vertices; ++i)
     normalesVertices.push_back(Tupla3f(0.0, 0.0, 0.0));
   // Por cada cara añadimos su normal a la de los vertices de la cara
   for (unsigned int i = 0; i < n_triangulos; ++i) {
     Tupla3f norCara = normalesCaras[i];
     normalesVertices[tablaTriangulos[i](0)] =
       normalesVertices[tablaTriangulos[i](0)] + norCara;
     normalesVertices[tablaTriangulos[i](1)] =
       normalesVertices[tablaTriangulos[i](1)] + norCara;
     normalesVertices[tablaTriangulos[i](2)] =
       normalesVertices[tablaTriangulos[i](2)] + norCara;
   // Normalizamos los vectores
   for (unsigned int i = 0; i < n_vertices; ++i)
     if (normalesVertices[i].lengthSq() != 0)
       normalesVertices[i] = normalesVertices[i].normalized();
  }

}
// -----------------------------------------------------------------------------

void MallaInd::visualizarBE_MI( ContextoVis & cv) {
  glBegin( GL_TRIANGLES );
  for (unsigned int i = 0; i < n_triangulos; ++i)
    for (unsigned int j = 0; j < 3; ++j) {
      unsigned int ind_ver = tablaTriangulos[i](j);
      if (colVertices.size() > 0 && !cv.modoSeleccionFBO )
        glColor3fv( colVertices[ind_ver] );
      if (normalesVertices.size() > 0)
        glNormal3fv( normalesVertices[ind_ver] );
      if (coordTextura.size() > 0)
        glTexCoord2fv( coordTextura[ind_ver] );
      glVertex3fv( tablaVertices[ind_ver] );
    }
  glEnd();
}

// -----------------------------------------------------------------------------


void MallaInd::visualizarBE_TRI( ContextoVis & cv) {
  glBegin( GL_TRIANGLES );
  for (unsigned int i = 0; i < n_triangulos; ++i) {
    if (normalesCaras.size() > 0)
      glNormal3fv( normalesCaras[i] );
    for (unsigned int j = 0; j < 3; ++j) {
      unsigned ind_ver = tablaTriangulos[i](j);
      if (colVertices.size() > 0)
        glColor3fv( colVertices[ind_ver] );
      if (coordTextura.size() > 0)
        glTexCoord2fv( coordTextura[ind_ver] );
      glVertex3fv( tablaVertices[ind_ver] );
    }
  }
  glEnd();
}


// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
   // COMPLETAR: en la práctica 1: visualizar en modo inmediato (glDrawElements)

  // Cambiar el color
  //glColor3f(1, 0, 0);

  if (colVertices.size() > 0 && !cv.modoSeleccionFBO) {
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 3, GL_FLOAT, 0, colVertices.data() );
  }

  if (normalesVertices.size() > 0) {
    glEnableClientState( GL_NORMAL_ARRAY );
    glNormalPointer( GL_FLOAT, 0, normalesVertices.data() );
  }

  if (coordTextura.size() > 0) {
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glTexCoordPointer( 2, GL_FLOAT, 0, coordTextura.data() );
  }

  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, tablaVertices.data() );
  glDrawElements( GL_TRIANGLES, 3 * n_triangulos, GL_UNSIGNED_INT, tablaTriangulos.data() );

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
  if (sinVBO)
    crearVBOs();

  if (colVertices.size() > 0 && !cv.modoSeleccionFBO) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_col );
    glColorPointer( 3, GL_FLOAT, 0, 0 );
    glEnableClientState( GL_COLOR_ARRAY );
  }

  if (normalesVertices.size() > 0) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_normal_ver );
    glNormalPointer( GL_FLOAT, 0, 0 );
    glEnableClientState( GL_NORMAL_ARRAY );
  }

  if (coordTextura.size() > 0) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_cctt );
    glTexCoordPointer( 2, GL_FLOAT, 0, 0);
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
  }

  glBindBuffer( GL_ARRAY_BUFFER, id_vbo_ver );
  glVertexPointer( 3, GL_FLOAT, 0, 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0);
  glEnableClientState( GL_VERTEX_ARRAY );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri );
  glDrawElements( GL_TRIANGLES, 3L* n_triangulos, GL_UNSIGNED_INT, NULL);
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   // Cambiar modo de visualización
   GLenum modoVisualizacion;
   GLenum modoSombra;
   int tipoVisualizacion;

   glEnable( GL_RESCALE_NORMAL );

   if (cv.modoSeleccionFBO) {
     modoVisualizacion = GL_FILL;
     modoSombra = GL_FLAT;
     tipoVisualizacion = 0;
     glDisable( GL_LIGHTING );
     glDisable( GL_TEXTURE_2D );
     if (leerIdentificador() >= 0)
      FijarColorIdent(leerIdentificador());
   } else {
     switch (cv.modoVis) {
       case modoSuave:
        modoVisualizacion = GL_FILL;
        modoSombra = GL_SMOOTH;
        tipoVisualizacion = 0;
        break;

       case modoPlano:
        modoVisualizacion = GL_FILL;
        modoSombra = GL_FLAT;
        tipoVisualizacion = 2;
        break;

       case modoPuntos:
        modoVisualizacion = GL_POINT;
        modoSombra = GL_FLAT;
        tipoVisualizacion = 0;
        glDisable( GL_LIGHTING );
        glDisable( GL_TEXTURE_2D );
        break;

       case modoAlambre:
        modoVisualizacion = GL_LINE;
        modoSombra = GL_FLAT;
        tipoVisualizacion = 0;
        glDisable( GL_LIGHTING );
        glDisable( GL_TEXTURE_2D );
        break;

       case modoSolido:
        modoVisualizacion = GL_FILL;
        modoSombra = GL_FLAT;
        tipoVisualizacion = 0;
        glDisable( GL_LIGHTING );
        glDisable( GL_TEXTURE_2D );
        break;

       default:
        modoVisualizacion = GL_FILL;
        modoSombra = GL_FLAT;
        tipoVisualizacion = 0;
        glDisable( GL_LIGHTING );
        glDisable( GL_TEXTURE_2D );
     }
   }

  // Cambiar modo de visualización
  glPolygonMode( GL_FRONT_AND_BACK, modoVisualizacion );
  glShadeModel( modoSombra );

  if (cv.usarVBOs && cv.modoVis != modoPlano)
    visualizarDE_VBOs(cv);
  else {
    switch (tipoVisualizacion) {
      case 0:
        visualizarDE_MI( cv );
        break;
      case 1:
        visualizarBE_MI( cv );
        break;
      case 2:
        visualizarBE_TRI( cv );
        break;
    }
  }
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
  if (normalesVertices.size() > 0)
    id_vbo_normal_ver = VBO_Crear( GL_ARRAY_BUFFER, sizeof(float) * 3L * n_vertices, normalesVertices.data() );
  if (coordTextura.size() > 0)
    id_vbo_cctt = VBO_Crear( GL_ARRAY_BUFFER, sizeof(float) * 2L * n_vertices, coordTextura.data() );
  id_vbo_ver = VBO_Crear( GL_ARRAY_BUFFER, sizeof(float) * 3L * n_vertices, tablaVertices.data() );
  id_vbo_tri = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * 3L * n_triangulos, tablaTriangulos.data() );
  sinVBO = false;
}


// *****************************************************************************

void MallaInd::calcularCentroOC() {
  if (!centroCalculado) {
    ponerCentroOC(centroCajaEnglobante(tablaVertices));
    centroCalculado = true;
  }
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
  Tupla3i triangulos[n_triangulos] = { Tupla3i(0, 2, 3), Tupla3i(1, 0, 3),
    Tupla3i(1, 3, 7), Tupla3i(5, 1, 7), Tupla3i(6, 4, 2), Tupla3i(2, 0, 4),
    Tupla3i(4, 7, 6), Tupla3i(4, 5, 7), Tupla3i(2, 6, 7), Tupla3i(3, 2, 7),
    Tupla3i(4, 0, 5), Tupla3i(1, 0, 5) };
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
    Tupla3i(3, 1, 2), Tupla3i(3, 2, 0) };
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

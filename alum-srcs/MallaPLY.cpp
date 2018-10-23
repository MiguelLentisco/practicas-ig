// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );
   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................

   vector<float> vertices;
   vector<int> caras;

   ply::read(nombre_arch, vertices, caras);

   n_vertices = vertices.size() / 3;
   n_triangulos = caras.size() / 3;

   for (int i = 0; i < n_vertices; ++i)
    tablaVertices.push_back(Tupla3f(vertices[i], vertices[i + 1], vertices[i + 2]));

   for (int i = 0; i < n_triangulos; ++i)
    tablaTriangulos.push_back(Tupla3i(caras[i], caras[i + 1], caras[i + 2]));

   sinVBO = true;

   // calcular la tabla de normales
   calcular_normales();

}

// *****************************************************************************

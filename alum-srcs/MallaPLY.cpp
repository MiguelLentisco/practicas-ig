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

   vector<float> vertices;
   vector<int> caras;

   ply::read(nombre_arch.c_str(), vertices, caras);

   n_vertices = vertices.size() / 3;
   n_triangulos = caras.size() / 3;

   for (unsigned i = 0; i < vertices.size(); i += 3)
    tablaVertices.push_back(Tupla3f(vertices[i], vertices[i + 1], vertices[i + 2]));

   for (unsigned i = 0; i < caras.size(); i += 3)
    tablaTriangulos.push_back(Tupla3i(caras[i], caras[i + 1], caras[i + 2]));

   sinVBO = true;

   // calcular la tabla de normales
   calcular_normales();

}

// *****************************************************************************

// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include <math.h>
#include "MallaRevol.hpp"

using namespace std ;

// *****************************************************************************


MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla  )
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));


   vector<float> vertices;
   ply::read_vertices(nombre_arch.c_str(), vertices);

   vector<Tupla3f> perfil_original;
   for (unsigned i = 0; i < vertices.size(); i += 3)
    perfil_original.push_back(Tupla3f(vertices[i], vertices[i + 1], vertices[i + 2]));

   crearMallaRevol(perfil_original, nperfiles, crear_tapas, cerrar_malla);

   // calcular la tabla de normales
   calcular_normales();

   // Le da color aleatorio
   fijarColorNodo();
}

// *****************************************************************************

void MallaRevol::crearMallaRevol( const std::vector<Tupla3f> & perfil_original,
                                  const unsigned nperfiles,
                                  const bool     crear_tapas,
                                  const bool     cerrar_malla)
{
  sinVBO = true;
  nper = nperfiles;
  nvp = perfil_original.size();
  n_vertices = nvp * nper;

  // Ángulo de rotación
  float alfa = cerrar_malla ? (360.0 / nper) : (360.0 / (nper - 1));
  // Matriz rotación de ángulo alfa y sobre el eje y
  Matriz4f mRot = MAT_Rotacion(alfa, 0, 1, 0);
  // Añadimos primero el perfil
  for (unsigned i = 0; i < perfil_original.size(); i++)
    tablaVertices.push_back(perfil_original[i]);

  vector<Tupla3f> caraActual = perfil_original;
  // Creamos la tabla de vértices completa
  for (unsigned i = 0; i < nper - 1; ++i)
    // Rotamos los vértices de la cara actual con ángulo alfa
    for (unsigned j = 0; j < caraActual.size(); ++j) {
      caraActual[j] = mRot * caraActual[j];
      tablaVertices.push_back(caraActual[j]);
    }

  n_triangulos = 0;
  // Creamos la tabla de caras
  for (unsigned i = 0; i < nper - 1 + (int)cerrar_malla; ++i)
    for (unsigned j = 0; j < nvp - 1; ++j) {
      int k1 = i * nvp + j;
      int k2 = i * nvp + j + 1;
      int k3 = ((i + 1) % nper) * nvp + j;
      int k4 = ((i + 1) % nper) * nvp + j + 1;
      tablaTriangulos.push_back(Tupla3i(k3, k4, k2));
      tablaTriangulos.push_back(Tupla3i(k3, k2, k1));
      n_triangulos += 2;
    }

   // Creamos las tapas
   if (crear_tapas) {

     Tupla3f tapa_arriba = perfil_original[nvp - 1];
     // Si el punto no está en el eje de rotación
     if (tapa_arriba(X) != 0) {
       tablaVertices.push_back(Tupla3f(0.0, tapa_arriba(Y), 0.0));
       ++n_vertices;

       for (unsigned i = 0; i < nper - 1 + int(cerrar_malla); ++i) {
         tablaTriangulos.push_back(Tupla3i(nvp * (i + 1) - 1,
            nvp * (((i + 1) % nper) + 1) - 1, n_vertices - 1));
         ++n_triangulos;
        }
     }

     Tupla3f tapa_abajo = perfil_original[0];
     // Si el punto no está en el eje de rotación
     if (tapa_abajo(X) != 0) {
       tablaVertices.push_back(Tupla3f(0.0, tapa_abajo(Y), 0.0));
       n_vertices++;

       for (unsigned i = 0; i < nper - 1 + int(cerrar_malla); ++i) {
         tablaTriangulos.push_back(Tupla3i(i * nvp, n_vertices - 1,
            nvp * ((i + 1) % nper)));
         ++n_triangulos;
       }
     }
   }
  }

// *****************************************************************************

Cilindro::Cilindro( float r,
                    float a,
                    const unsigned nvp,
                    const unsigned nperfiles,
                    const bool     crear_tapas,
                    const bool     cerrar_malla )
{
  this->r = r;
  this->a = a;

  vector<Tupla3f> perfil_original;

  // Dividimos la recta desde el origen hasta a (altura) con nvp puntos.
  for (unsigned i = 0; i < nvp; ++i)
    perfil_original.push_back(Tupla3f(r, a * i / (nvp - 1), 0.0));

  crearMallaRevol(perfil_original, nperfiles, crear_tapas, cerrar_malla);

  // Le da color aleatorio
  fijarColorNodo();
}

Cilindro::Cilindro(float r, float a) : Cilindro(r, a, 50, 50, true, true) {
}

Cilindro::Cilindro() : Cilindro(1.0, 1.0) {
}

// *****************************************************************************


Esfera::Esfera( float r,
                const unsigned nvp,
                const unsigned nperfiles,
                const bool     crear_tapas,
                const bool     cerrar_malla )
{
  this->r = r;

  vector<Tupla3f> perfil_original;

  // Dividimos la recta entre -r y r (eje y) con nvp puntos y con ese valor y,
  // sacamos el valor x despejandolo de la fórmula de la circunferencia
  for (unsigned i = 0; i < nvp; ++i) {
    float y = r * (-1 + 2.0 * i / (nvp - 1));
    perfil_original.push_back(Tupla3f(sqrt(r * r - y * y), y, 0.0));
  }

  crearMallaRevol(perfil_original, nperfiles, crear_tapas, cerrar_malla);

  // Le da color aleatorio
  fijarColorNodo();
}

Esfera::Esfera(float r) : Esfera(r, 50, 50, true, true) {
}

Esfera::Esfera() : Esfera(1.0) {
}

// *****************************************************************************

Cono::Cono( float r,
            float a,
            const unsigned nvp,
            const unsigned nperfiles,
            const bool     crear_tapas,
            const bool     cerrar_malla )
{
  this->r = r;
  this->a = a;

  vector<Tupla3f> perfil_original;

  // Dividimos la recta entre 0 y r (eje x) con nvp puntos y con los valores
  // de x sacamos los de y proyectando sobre la recta de (0, a, 0) a (r, 0, 0)
  for (unsigned i = 0; i < nvp; ++i) {
    float x = r * (1 - 1.0 * i / (nvp - 1));
    perfil_original.push_back(Tupla3f(x, a * (- x / r + 1.0), 0));
  }

  crearMallaRevol(perfil_original, nperfiles, crear_tapas, cerrar_malla);

  // Le da color aleatorio
  fijarColorNodo();
}

Cono::Cono(float r, float a) : Cono(r, a, 50, 50, true, true) {
}

Cono::Cono() : Cono(1.0, 1.0) {
}

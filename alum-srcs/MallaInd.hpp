// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

   protected:
      bool sinVBO;

      GLuint id_vbo_ver;
      GLuint id_vbo_tri;
      GLuint id_vbo_col;
      GLuint id_vbo_normal_ver;

      unsigned n_vertices;
      unsigned n_triangulos;
      std::vector<Tupla3f> tablaVertices;
      std::vector<Tupla3i> tablaTriangulos;
      std::vector<Tupla3f> colVertices;
      std::vector<Tupla3f> normalesCaras;
      std::vector<Tupla3f> normalesVertices;

      // calculo de las normales de esta malla
      void calcular_normales();

      // visualizar con 'begin/end', en modo inmediato
      void visualizarBE_MI( ContextoVis & cv);
      // visualizar con 'draw elements', en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );
      // Creamos los dos VBOs
      void crearVBOs();

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;
      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;
      // Rellena de los colores asignados
      void colorearEntero(const std::vector<Tupla3f>& colores);
      // Rellena el mismo color para toda la figura
      void fijarColorNodo(const Tupla3f & color);
      // Rellena el mismo color para toda la figura, con color aleatorio
      void fijarColorNodo();

} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
  protected:
    float lado;
    Tupla3f centro;

   public:
      Cubo(float lado, Tupla3f centro);
      Cubo(float lado);
      Cubo(Tupla3f centro);
      Cubo();
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
  protected:
    float lado;
    Tupla3f centro;

   public:
      Tetraedro(float lado, Tupla3f centro);
      Tetraedro(float lado);
      Tetraedro(Tupla3f centro);
      Tetraedro();
};


#endif

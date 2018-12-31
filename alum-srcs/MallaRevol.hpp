// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
  protected:
    MallaRevol() {}

    unsigned
       nper , // numero de perfiles
       nvp  ; // numero de vertices por perfil

    // Método que crear las tablas vertices y tiangulos (nper y nvp ya asignados)
    void crearMallaRevol( const std::vector<Tupla3f> & perfil_original,
                          const unsigned nperfiles,
                          const bool     crear_tapas,
                          const bool     cerrar_malla,
                          const bool     crear_texturas
                        );


   public:
      // crea una malla de revolucion
      MallaRevol( const std::string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla ) ;

      MallaRevol( const std::string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla,
                  const bool     crear_texturas) ;
} ;


// *****************************************************************************

class Cilindro : public MallaRevol {
  protected:
    float a; // altura
    float r; // radio

  public:
    // El cilindro se crea con el centro en el origen
    Cilindro( float r,
              float a,
              const unsigned nvp,
              const unsigned nperfiles,
              const bool     crear_tapas,
              const bool     cerrar_malla );
    Cilindro( float r, float a );
    Cilindro();
};

// *****************************************************************************

class Esfera : public MallaRevol {
  protected:
    float r; // radio


  public:
    // La esfera se crea con centro el origen
    Esfera( float r,
            const unsigned nvp,
            const unsigned nperfiles,
            const bool     crear_tapas,
            const bool     cerrar_malla );
    Esfera( float r );
    Esfera();
};

// ***************************************************************************

class Cono : public MallaRevol {
  protected:
    float r; // radio
    float a; // altura

  public:
    // El cono se crea con el centro de la base en el origen
    Cono( float r,
          float a,
          const unsigned nvp,
          const unsigned nperfiles,
          const bool     crear_tapas,
          const bool     cerrar_malla);
    Cono( float r, float a );
    Cono();
};

#endif

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
                          const bool     cerrar_malla
                        );


   public:
      // crea una malla de revolucion
      MallaRevol( const std::string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla ) ;


} ;

#endif

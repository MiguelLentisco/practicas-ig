#include <string>
#include <functional>
#include <matrizg.hpp>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D ;


// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef std::function< Matriz4f( float )> TFuncionCMF  ;

// -----------------------------------------------------------------------------

class Parametro
{
   private:
     const std::string descripcion;
     const bool acotado; //true si oscila entre 2 valores
     const TFuncionCMF fun_calculo_matriz; // función nueva matriz a partir de float
     const float c; // valor inicial (central si acotado)
     const float s; // semiamplitud (acotado) o factor escala (no acotado)
     const float f; // si acotado, frecuencia (ciclos por unidad de valor normalizado)
     Matriz4f * const ptr_mat; // puntero a matriz del modelo
     float valor_norm; //valor actual, normalizado, no acotado (crece desde 0)
     float velocidad; // velocidad actual (se suma al valor_norm)

     const float v0 = 1.0; // velocidad inicial
     const float incremento = 1.0; // incremento de p
     const float a = 1.0; // aceleración


   public:
   // crear grado de libertad no acotado
   Parametro
   (  const std::string &  p_descripcion,
      Matriz4f *           p_ptr_mat,
      TFuncionCMF          p_fun_calculo_matriz,
      bool                 p_acotado,
      float                p_c,
      float                p_s,
      float                p_f
   );

   void  siguiente_cuadro();   // actualizar valor y matriz al siguiente frame
   void  reset();        // vuelve al estado inicial
   void  incrementar();  // incrementar el valor
   void  decrementar() ; // decrementar el valor
   void  acelerar();     // acelerar (aumentar velocidad normalizada)
   void  decelerar();    // decelerar (disminuir la velocidad normalizada)
   float leer_valor_actual(); // devuelve el valor actual (escalado, no normalizado)
   float leer_velocidad_actual();    // devuelve velocidad o frecuencia actual
   std::string
         leer_descripcion();
};

#endif

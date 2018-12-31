// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"


using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   cv.pilaMateriales.push();
   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();

   for (unsigned i = 0; i < entradas.size(); ++i) {
     if (entradas[i].tipo == TipoEntNGE::objeto)
      entradas[i].objeto->visualizarGL(cv);
     else if (entradas[i].tipo == TipoEntNGE::transformacion) {
       glMatrixMode( GL_MODELVIEW );
       glMultMatrixf(*(entradas[i].matriz));
     } else if (entradas[i].tipo == TipoEntNGE::material)
        cv.pilaMateriales.activarMaterial( entradas[i].material );
   }

   glMatrixMode( GL_MODELVIEW );
   glPopMatrix();
   cv.pilaMateriales.pop();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
   for (unsigned i = 0; i < entradas.size(); ++i)
     if (entradas[i].tipo == TipoEntNGE::objeto)
       entradas[i].objeto->fijarColorNodo( nuevo_color );
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
  entradas.push_back(entrada);
  return entradas.size() - 1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{

  if (indice >= 0 && indice < entradas.size()
      && entradas[indice].tipo == TipoEntNGE::transformacion
      && entradas[indice].matriz != nullptr )
      return entradas[indice].matriz;

   cerr << "ERROR P3: No hay una matriz (puntero no válido) para el índice"
    << indice << endl << flush;

    exit(EXIT_FAILURE);
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // ........

}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************


// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros()
{
   return parametros.size();
}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned i )
{
   assert(i >= 0 && i < parametros.size());
   return &parametros[i];
}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
   for (unsigned i = 0; i < parametros.size(); ++i)
    parametros[i].siguiente_cuadro();

}
// -----------------------------------------------------------------------------

PuntoBoca::PuntoBoca() {
  agregar(MAT_Traslacion(0.0, 0.0, 1.0));
  agregar(MAT_Escalado(0.05, 0.05, 0.05));
  agregar(new Esfera);
  fijarColorNodo(Tupla3f(0.0, 0.0, 0.0));
}

MitadBoca::MitadBoca() {
  agregar(MAT_Traslacion(0.1, 0.01, 0.0));
  agregar(new PuntoBoca);
  agregar(MAT_Traslacion(0.08, 0.02, 0.0));
  agregar(new PuntoBoca);
  agregar(MAT_Traslacion(0.07, 0.02, -0.02));
  agregar(new PuntoBoca);
  agregar(MAT_Traslacion(0.06, 0.04, -0.02));
  agregar(new PuntoBoca);
  agregar(MAT_Traslacion(0.05, 0.05, -0.02));
  agregar(new PuntoBoca);
  agregar(MAT_Traslacion(0.04, 0.08, -0.02));
  agregar(new PuntoBoca);
  agregar(MAT_Traslacion(0.02, 0.09, -0.03));
  agregar(new PuntoBoca);
}

Boca::Boca() {
  agregar(new PuntoBoca);
  agregar(new MitadBoca);
  agregar(MAT_Escalado(-1.0, 1.0, 1.0));
  agregar(new MitadBoca);
}

Nariz::Nariz() {
  agregar(MAT_Traslacion(0.0, 0.4, 0.7));
  agregar(MAT_Rotacion(90.0, 1.0, 0.0, 0.0));
  agregar(MAT_Escalado(0.1, 0.8, 0.1));
  agregar(new Cono);
  fijarColorNodo(Tupla3f(1.0, 0.5, 0.0));
}

Ojo::Ojo() {
  agregar(MAT_Traslacion(0.4, 0.6, 0.7));
  agregar(MAT_Escalado(0.1, 0.1, 0.1));
  agregar(new Esfera);
  fijarColorNodo(Tupla3f(0.0, 0.0, 0.0));
}

Ojos::Ojos() {
  agregar(new Ojo);
  agregar(MAT_Escalado(-1.0, 1.0, 1.0));
  agregar(new Ojo);
}

BaseSombrero::BaseSombrero() {
  agregar(MAT_Escalado(1.0, 0.1, 1.0));
  agregar(new Cilindro);
  fijarColorNodo(Tupla3f(0.0, 0.0, 0.0));
}

Sombrero::Sombrero() {
  agregar(MAT_Rotacion(20.0, 0.0, 0.0, 1.0));
  agregar(MAT_Traslacion(0.0, 0.95, 0.0));
  agregar(MAT_Escalado(0.5, 0.5, 0.5));
  agregar(new BaseSombrero);
  agregar(MAT_Escalado(0.7, 1.4, 0.7));
  agregar(new Cilindro);
  fijarColorNodo(Tupla3f(0.0, 0.0, 0.0));

}

PliegueBufanda::PliegueBufanda() {
  mPliegue = leerPtrMatriz(agregar(MAT_Ident()));
  agregar(MAT_Traslacion(0.5, 0.18, 0.93));
  agregar(MAT_Escalado(1.4, 0.4, 0.1));
  agregar(new Cubo);
  fijarColorNodo(Tupla3f(1.0, 0.0, 0.0));
}

Bufanda::Bufanda() {
  agregar(MAT_Traslacion(0.0, -0.7, 0.0));
  PliegueBufanda * pB = new PliegueBufanda;
  mPliegue = pB->mPliegue;
  agregar(pB);
  agregar(MAT_Escalado(1.0, 0.4, 1.0));
  agregar(new Cilindro(1.0, 1.0, 50, 50, false, true));
  fijarColorNodo(Tupla3f(1.0, 0.0, 0.0));
}

Cabeza::Cabeza() {
  agregar(MAT_Traslacion(0.0, 4.5, 0.0));
  agregar(MAT_Escalado(1.2, 1.2, 1.2));
  agregar(new Esfera);
  fijarColorNodo(Tupla3f(1.0, 1.0, 1.0));
  Bufanda * bf = new Bufanda;
  mPliegue = bf->mPliegue;
  agregar(bf);
  agregar(new Sombrero);
  agregar(new Ojos);
  agregar(new Nariz);
  agregar(new Boca);
}

Palo::Palo() {
  agregar(MAT_Escalado(0.03, 1.3, 0.03));
  agregar(new Cilindro);
  fijarColorNodo(Tupla3f(0.647, 0.165, 0.165));
}

PalitoDerecho::PalitoDerecho() {
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));
  agregar(MAT_Rotacion(300.0, 1.0, 0.0, 0.0));
  agregar(MAT_Escalado(0.80, 0.2, 0.80));
  agregar(new Palo);
}

PalitoIzquierdo::PalitoIzquierdo() {
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));
  agregar(MAT_Rotacion(60.0, 1.0, 0.0, 0.0));
  agregar(MAT_Escalado(0.80, 0.2, 0.80));
  agregar(new Palo);
}

Brazo::Brazo() {
  matrizBrazo = leerPtrMatriz(agregar(MAT_Ident()));
  agregar(MAT_Traslacion(0.6, 0.5, -0.2));
  agregar(MAT_Rotacion(30.0, 0.0, 1.0, 0.0));
  agregar(MAT_Rotacion(90.0, 1.0, 0.0, 0.0));
  agregar(new Palo);
  agregar(MAT_Escalado(0.55, 1.0, 0.55));
  agregar(new PalitoDerecho);
  agregar(new PalitoIzquierdo);
}

Brazos::Brazos() {
  Brazo * brDer = new Brazo;
  matrizDer = brDer->matrizBrazo;
  agregar(brDer);
  agregar(MAT_Escalado(-1.0, 1.0, 1.0));
  Brazo * brIzq = new Brazo;
  matrizIzq = brIzq->matrizBrazo;
  agregar(brIzq);
}

Botones::Botones() {
  agregar(MAT_Escalado(0.1, 0.1, 0.1));
  agregar(MAT_Traslacion(0.0, -1.0, 9.7));
  agregar(new Esfera);
  agregar(MAT_Traslacion(0.0, 4.0, 0.0));
  agregar(new Esfera);
  agregar(MAT_Traslacion(0.0, 4.0, -2.0));
  agregar(new Esfera);
  fijarColorNodo(Tupla3f(0.0, 0.0, 0.0));
}

Cuerpo::Cuerpo() {
  agregar(MAT_Traslacion(0.0, 2.5, 0.0));
  agregar(MAT_Escalado(2.0, 1.5, 2.0));
  agregar(new Esfera);
  fijarColorNodo(Tupla3f(1.0, 1.0, 1.0));
  agregar(new Botones);
  Brazos * brazos = new Brazos;
  matrizIzq = brazos->matrizIzq;
  matrizDer = brazos->matrizDer;
  agregar(brazos);
}

Base::Base() {
  agregar(MAT_Escalado(3.0, 2.2, 3.0));
  agregar(new Esfera);
  fijarColorNodo(Tupla3f(1.0, 1.0, 1.0));
}

Snowman::Snowman() {
  ponerNombre("raíz del modelo jerárquico");

  Cuerpo * cuerpo = new Cuerpo;
  Cabeza * cabeza = new Cabeza;

  Matriz4f * mFigura = leerPtrMatriz(agregar(MAT_Ident()));
  Matriz4f * mFigura2 = leerPtrMatriz(agregar(MAT_Ident()));
  Matriz4f * mFigura3 = leerPtrMatriz(agregar(MAT_Ident()));
  Matriz4f * mBrazoIzq = cuerpo->matrizIzq;
  Matriz4f * mBrazoDer = cuerpo->matrizDer;
  Matriz4f * mPliegue = cabeza->mPliegue;

  agregar(new Base);
  Matriz4f * mSinBase = leerPtrMatriz(agregar(MAT_Ident()));
  agregar(cuerpo);
  Matriz4f * mSinCuerpo = leerPtrMatriz(agregar(MAT_Ident()));
  agregar(cabeza);

  Parametro p1("rotación del cuerpo entero", mFigura,
    [=](float v){return MAT_Rotacion(-v, 0, 1, 0);}, true, 0, 360, 0.04);
  parametros.push_back(p1);

  Parametro p2("rotación del brazo izq", mBrazoIzq,
    [=](float v){return MAT_Rotacion(v, 1, 0, 0);}, true, 0, 50, 0.2);
  parametros.push_back(p2);

  Parametro p3("rotación del brazo der", mBrazoDer,
    [=](float v){return MAT_Rotacion(-v, 1, 0, 0);}, true, 0, 50, 0.2);
  parametros.push_back(p3);

  Parametro p4("mecedora del cuerpo entero", mFigura2,
    [=](float v){return MAT_Rotacion(v, 0, 0, 1);}, true, 0, 10, 0.1);
  parametros.push_back(p4);

  Parametro p5("rotación del pliegue de la bufanda", mPliegue,
    [=](float v){return MAT_Rotacion(v, 0, 1, 0);}, true, 0, 8, 0.5);
  parametros.push_back(p5);

  Parametro p6("movimiento der-izq", mFigura3,
    [=](float v){return MAT_Traslacion(v, 0, 0);}, true, 0, 2, 0.01);
  parametros.push_back(p6);

  Parametro p7("salto sin la base", mSinBase,
    [=](float v){return MAT_Traslacion(0, v, 0);}, true, 1.2, 1.2, 0.1, - M_PI / 2);
  parametros.push_back(p7);

  Parametro p8("salto solo cabeza", mSinCuerpo,
    [=](float v){return MAT_Traslacion(0, v, 0);}, true, 1.0, 1.0, 0.1, - M_PI / 2);
  parametros.push_back(p8);

}

//**********************************************************************


CentroLata::CentroLata()
{
  agregar(new MaterialLata());
  agregar(new MallaRevol("../plys/lata-pcue.ply", 100 , false, false, true));
}

//**********************************************************************

TapasLata::TapasLata() {
  agregar(new MaterialTapasLata());
  agregar(new MallaRevol("../plys/lata-pinf.ply", 100, false, false, true));
  agregar(new MallaRevol("../plys/lata-psup.ply", 100, false, false, true));
}

//**********************************************************************

Lata::Lata() {
  agregar(new TapasLata());
  agregar(new CentroLata());
}

//**********************************************************************

EscenaP4::EscenaP4() {
  ponerNombre("Escena P4");
  agregar(new Lata());
}

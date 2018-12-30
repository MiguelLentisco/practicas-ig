# Grafo Escena del Snowman

## Miguel Lentisco Ballesteros

### Modelo jerárquico
El modelo jerárquico que he realizado es un muñeco de nieve, y realiza
distintos movimientos como movimiento de sus brazos, giro en si mismo,
movimiento lateral, meceo y salto de sus bolas.

![Grafo Escena Snowman](GrafoEscena.svg)

<div style="page-break-after: always;"></div>

### Grados de libertad

Los grados de libertad (parámetros) comparten las siguientes constantes:
  - Velocidad inicial = 0.1
  - Incremento = 0.2
  - Aceleración = 0.03

Se ha añadido a la clase Parametro el valor float fase, que permite cambiar
la posición inicial del objeto en un movimiento acotado.

Para el objeto creado se han hecho los siguientes grados de libertad:
  - P1: Rotación del cuerpo entero, que afecta a la clase Snowman, a toda la
    figura entera. Es una rotación negativa del eje Y de 360 grados acotada (giro
    completo y vuelta), empezando en 0 y frecuencia de 0.04.
  - P2: Rotación del brazo izquierdo, afecta a la clase Brazo, al brazo izquierdo,
    que rota su brazo en sentido positivo con una rotación del eje X de 50
    grados acotada, empezando en 0 y frecuencia de 0.04.
  - P3: Rotación del brazo derecho, afecta a la clase Brazo, el brazo derecho,
    que rota su brazo en sentido negativo con una rotación del eje X de 50
    grados acotada, empezando en 0 y frecuencia de 0.04.
  - P4: Mecedora del cuerpo entero, afecta a la clase Snowman, al cuerpo entero,
    mece el cuerpo de un lado a otro, rotando sobre el eje Z 10 grados acotada,
    inicialmente 0 y frecuencia 0.1.
  - P5: Rotación del pliegue de la bufanda, afecta a la clase PliegueBufanda,
    al pliegue que sobresale de la bufanda, rotación sobre el eje Y de 8 grados
    acotada, inicialmente 0 y frecuencia 0.5.
  - P6: Movimiento derecha-izquierda, afecta a la clase Snowman, a toda la figura
    que hace una traslación sobre el eje X de 2 unidades acotada, incialmente
    0 y frecuencia 0.01.
  - P7: Salto sin la base, afecta a la clase Snowman, afecta a la cabeza y al
    cuerpo pero no a la base, traslación en el eje Y de 1.2 unidades acotada,
    de centro 1.2, frecuencia 0.1 y desfase - PI / 2, haciendo que empiece el
    movimiento en el estado original del muñeco.
  - P8: Salto solo de la cabeza, afecta a la clase Snowman, afecta únicamente
    a la cabeza, traslación en el eje Y de 1.0 unidades acotada, de centro 1.0,
    frecuencia 0.1 y desfase - PI / 2, haciendo que empiece el movimiento en el
    estado original de muñeco.

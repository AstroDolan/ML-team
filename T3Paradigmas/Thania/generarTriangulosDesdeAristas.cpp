#include "Triangulo.h"
#include "Arista.h"
#include <set>
#include <vector>

// Devuelve un set de triángulos a partir de las aristas (O(n^3), pero aceptable para datos pequeños)
std::set<Triangulo> generarTriangulosDesdeAristas(const std::set<Arista>& aristas) {
    std::set<Triangulo> triangulos;
    std::vector<Arista> aristasVec(aristas.begin(), aristas.end());

    for (size_t i = 0; i < aristasVec.size(); ++i) {
        for (size_t j = i + 1; j < aristasVec.size(); ++j) {
            for (size_t k = j + 1; k < aristasVec.size(); ++k) {
                const Arista& a = aristasVec[i];
                const Arista& b = aristasVec[j];
                const Arista& c = aristasVec[k];

                // Obtener todos los puntos involucrados
                std::set<Punto> puntosUnicos;
                puntosUnicos.insert(a.p1); puntosUnicos.insert(a.p2);
                puntosUnicos.insert(b.p1); puntosUnicos.insert(b.p2);
                puntosUnicos.insert(c.p1); puntosUnicos.insert(c.p2);

                // Solo considerar si hay 3 puntos únicos (forman un triángulo)
                if (puntosUnicos.size() == 3) {
                    std::vector<Punto> puntos(puntosUnicos.begin(), puntosUnicos.end());
                    Triangulo t(puntos[0], puntos[1], puntos[2]);

                    // Confirmar que las 3 aristas están realmente en el conjunto original
                    if (aristas.count(t.aristas[0]) &&
                        aristas.count(t.aristas[1]) &&
                        aristas.count(t.aristas[2])) {
                        triangulos.insert(t);
                    }
                }
            }
        }
    }

    return triangulos;
}

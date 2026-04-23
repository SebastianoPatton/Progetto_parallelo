using namespace std;

#include <vector>
#include <string>
#include <cstdint>

// ============================================================
//  Struttura parametri
// ============================================================

struct MandelbrotParams {
    int    width;       // larghezza immagine in pixel
    int    height;      // altezza immagine in pixel
    int    max_iter;    // iterazioni massime per pixel
    double x_min;       // estremo sinistro del piano complesso
    double x_max;       // estremo destro del piano complesso
    double y_min;       // estremo inferiore del piano complesso
    double y_max;       // estremo superiore del piano complesso
};

// ============================================================
//  Dichiarazioni funzioni
// ============================================================


int compute_pixel(double cx, double cy, int max_iter);

/**
 * Calcola il numero di iterazioni prima che il punto (cx, cy)
 * del piano complesso diverga (|z| > 2).
 * Restituisce max_iter se il punto appartiene all'insieme.
 */


void render(vector <vector<int>> & image, 
            const MandelbrotParams & params);

/**
 * Calcola l'intera immagine riempiendo la matrice image[][].
 * image[i][j] conterrà il numero di iterazioni del pixel (j, i).
 */

void iter_to_color(int iter, int max_iter, 
                   uint8_t & r, uint8_t & g, uint8_t & b);

/**
 * uint8_t è un tipo intero senza segno a 8 bit (0-255), ideale per i canali RGB.
 * Converte un valore di iterazione in un colore RGB.
 * I pixel dell'insieme (iter == max_iter) sono neri.
 */

bool save_png(const string & filename,
              const vector <vector<int>> & image,
              const MandelbrotParams & params);

/**
 * Salva l'immagine come file PNG usando stb_image_write.
 * Restituisce true in caso di successo.
 */


MandelbrotParams load_params(const string & filename);

/**
 * Carica i parametri da un file di testo nel formato:
 *   width     <int>
 *   height    <int>
 *   max_iter  <int>
 *   x_min     <double>
 *   x_max     <double>
 *   y_min     <double>
 *   y_max     <double>
 */

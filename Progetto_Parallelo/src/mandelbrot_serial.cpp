// ============================================================
//  mandelbrot_serial.cpp
//  Calcolo seriale dell'insieme di Mandelbrot con output PNG.
//
//  Uso: ./mandelbrot_serial <params_file> <output_file>
//  Es.: ./mandelbrot_serial inputs/params.txt outputs/mandelbrot.png
// ============================================================
using namespace std;
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "mandelbrot.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include <stdexcept>

// ============================================================
//  compute_pixel: calcola il numero di iterazioni per il punto (cx, cy)
// ============================================================

int compute_pixel(double cx, double cy, int max_iter) {
    double zx = 0.0;    //parte reale di z
    double zy = 0.0;    //parte immaginaria di z
    int iter = 0;       // contatore iterazioni

    // Itera z = z² + c finché |z|² > 4  oppure si raggiunge max_iter
    while (zx * zx + zy * zy <= 4.0 && iter < max_iter) {           // |z|² = zx² + zy²
        double tmp = zx * zx - zy * zy + cx;                        // parte reale di z²+c
        zy = 2.0 * zx * zy + cy;                                    // parte immaginaria di z²+c
        zx = tmp;
        ++iter;
    }
    return iter;
}

// ============================================================
//  render: calcola l'intera immagine
// ============================================================

void render(vector<vector<int>>& image,
            const MandelbrotParams& params) {
    //  x = x_min + col * (x_max - x_min) / width
    // ogni pixel orizzontale vale dx o dY unità nel piano complesso
    const double dx = (params.x_max - params.x_min) / params.width; 
    const double dy = (params.y_max - params.y_min) / params.height;

    for (int row = 0; row < params.height; ++row) {
        // Mappa la riga al valore y del piano complesso
        double cy = params.y_min + row * dy;

        for (int col = 0; col < params.width; ++col) {
            // Mappa la colonna al valore x del piano complesso 
            double cx = params.x_min + col * dx;

            image[row][col] = compute_pixel(cx, cy, params.max_iter);
        }
    }
}

// ============================================================
//  iter_to_color  —  palette liscia basata su coseno 
// ============================================================

void iter_to_color(int iter, int max_iter,
                   uint8_t& r, uint8_t& g, uint8_t& b) {

    // Pixel dentro l'insieme → nero
    if (iter == max_iter) {
        r = g = b = 0;
        return;
    }

    // Smooth coloring: mappa iter in [0, 1] con curva logaritmica
    double t = static_cast<double>(iter) / static_cast<double>(max_iter);

    // Palette a tre canali sfasati (dà un bel gradiente blu-arancio-bianco)
    r = static_cast<uint8_t>(9   * (1 - t) * t * t * t * 255);
    g = static_cast<uint8_t>(15  * (1 - t) * (1 - t) * t * t * 255);
    b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
}

// ============================================================
//  save_png: salva l'immagine su file usando stb_image_write 
// ============================================================

bool save_png(const string& filename,
              const vector<vector<int>>& image,
              const MandelbrotParams& params) {

    const int W = params.width;
    const int H = params.height;
    const int channels = 3;  // RGB

    // Buffer piatto richiesto da stb_image_write (row-major, RGB)
    vector<uint8_t> pixels(W * H * channels);

    for (int row = 0; row < H; ++row) {
        for (int col = 0; col < W; ++col) {
            uint8_t r, g, b;
            iter_to_color(image[row][col], params.max_iter, r, g, b);

            int idx = (row * W + col) * channels;
            pixels[idx + 0] = r;
            pixels[idx + 1] = g;
            pixels[idx + 2] = b;
        }
    }

    int result = stbi_write_png(filename.c_str(), W, H, channels,
                                pixels.data(), W * channels);
    return result != 0;
}

// ============================================================
//  load_params
// ============================================================

MandelbrotParams load_params(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Impossibile aprire il file: " + filename);
    }

    MandelbrotParams p{};
    string key;
    double value;

    while (file >> key >> value) {
        if      (key == "width")    p.width    = static_cast<int>(value);
        else if (key == "height")   p.height   = static_cast<int>(value);
        else if (key == "max_iter") p.max_iter = static_cast<int>(value);
        else if (key == "x_min")    p.x_min    = value;
        else if (key == "x_max")    p.x_max    = value;
        else if (key == "y_min")    p.y_min    = value;
        else if (key == "y_max")    p.y_max    = value;
        else {
            cerr << "Parametro sconosciuto ignorato: " << key << "\n";
        }
    }
    return p;
}

// ============================================================
//  main
// ============================================================

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Uso: " << argv[0]
                  << " <params_file> <output_png>\n"
                  << "Es.: " << argv[0]
                  << " inputs/params.txt outputs/mandelbrot.png\n";
        return 1;
    }

    const string params_file  = argv[1];
    const string output_file  = argv[2];

    // --- Carica parametri ---
    MandelbrotParams params;
    try {
        params = load_params(params_file);
    } catch (const exception& e) {
        std::cerr << "Errore lettura parametri: " << e.what() << "\n";
        return 1;
    }

    cout << "=== Mandelbrot Serial ===\n"
              << "Risoluzione : " << params.width  << " x " << params.height << "\n"
              << "Max iter    : " << params.max_iter << "\n"
              << "Piano reale : [" << params.x_min << ", " << params.x_max << "]\n"
              << "Piano imm.  : [" << params.y_min << ", " << params.y_max << "]\n"
              << "Output      : " << output_file << "\n"
              << "Calcolo in corso...\n";

    // --- Alloca immagine ---
    vector<vector<int>> image(params.height,
                                        vector<int>(params.width, 0));

    // --- Calcolo (solo questa parte viene misurata) ---
    auto t_start = chrono::high_resolution_clock::now();
    render(image, params);
    auto t_end   = chrono::high_resolution_clock::now();

    double elapsed = chrono::duration<double>(t_end - t_start).count();
    cout << "Tempo di calcolo: " << elapsed << " s\n";

    // --- Salvataggio PNG (escluso dal tempo) ---
    if (!save_png(output_file, image, params)) {
        cerr << "Errore: impossibile salvare " << output_file << "\n";
        return 1;
    }
    cout << "Immagine salvata con successo.\n";

    return 0;
}

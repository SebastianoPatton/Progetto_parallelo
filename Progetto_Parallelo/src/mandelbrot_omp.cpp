// ============================================================
//  mandelbrot_omp.cpp
//  Calcolo PARALLELO dell'insieme di Mandelbrot con OpenMP.
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
#include <omp.h> // --- OPENMP: Libreria necessaria per le funzioni OpenMP ---

// La funzione compute_pixel rimane IDENTICA al seriale
int compute_pixel(double cx, double cy, int max_iter) {
    double zx = 0.0;
    double zy = 0.0;
    int iter = 0;

    while (zx * zx + zy * zy <= 4.0 && iter < max_iter) {
        double tmp = zx * zx - zy * zy + cx;
        zy = 2.0 * zx * zy + cy;
        zx = tmp;
        ++iter;
    }
    return iter;
}

// ============================================================
//  render: calcola l'intera immagine IN PARALLELO
// ============================================================
void render(vector<vector<int>>& image, const MandelbrotParams& params) {
    const double dx = (params.x_max - params.x_min) / params.width; 
    const double dy = (params.y_max - params.y_min) / params.height;

    // --- OPENMP: La magia avviene qui! ---
    // Diciamo al compilatore di dividere il ciclo for sui vari thread.
    // Usiamo schedule(dynamic) per bilanciare il carico sbilanciato di Mandelbrot.
    #pragma omp parallel for schedule(runtime)
    for (int row = 0; row < params.height; ++row) {
        double cy = params.y_min + row * dy;

        for (int col = 0; col < params.width; ++col) {
            double cx = params.x_min + col * dx;
            image[row][col] = compute_pixel(cx, cy, params.max_iter);
        }
    }
}

// Le funzioni iter_to_color e save_png rimangono IDENTICHE
void iter_to_color(int iter, int max_iter, uint8_t& r, uint8_t& g, uint8_t& b) {
    if (iter == max_iter) {
        r = g = b = 0;
        return;
    }
    double t = static_cast<double>(iter) / static_cast<double>(max_iter);
    r = static_cast<uint8_t>(9   * (1 - t) * t * t * t * 255);
    g = static_cast<uint8_t>(15  * (1 - t) * (1 - t) * t * t * 255);
    b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
}

bool save_png(const string& filename, const vector<vector<int>>& image, const MandelbrotParams& params) {
    const int W = params.width;
    const int H = params.height;
    const int channels = 3;
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
    return stbi_write_png(filename.c_str(), W, H, channels, pixels.data(), W * channels) != 0;
}

MandelbrotParams load_params(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) throw runtime_error("Impossibile aprire il file: " + filename);
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
    }
    return p;
}

int main(int argc, char* argv[]) {
    // 1. Controlliamo che gli argomenti siano 3 oppure 4
    if (argc < 3 || argc > 4) {
        cerr << "Uso: " << argv[0] << " <params_file> <output_png> [num_threads]\n";
        return 1;
    }

    const string params_file  = argv[1];
    const string output_file  = argv[2];

    // 2. Se l'utente ha passato il numero di thread (4° argomento), lo impostiamo
    if (argc == 4) {
        int threads_richiesti = stoi(argv[3]);
        omp_set_num_threads(threads_richiesti);
    }

    MandelbrotParams params;
    try {
        params = load_params(params_file);
    } catch (const exception& e) {
        std::cerr << "Errore: " << e.what() << "\n";
        return 1;
    }

    // 3. Recuperiamo quanti thread verranno EFFETTIVAMENTE usati da OpenMP
    int num_threads = omp_get_max_threads();

    cout << "=== Mandelbrot OpenMP ===\n"
         << "Risoluzione : " << params.width  << " x " << params.height << "\n"
         << "Thread Usati: " << num_threads << "\n" // Aggiornato per mostrare i thread reali
         << "Calcolo in corso...\n";

    vector<vector<int>> image(params.height, vector<int>(params.width, 0));

    auto t_start = chrono::high_resolution_clock::now();
    render(image, params);
    auto t_end   = chrono::high_resolution_clock::now();

    double elapsed = chrono::duration<double>(t_end - t_start).count();
    cout << "Tempo di calcolo: " << elapsed << " s\n";

    if (!save_png(output_file, image, params)) {
        cerr << "Errore salvataggio PNG\n";
        return 1;
    }
    cout << "Immagine salvata con successo.\n";

    return 0;
}
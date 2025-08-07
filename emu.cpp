#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

#define PI 3.14159265358979323846

// Leitor simples de WAV PCM 16-bit estéreo
vector<double> readWavMono(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Erro ao abrir " << filename << endl;
        return {};
    }

    char header[44];
    file.read(header, 44);  // Pula cabeçalho

    vector<double> samples;
    while (!file.eof()) {
        int16_t left, right;
        file.read(reinterpret_cast<char*>(&left), sizeof(int16_t));
        file.read(reinterpret_cast<char*>(&right), sizeof(int16_t));
        samples.push_back(left / 32768.0);  // Normaliza
    }

    return samples;
}

// Mini FFT (magnitude) para potências de 2
vector<double> computeFFT(const vector<double>& signal) {
    size_t N = signal.size();
    vector<double> magnitudes(N / 2);

    for (size_t k = 0; k < N / 2; ++k) {
        double real = 0.0, imag = 0.0;
        for (size_t n = 0; n < N; ++n) {
            real += signal[n] * cos(2 * PI * k * n / N);
            imag -= signal[n] * sin(2 * PI * k * n / N);
        }
        magnitudes[k] = sqrt(real * real + imag * imag);
    }

    return magnitudes;
}

// Compara FFTs via correlação simples
double compareAudio(const vector<double>& a, const vector<double>& b) {
    size_t size = min(a.size(), b.size());
    double dot = 0, normA = 0, normB = 0;

    for (size_t i = 0; i < size; ++i) {
        dot += a[i] * b[i];
        normA += a[i] * a[i];
        normB += b[i] * b[i];
    }

    return dot / (sqrt(normA) * sqrt(normB));  // Correlação de cosseno
}

int main() {
    vector<double> audio1 = readWavMono("audio1.wav");
    vector<double> audio2 = readWavMono("audio2.wav");

    if (audio1.empty() || audio2.empty()) {
        cerr << "Erro ao carregar os áudios!" << endl;
        return 1;
    }

    size_t fftSize = 1024;
    audio1.resize(fftSize);
    audio2.resize(fftSize);

    vector<double> fft1 = computeFFT(audio1);
    vector<double> fft2 = computeFFT(audio2);

    double similarity = compareAudio(fft1, fft2);

    cout << "Similaridade (0 a 1): " << similarity << endl;

    return 0;
}

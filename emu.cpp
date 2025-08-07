#include <iostream>
#include <cmath>
#include <vector>

// Simulando dois áudios com tom diferente
std::vector<float> generateSineWave(float freq, int sampleRate, int samples) {
    std::vector<float> wave(samples);
    for (int i = 0; i < samples; ++i) {
        wave[i] = sin(2 * M_PI * freq * i / sampleRate);
    }
    return wave;
}

// Função para comparar frequência dominante
float detectDominantFrequency(const std::vector<float>& audio, int sampleRate) {
    int N = audio.size();
    int maxIndex = 0;
    float maxVal = 0;

    for (int k = 0; k < N / 2; ++k) {
        float re = 0, im = 0;
        for (int n = 0; n < N; ++n) {
            re += audio[n] * cos(2 * M_PI * k * n / N);
            im -= audio[n] * sin(2 * M_PI * k * n / N);
        }
        float mag = sqrt(re * re + im * im);
        if (mag > maxVal) {
            maxVal = mag;
            maxIndex = k;
        }
    }

    return maxIndex * sampleRate / N;
}

int main() {
    const int sampleRate = 8000;
    const int samples = 1024;

    auto audioA = generateSineWave(440.0, sampleRate, samples); // A4
    auto audioB = generateSineWave(445.0, sampleRate, samples); // quase igual

    float freqA = detectDominantFrequency(audioA, sampleRate);
    float freqB = detectDominantFrequency(audioB, sampleRate);

    std::cout << "Freq A: " << freqA << " Hz" << std::endl;
    std::cout << "Freq B: " << freqB << " Hz" << std::endl;

    float diff = std::abs(freqA - freqB);
    if (diff < 5.0)
        std::cout << "Áudios semelhantes!" << std::endl;
    else
        std::cout << "Áudios diferentes." << std::endl;

    return 0;
}

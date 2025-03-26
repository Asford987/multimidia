#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;

struct PGMImage {
    int width;
    int height;
    int maxVal;
    vector<int> pixels;
};

PGMImage readPGM(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: cannot open file " << filename << endl;
        exit(1);
    }
    string line;
    // Read magic number (must be "P2")
    getline(file, line);
    if (line != "P2") {
        cerr << "Error: Not a P2 PGM file" << endl;
        exit(1);
    }
    // Skip comment lines (lines starting with '#')
    while(getline(file, line)) {
        if(line[0] != '#') break;
    }
    // Read image dimensions
    stringstream ss(line);
    int width, height;
    ss >> width >> height;
    // Read maximum gray value
    int maxVal;
    file >> maxVal;
    vector<int> pixels;
    pixels.reserve(width * height);
    int val;
    while(file >> val) {
        pixels.push_back(val);
    }
    if(pixels.size() != (size_t)width * height) {
        cerr << "Error: pixel count does not match width*height" << endl;
        exit(1);
    }
    return {width, height, maxVal, pixels};
}

void writePGM(const string &filename, const PGMImage &img) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: cannot open file " << filename << endl;
        exit(1);
    }
    file << "P2\n";
    file << img.width << " " << img.height << "\n";
    file << img.maxVal << "\n";
    int count = 0;
    for (int i = 0; i < img.pixels.size(); i++) {
        file << img.pixels[i] << " ";
        count++;
        if(count % img.width == 0)
            file << "\n";
    }
}

vector<int> computeFrequency(const vector<int>& pixels, int maxVal) {
    vector<int> freq(maxVal+1, 0);
    for (int p : pixels) {
        freq[p]++;
    }
    return freq;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cerr << "Usage: " << argv[0] << " input_pgm output_codestream" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];

    // Read the PGM image file
    PGMImage img = readPGM(inputFile);

    // Compute frequency table for pixel values
    vector<int> freq = computeFrequency(img.pixels, img.maxVal);
    int total = img.width * img.height;

    // Compute probability and cumulative probability tables
    vector<double> prob(img.maxVal+1, 0.0);
    vector<double> cumulative(img.maxVal+2, 0.0); // cumulative[0]=0, cumulative[maxVal+1]=1
    for (int i = 0; i <= img.maxVal; i++) {
        prob[i] = static_cast<double>(freq[i]) / total;
    }
    cumulative[0] = 0.0;
    for (int i = 0; i <= img.maxVal; i++) {
        cumulative[i+1] = cumulative[i] + prob[i];
    }

    // Arithmetic encoding: narrow the interval for each symbol
    double low = 0.0, high = 1.0;
    for (int symbol : img.pixels) {
        double range = high - low;
        high = low + range * cumulative[symbol+1];
        low  = low + range * cumulative[symbol];
    }
    // Choose the code as the midpoint of the final interval
    double code = (low + high) / 2.0;

    // Write the codestream file:
    // Format: width height, maxVal, total pixel count, frequency table, and the encoded value.
    ofstream out(outputFile);
    if (!out) {
        cerr << "Error: cannot open output file " << outputFile << endl;
        exit(1);
    }
    out << img.width << " " << img.height << "\n";
    out << img.maxVal << "\n";
    out << total << "\n";
    for (int i = 0; i <= img.maxVal; i++) {
        out << freq[i] << " ";
    }
    out << "\n";
    out << setprecision(17) << code << "\n";
    out.close();

    // Report file sizes and compression ratio
    ifstream inFile(inputFile, ios::binary | ios::ate);
    ifstream codFile(outputFile, ios::binary | ios::ate);
    streamsize sizeInput = inFile.tellg();
    streamsize sizeCod = codFile.tellg();

    cout << "Encoding completed.\n";
    cout << "Original file size: " << sizeInput << " bytes\n";
    cout << "Codestream file size: " << sizeCod << " bytes\n";
    double compressionRatio = static_cast<double>(sizeInput) / sizeCod;
    cout << "Compression Ratio: " << compressionRatio << "\n";

    return 0;
}

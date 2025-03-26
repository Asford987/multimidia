#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

struct PGMImage {
    int width;
    int height;
    int maxVal;
    vector<int> pixels;
};

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

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cerr << "Usage: " << argv[0] << " input_codestream output_pgm" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];

    // Read the codestream file
    ifstream in(inputFile);
    if (!in) {
        cerr << "Error: cannot open input file " << inputFile << endl;
        exit(1);
    }
    int width, height, maxVal, total;
    in >> width >> height;
    in >> maxVal;
    in >> total;
    vector<int> freq(maxVal+1, 0);
    for (int i = 0; i <= maxVal; i++) {
        in >> freq[i];
    }
    double code;
    in >> code;
    in.close();

    // Recompute probability and cumulative probability tables
    vector<double> prob(maxVal+1, 0.0);
    vector<double> cumulative(maxVal+2, 0.0);
    for (int i = 0; i <= maxVal; i++) {
        prob[i] = static_cast<double>(freq[i]) / total;
    }
    cumulative[0] = 0.0;
    for (int i = 0; i <= maxVal; i++) {
        cumulative[i+1] = cumulative[i] + prob[i];
    }

    // Decode the arithmetic code to recover the pixel values
    vector<int> decoded;
    decoded.reserve(total);
    double value = code;
    for (int i = 0; i < total; i++) {
        int symbol = 0;
        // Find the symbol such that cumulative[s] <= value < cumulative[s+1]
        for (int s = 0; s <= maxVal; s++) {
            if (value >= cumulative[s] && value < cumulative[s+1]) {
                symbol = s;
                break;
            }
        }
        decoded.push_back(symbol);
        double range = cumulative[symbol+1] - cumulative[symbol];
        if(range == 0) {
            cerr << "Decoding error: zero range encountered." << endl;
            exit(1);
        }
        value = (value - cumulative[symbol]) / range;
    }

    // Create a PGM image from the decoded pixels and write it out
    PGMImage img;
    img.width = width;
    img.height = height;
    img.maxVal = maxVal;
    img.pixels = decoded;
    writePGM(outputFile, img);

    cout << "Decoding completed. Output saved to " << outputFile << endl;
    return 0;
}

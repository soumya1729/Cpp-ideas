#include <iostream>
#include <opencv2/opencv.hpp>
#include <bitset>
#include <string>
#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

// Function to convert a string to a binary string
string stringToBinary(const string& text) {
    string binary = "";
    for (char c : text) {
        binary += bitset<8>(c).to_string();
    }
    return binary;
}

// Function to convert a binary string back to text
string binaryToString(const string& binary) {
    string text = "";
    for (size_t i = 0; i < binary.size(); i += 8) {
        bitset<8> bits(binary.substr(i, 8));
        text += char(bits.to_ulong());
    }
    return text;
}

// Function to ensure the input image is in PNG format
string convertToPNG(const string& inputPath) {
    Mat image = imread(inputPath);
    if (image.empty()) {
        cerr << "Error: Could not open or find the input image!" << endl;
        exit(1);
    }

    string pngPath = fs::path(inputPath).stem().string() + ".png";
    imwrite(pngPath, image);
    cout << "Converted input image to PNG: " << pngPath << endl;
    return pngPath;
}

// Function to hide a message in an image
void hideMessage(const string& imagePath, const string& outputImagePath, const string& message) {
    Mat image = imread(imagePath);
    if (image.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return;
    }

    string binaryMessage = stringToBinary(message) + "11111111"; // Add delimiter
    size_t messageIndex = 0;

    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            for (int channel = 0; channel < image.channels(); ++channel) {
                if (messageIndex < binaryMessage.size()) {
                    // Modify the least significant bit
                    Vec3b& pixel = image.at<Vec3b>(row, col);
                    pixel[channel] &= 0xFE; // Clear LSB
                    pixel[channel] |= binaryMessage[messageIndex] - '0'; // Set LSB
                    ++messageIndex;
                }
            }
        }
    }

    imwrite(outputImagePath, image);
    cout << "Message hidden successfully in " << outputImagePath << endl;
}

// Function to retrieve a message from an image
string retrieveMessage(const string& imagePath) {
    Mat image = imread(imagePath);
    if (image.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return "";
    }

    string binaryMessage = "";

    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            for (int channel = 0; channel < image.channels(); ++channel) {
                // Retrieve the least significant bit
                Vec3b pixel = image.at<Vec3b>(row, col);
                binaryMessage += (pixel[channel] & 1) + '0';

                // Check for the delimiter
                if (binaryMessage.size() >= 8 &&
                    binaryMessage.substr(binaryMessage.size() - 8) == "11111111") {
                    return binaryToString(binaryMessage.substr(0, binaryMessage.size() - 8));
                }
            }
        }
    }

    cerr << "Error: Delimiter not found in the image!" << endl;
    return "";
}

int main() {
    string imagePath, outputImagePath, message;

    cout << "Enter the path of the input image: ";
    getline(cin, imagePath);

    // Convert to PNG if necessary
    string pngPath = convertToPNG(imagePath);

    cout << "Enter the path for the output image: ";
    getline(cin, outputImagePath);

    // Ensure the output image has a PNG extension
    if (fs::path(outputImagePath).extension() != ".png") {
        outputImagePath = fs::path(outputImagePath).stem().string() + ".png";
        cout << "Output image renamed to: " << outputImagePath << endl;
    }

    cout << "Enter the secret message to hide: ";
    getline(cin, message);

    // Hide the message
    hideMessage(pngPath, outputImagePath, message);

    // Retrieve the message
    cout << "Retrieving the hidden message from the output image..." << endl;
    string extractedMessage = retrieveMessage(outputImagePath);

    if (!extractedMessage.empty()) {
        cout << "Extracted Message: " << extractedMessage << endl;
    }

    return 0;
}

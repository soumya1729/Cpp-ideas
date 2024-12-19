#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Function to encrypt plaintext using Vigenère cipher
string encrypt(string plaintext, string keyword) {
    string ciphertext = "";
    int keyIndex = 0;
    for (int i = 0; i < plaintext.length(); ++i) {
        char currentChar = plaintext[i];

        // Only encrypt alphabetic characters
        if (isalpha(currentChar)) {
            char keyChar = tolower(keyword[keyIndex % keyword.length()]);
            int shift = keyChar - 'a'; // Shift based on keyword character
            char encryptedChar;

            // Encrypt uppercase characters
            if (isupper(currentChar)) {
                encryptedChar = ((currentChar - 'A' + shift) % 26) + 'A';
            }
            // Encrypt lowercase characters
            else {
                encryptedChar = ((currentChar - 'a' + shift) % 26) + 'a';
            }

            ciphertext += encryptedChar;
            keyIndex++; // Move to the next letter of the keyword
        } else {
            // If non-alphabet character, add it as-is
            ciphertext += currentChar;
        }
    }
    return ciphertext;
}

// Function to decrypt ciphertext using Vigenère cipher
string decrypt(string ciphertext, string keyword) {
    string plaintext = "";
    int keyIndex = 0;
    for (int i = 0; i < ciphertext.length(); ++i) {
        char currentChar = ciphertext[i];

        // Only decrypt alphabetic characters
        if (isalpha(currentChar)) {
            char keyChar = tolower(keyword[keyIndex % keyword.length()]);
            int shift = keyChar - 'a'; // Shift based on keyword character
            char decryptedChar;

            // Decrypt uppercase characters
            if (isupper(currentChar)) {
                decryptedChar = ((currentChar - 'A' - shift + 26) % 26) + 'A';
            }
            // Decrypt lowercase characters
            else {
                decryptedChar = ((currentChar - 'a' - shift + 26) % 26) + 'a';
            }

            plaintext += decryptedChar;
            keyIndex++; // Move to the next letter of the keyword
        } else {
            // If non-alphabet character, add it as-is
            plaintext += currentChar;
        }
    }
    return plaintext;
}

int main() {
    string plaintext, keyword;

    // Input the plaintext and the keyword
    cout << "Enter the plaintext: ";
    getline(cin, plaintext);
    cout << "Enter the keyword: ";
    getline(cin, keyword);

    // Ensure keyword is all alphabetic
    for (char &c : keyword) {
        c = tolower(c);
    }

    // Encrypt the plaintext
    string encryptedText = encrypt(plaintext, keyword);
    cout << "Encrypted Text: " << encryptedText << endl;

    // Decrypt the ciphertext
    string decryptedText = decrypt(encryptedText, keyword);
    cout << "Decrypted Text: " << decryptedText << endl;

    return 0;
}

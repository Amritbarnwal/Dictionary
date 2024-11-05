#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

// Node structure for Trie
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    string meaning;  // Store meaning in the leaf node
    bool isEndOfWord; // To mark the end of a word

    TrieNode() : isEndOfWord(false) {}
};

// Trie class
class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // Function to insert a word and its meaning
    void insert(const string& word, const string& meaning) {
        TrieNode* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
        current->meaning = meaning; // Store meaning at the end node
    }

    // Function to search for a word and display its meaning
    bool search(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                return false; // Word not found
            }
            current = current->children[ch];
        }
        return current->isEndOfWord; // Check if it's the end of a valid word
    }

    // Function to get the meaning of a word
    string getMeaning(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                return ""; // Word not found
            }
            current = current->children[ch];
        }
        return current->isEndOfWord ? current->meaning : ""; // Return meaning if valid word
    }

    // Function to display words starting with a given prefix
    void displayPrefix(TrieNode* node, string prefix) {
        if (node->isEndOfWord) {
            cout << prefix << ": " << node->meaning << endl;
        }
        for (auto& pair : node->children) {
            displayPrefix(pair.second, prefix + pair.first);
        }
    }

    void displayWordsWithPrefix(const string& prefix) {
        TrieNode* current = root;
        for (char ch : prefix) {
            if (!current->children.count(ch)) {
                cout << "No words found with prefix \"" << prefix << "\".\n";
                return;
            }
            current = current->children[ch];
        }
        cout << "Words starting with \"" << prefix << "\":\n";
        displayPrefix(current, prefix);
    }

    // Function to delete a word by unmarking the end of word
    void deleteWord(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                cout << "Word \"" << word << "\" not found.\n";
                return;
            }
            current = current->children[ch];
        }
        if (current->isEndOfWord) {
            current->isEndOfWord = false;
            cout << "Deleted word \"" << word << "\" successfully.\n";
        } else {
            cout << "Word \"" << word << "\" not found as a complete word.\n";
        }
    }

    // Function to save the dictionary to a file
    void saveToFile(ostream& os, TrieNode* node, const string& word) {
        if (node->isEndOfWord) {
            os << word << "\n" << node->meaning << "\n";
        }
        for (auto& pair : node->children) {
            saveToFile(os, pair.second, word + pair.first);
        }
    }

    void save(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            saveToFile(file, root, "");
            file.close();
        } else {
            cout << "Unable to open file.\n";
        }
    }

    // Function to load the dictionary from a file
    void load(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Unable to open file.\n";
            return;
        }
        string word, meaning;
        while (getline(file, word) && getline(file, meaning)) {
            insert(word, meaning);
        }
        file.close();
    }
};

int main() {
    Trie dictionary;
    dictionary.load("dictionary.txt"); // Load existing dictionary from file

    int choice;
    do {
        cout << "\nDictionary Menu:\n";
        cout << "1. Insert Word\n";
        cout << "2. Search Word\n";
        cout << "3. Show Meaning\n";
        cout << "4. Show Words with Prefix\n";
        cout << "5. Delete Word\n";  // New option for deletion
        cout << "6. Save and Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string word, meaning;
                cout << "Enter word to insert: ";
                cin >> word;
                cout << "Enter meaning of the word: ";
                cin.ignore();
                getline(cin, meaning);
                dictionary.insert(word, meaning);
                cout << word << " inserted successfully.\n";
                break;
            }
            case 2: {
                string searchTerm;
                cout << "Enter word to search: ";
                cin >> searchTerm;
                if (dictionary.search(searchTerm)) {
                    cout << searchTerm << " found.\n";
                } else {
                    cout << searchTerm << " not found in the dictionary.\n";
                }
                break;
            }
            case 3: {
                string searchTerm;
                cout << "Enter word to get meaning: ";
                cin >> searchTerm;
                string meaning = dictionary.getMeaning(searchTerm);
                if (!meaning.empty()) {
                    cout << searchTerm << " found: " << meaning << endl;
                } else {
                    cout << searchTerm << " not found in the dictionary.\n";
                }
                break;
            }
            case 4: {
                string prefix;
                cout << "Enter prefix to search: ";
                cin >> prefix;
                dictionary.displayWordsWithPrefix(prefix);
                break;
            }
            case 5: {  // New case for deletion
                string word;
                cout << "Enter word to delete: ";
                cin >> word;
                dictionary.deleteWord(word);
                break;
            }
            case 6:
                dictionary.save("dictionary.txt"); // Save dictionary to file before exiting
                cout << "Exiting and saving dictionary.\n";
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

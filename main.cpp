#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Evaluate the given lines of code
void evaluate(const vector<string>& lines) {
  // Create a string with the code
  string code = "";
  for (const string& l : lines) {
    code += l + "\n";
  }

  // Create a temporary file with the code
  string filename = "tmp.cpp";
  ofstream file(filename);
  file << code;
  file.close();

  // Compile the code
  string compile_command = "g++ -o tmp " + filename;
  int result = system(compile_command.c_str());
  if (result != 0) {
    cerr << "Error: Compilation failed" << endl;
    return;
  }

  // Execute the code
  string execute_command = "./tmp";
  result = system(execute_command.c_str());
  if (result != 0) {
    cerr << "Error: Execution failed" << endl;
  }
}

// The main REPL loop
void repl() {
  string line;
  vector<string> lines;

  // Read
  cout << "C++> ";
  while (getline(cin, line)) {
    if (line == "") {
      break;
    }
    if (line == "help") {
      // Show the help menu
      cout << "Available commands:" << endl;
      cout << "  help: show this help menu" << endl;
      cout << "  evaluate: evaluate the input code" << endl;
      cout << "  exit: exit the REPL" << endl;
      cout << endl;
      cout << "C++> ";
      continue;
    }
    if (line == "exit") {
      // Exit the REPL
      return;
    }
    if (line == "evaluate") {
      // Evaluate the code
      evaluate(lines);

      // Clear the input buffer
      lines.clear();

      cout << "C++> ";
      continue;
    }
    lines.push_back(line);
    cout <<"";
    //cout << "  added line to input" << endl;
    cout << "C++> ";
  }

  // Check for #include directives
  vector<string> includes;
  for (const string& l : lines) {
    if (l.find("#include") != string::npos) {
      size_t start = l.find('"') + 1;
      size_t end = l.find('"', start);
      string include = l.substr(start, end - start);
      includes.push_back(include);
    }
  }

  // Read header files
  vector<string> header_lines;
  for (const string& include : includes) {
    ifstream file(include);
    if (!file.is_open()) {
      cerr << "Error: Could not open file " << include << endl;
      continue;
    }
    string header_line;
    while (getline(file, header_line)) {
      header_lines.push_back(header_line);
    }
  }

  // Print
  cout << "Output:" << endl;
  for (const string& l : header_lines) {
    cout << l << endl;
  }
  for (const string& l : lines) {
    cout << l << endl;
  }

  // Loop
  repl();
}

int main() {
  repl();
  return 0;
}

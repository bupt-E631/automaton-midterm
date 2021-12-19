#pragma once
#include "RegularExpression.h"
#include<fstream>
#include<filesystem>
#include<iostream>
#include<stdexcept>
#include<exception>
namespace fs = std::filesystem;

void output(MinimizedDFA*);

void output_file(MinimizedDFA*);


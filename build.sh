#!/bin/bash

echo "🔨 Compiling Monopoly Game..."

# Compile
g++ -std=c++11 sources/*.cpp -o monopoly

if [ $? -eq 0 ]; then
  echo "✅ Compilation successful! Run it using: ./monopoly"
else
  echo "❌ Compilation failed."
fi
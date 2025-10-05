#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.13"
# dependencies = [
#     #"libclang==12.0.0",
#     "libclang==16.0.0",
# ]
# ///


import sys
import os

# Добавляем tools в Python path
tools_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, tools_dir)

from api_generator import main

if __name__ == "__main__":
    main.main()

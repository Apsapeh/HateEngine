# Copy to clipboard
import pyperclip
import sys

def main():
    # File path from arg
    #file_path = sys.argv[1]
    #text = open(file_path, 'r').read()
    text = pyperclip.paste()
    change_pairs = {}
    while True:
        key = input("Enter a key (or 'done' to finish): ")
        if key == 'done':
            break
        value = input("Enter a value for " + key + ": ")
        change_pairs[key] = value
    
    # Copy the modified text to clipboard
    modified_text = text
    for key, value in change_pairs.items():
        modified_text = modified_text.replace(key, value)
    pyperclip.copy(modified_text)
    print(modified_text)
    
if __name__ == "__main__":
    main()
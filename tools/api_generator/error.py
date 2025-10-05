def error(message: str):
    print(f"\033[31m✗ {message}\033[0m")
    exit(1)

def warn(message: str):
    print(f"⚠️ {message}")
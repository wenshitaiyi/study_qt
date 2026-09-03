import os
import shutil

def main():
    repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    src_dir = os.path.join(repo_root, "app", "data", "ScientificColourMaps8")
    dst_dir = os.path.join(repo_root, "app", "data", "colourmap")

    os.makedirs(dst_dir, exist_ok=True)
    count = 0

    for root, dirs, files in os.walk(src_dir):
        for f in files:
            if f.lower().endswith(".clr"):
                src_file = os.path.join(root, f)
                dst_file = os.path.join(dst_dir, f)
                shutil.copy2(src_file, dst_file)
                count += 1
                print(f"Extracted: {f}")

    print(f"Total extracted .clr files: {count} to {dst_dir}")

if __name__ == "__main__":
    main()

import subprocess

def apply_filters() -> None:
    make_command = "make"
    compiled_prog_name = "filter"

    images_dir = "./images/"
    
    parameters = {
        "g": "grayscaled",
        "r": "reflected",
        "b": "blurred",
        "e": "edges"
    }


    original_files = [
        "courtyard",
        "stadium",
        "tower",
        "yard"
    ]

    files_ext = ".bmp"
    
    # Removing old file
    subprocess.run(["rm", compiled_prog_name], stdout=subprocess.DEVNULL)
    
    # Compiling
    subprocess.run([make_command, compiled_prog_name],
                   stdout=subprocess.DEVNULL)

    # Running the filters
    for image in original_files:
        print(f'--- File: {image}{files_ext} ---')
        
        for p in parameters.keys():        
            print(f'Applying filter: {parameters[p]}... ', end="")
            subprocess.run(
                [f"./{compiled_prog_name}", f"-{p}", f"{images_dir}{image}{files_ext}", f"{images_dir}{image}_{parameters[p]}{files_ext}"])
            print("Done")   

        print()         


if __name__ == '__main__':
    apply_filters()
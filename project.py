import argparse
import os
import subprocess
import shutil

# Utils -----------------------------------------------------------------------

def run_command(command_args):
    subprocess.call(command_args, text=True)

# Build command ---------------------------------------------------------------

def find_cmake_app():
    cmake_app_name = "cmake"
    vcpkg_root = "vcpkg"

    for root, _, files in os.walk(vcpkg_root):
        if cmake_app_name in files:
            path_to_cmake = os.path.join(root, cmake_app_name)
            return os.path.abspath(path_to_cmake)
    
    return ""
    
def handle_build_command(build_folder, cmake_args):
    cmake = find_cmake_app()
    vcpkg_toolchain_path = "vcpkg/scripts/buildsystems/vcpkg.cmake"

    cmake_config =  [cmake, "-S", ".", "-B", build_folder]
    cmake_config.extend(cmake_args)
    cmake_config.extend([f"-DCMAKE_TOOLCHAIN_FILE={vcpkg_toolchain_path}"])
    
    run_command(cmake_config)

    cmake_build = [cmake, "--build", build_folder]
    run_command(cmake_build)

# -----------------------------------------------------------------------------

# Add command -----------------------------------------------------------------

def handle_add_command(packages_to_add):
    
    if os.name == "posix":
        path_to_vcpkg = "vcpkg/vcpkg"
    else:
        path_to_vcpkg = "vcpkg/vcpkg.exe"
    vcpkg_add_port = [path_to_vcpkg, "add", "port"]
    vcpkg_add_port.extend(packages_to_add)

    run_command(vcpkg_add_port)

# -----------------------------------------------------------------------------

# Install command -------------------------------------------------------------

def handle_install_command():
    
    if os.name == "posix":
        path_to_vcpkg = "vcpkg/vcpkg"
    else:
        path_to_vcpkg = "vcpkg/vcpkg.exe"
    vcpkg_install = [path_to_vcpkg, "install"]

    run_command(vcpkg_install)

# Init command ----------------------------------------------------------------

def handle_init_command():
    git_command = shutil.which("git")
    git_clone_command = [
        git_command, 
        "clone", 
        "https://github.com/Microsoft/vcpkg.git"
    ]
    run_command(git_clone_command)
    
    os.chdir("vcpkg")

    run_bootstrap_script = []
    if os.name == "posix":
        unix_sh_command = shutil.which("sh")
        run_bootstrap_script = [unix_sh_command, "bootstrap-vcpkg.sh"]
    else: #windows
        windows_call_command = shutil.which("call")
        run_bootstrap_script = [windows_call_command, "bootstrap-vcpkg.bat"]
    run_command(run_bootstrap_script)

    os.chdir("..")

    handle_install_command()


# -----------------------------------------------------------------------------

# main ------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        prog="project",
        description="Project manager for C++ projects"
    )

    subparsers = parser.add_subparsers(dest="command", required=True)
    
    parser_build = subparsers.add_parser("build")
    parser_build.add_argument("-o", "--build_output", default="build")
    parser_build.add_argument(
        "--cmake_args",
        nargs=argparse.REMAINDER,
        default=[],
        help="Parameter list to forward to cmake command. Must be the last parameter, if used!"
    )

    subparsers.add_parser("init")

    parser_add = subparsers.add_parser("add")
    parser_add.add_argument(
        "vcpkg_packages",
        nargs=argparse.REMAINDER,
        default=[],
        help="Packages list to forward to vcpkg install. Must be the last parameter, if used!"
    )

    subparsers.add_parser("install")
    
    args = parser.parse_args()

    if args.command == "build":
        handle_build_command(args.build_output, args.cmake_args)
    elif args.command == "init":
        handle_init_command()
    elif args.command == "add":
        handle_add_command(args.vcpkg_packages)
    elif args.command == "install":
        handle_install_command()
    

if __name__ == "__main__":
    main()

# -----------------------------------------------------------------------------


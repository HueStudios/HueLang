import sys
import os
import termcolor
import shutil
import functools

EXTENSIONS = {
    "executable": "",
    "shared": ".so"
}

CFLAGS = {
    "executable": {
        "release": "-march=native -O2 -g0 -Wall",
        "debug": "-march=native -O0 -g -Wall"
    },
    "shared": {
        "release": "-march=native -O2 -g0 -Wall -fpic",
        "debug": "-march=native -O0 -g -Wall -fpic" 
    }
}

LFLAGS = {
    "executable": {
        "release": "-O2 -g0 -L./bin",
        "debug": "-O0 -g -L./bin"
    },
    "shared": {
        "release": "-O2 -g0 -L./bin -shared",
        "debug": "-O0 -g -L./bin -shared"
    }
}


def build_binary(mode, name, kind, cflags, lflags):
    if os.path.isdir("obj/{}".format(name)):
        shutil.rmtree("obj/{}".format(name))
    for root, _, files in os.walk("src/{}".format(name)):
        source_root_elements = root.split(os.sep)
        object_root = os.path.join("obj", *source_root_elements[1:])
        os.makedirs(object_root)
        for file in files:
            if file[-3:] != "cpp":
                continue
            source_path = os.path.join(root, file)
            object_path = os.path.join(object_root, file.replace(".cpp", ".o"))
            termcolor.cprint("<.> Building {} to {}...".format(source_path,
                                                               object_path),
                             "magenta")
            ec = os.system("g++ {} {} -o {} -c {}".format(CFLAGS[kind][mode],
                                                          cflags,
                                                          object_path,
                                                          source_path))
            if not ec == 0:
                termcolor.cprint("<!> Unable to build {}".format(source_path),
                                 "red")
                exit(200)

    if not os.path.isdir("bin"):
        os.mkdir("bin")
    out_name = "bin/{}{}".format(name, EXTENSIONS[kind])
    if os.path.isfile("bin/{}".format(out_name)):
        os.remove("bin/{}".format(out_name))
    all_objects = []
    for root, _, files in os.walk("obj/{}".format(name)):
        for file in files:
            all_objects.append(os.path.join(root, file))

    termcolor.cprint("<.> Linking objects into binary {}...".format(
        out_name), "magenta")
    objs_line = functools.reduce(
        lambda x, y: "{} {}".format(x, y), all_objects)
    ec = os.system("g++ {} {} -o {} {}".format(LFLAGS[kind][mode],
                                               lflags,
                                               out_name,
                                               objs_line))
    if not ec == 0:
        termcolor.cprint("<!> Unable to build {}".format(source_path),
                         "red")
        exit(201)
    termcolor.cprint("<!> Done", "green")


if __name__ == "__main__":
    build_binary("debug", "libhuelang", "shared", "", "")
    build_binary("debug", "huelang", "executable", "", "-lhuelang -lboost_system -lreadline -ldl")

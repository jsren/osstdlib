#!/usr/bin/python
# (c) 2018 James Renwick
import os
import shutil

abidir = os.environ.get("ABI_DIR")

thisdir = os.path.dirname(__file__)
basedir = os.path.join(thisdir, "..")
includedir = os.path.join(thisdir, "include")
abidir = abidir if abidir else os.path.join(basedir, "osabi")


def get_files_by_ext(ext, path):
    """Returns list of files with the given extension in the given path"""
    return [file for file in os.listdir(path) \
        if file.endswith(ext)]

def get_public_headers(path):
    """Returns list of .hpp files in given path"""
    return [os.path.join(path, f) for f in get_files_by_ext(".hpp", path)]

def get_platform_header(path):
    platform = os.environ.get("OSSTDLIB_PLATFORM")
    if not platform:
        raise Exception("Required environment variable OSSTDLIB_PLATFORM not set or empty")
    file = os.path.join(abidir, platform, "platform.hpp")
    if not os.path.exists(file):
        raise Exception("Platform header '%s' does not exist"%(file))
    else:
        return file

if __name__ == "__main__":
    if not os.path.exists(includedir):
        os.mkdir(includedir)

    for header in get_public_headers(basedir) + get_public_headers(abidir):
        target = os.path.join(includedir, os.path.splitext(os.path.split(header)[1])[0])
        if not os.path.exists(target) or os.stat(header).st_mtime - os.stat(target).st_mtime > 1:
            shutil.copy2(header, target)

    target = "__platform-impl"
    shutil.copy2(get_platform_header(basedir), os.path.join(includedir, target))

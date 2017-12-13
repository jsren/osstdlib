#!/usr/bin/python
import os
import shutil

thisdir = os.path.dirname(__file__)
includedir = os.path.join(thisdir, "include")


def get_files_by_ext(ext, path):
    return [file for file in os.listdir(path) \
        if file.endswith(ext)]

def get_public_headers(path):
    return [os.path.join(path, f) for f in get_files_by_ext(".hpp", path)]

def get_platform_header(path):
    platform = os.environ.get("OSSTDLIB_PLATFORM")
    if not platform:
        raise Exception("Required environment variable OSSTDLIB_PLATFORM not set or empty")
    file = os.path.join(path, "platform", "platform-" + platform + ".hpp")
    if not os.path.exists(file):
        raise Exception("Platform header '%s' does not exist"%(file))
    else:
        return file

def get_platform_cpp(path):
    platform = os.environ.get("OSSTDLIB_PLATFORM")
    if not platform:
        raise Exception("Required environment variable OSSTDLIB_PLATFORM not set or empty")
    file = os.path.join(path, "platform", "platform-" + platform + ".cpp")
    if not os.path.exists(file):
        raise Exception("Platform cpp '%s' does not exist"%(file))
    else:
        return file


if __name__ == "__main__":
    if not os.path.exists(includedir):
        os.mkdir(includedir)

    for header in get_public_headers("."):
        target = os.path.splitext(os.path.split(header)[1])[0]
        shutil.copy(header, os.path.join(includedir, target))

    target = "__platform-impl"
    shutil.copy(get_platform_header("."), os.path.join(includedir, target))

    target = "__platform.cpp"
    shutil.copy(get_platform_cpp("."), os.path.join(includedir, target))

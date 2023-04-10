#!/usr/bin/env python3
# This code was taken from the official Flipper Zero firmware repository:
# https://github.com/flipperdevices/flipperzero-firmware
# And distributes under GNU GPL-3.0 license
import io
import os
import argparse
from PIL import Image, ImageOps

ICONS_SUPPORTED_FORMATS = ["png"]

ICONS_TEMPLATE_H_HEADER = """#pragma once

#include <icon.h>

"""
ICONS_TEMPLATE_H_ICON_NAME = "extern const Icon {name};\n"

ICONS_TEMPLATE_C_HEADER = """#include "{assets_filename}.h"

"""
ICONS_TEMPLATE_C_FRAME = "const uint8_t {name}[] = {data};\n"
ICONS_TEMPLATE_C_DATA = "const uint8_t* const {name}[] = {data};\n"
ICONS_TEMPLATE_C_ICONS = "const Icon {name} = {{.width={width},.height={height},.frame_count={frame_count},.frame_rate={frame_rate},.frames=_{name}}};\n"


class CImage:
    def __init__(self, width: int, height: int, data: bytes):
        self.width = width
        self.height = height
        self.data = data

    def write(self, filename):
        with open(filename, "wb") as file:
            file.write(self.data)

    def data_as_carray(self):
        return (
            "{" + "".join("0x{:02x},".format(img_byte) for img_byte in self.data) + "}"
        )


def is_file_an_icon(filename):
    extension = filename.lower().split(".")[-1]
    return extension in ICONS_SUPPORTED_FORMATS


def png2xbm(file):
    with Image.open(file) as im:
        with io.BytesIO() as output:
            bw = im.convert("1")
            bw = ImageOps.invert(bw)
            bw.save(output, format="XBM")
            return output.getvalue()


def file2image(file):
    output = png2xbm(file)
    assert output
    f = io.StringIO(output.decode().strip())
    width = int(f.readline().strip().split(" ")[2])
    height = int(f.readline().strip().split(" ")[2])
    data = f.read().strip().replace("\n", "").replace(" ", "").split("=")[1][:-1]
    data_str = data[1:-1].replace(",", " ").replace("0x", "")
    data_bin = bytearray.fromhex(data_str)
    data = b"\x00" + data_bin
    return CImage(width, height, data)


def _icon2header(file):
    image = file2image(file)
    return image.width, image.height, image.data_as_carray()


def _iconIsSupported(filename):
    extension = filename.lower().split(".")[-1]
    return extension in ICONS_SUPPORTED_FORMATS


def icons(args):
    print("ICON: Converting icons")
    icons_c = open(
        os.path.join(args.output_directory, f"{args.filename}.c"),
        "w",
        newline="\n",
    )
    icons_c.write(ICONS_TEMPLATE_C_HEADER.format(assets_filename=args.filename))
    icons = []
    # Traverse icons tree, append image data to source file
    for dirpath, dirnames, filenames in os.walk(args.input_directory):
        print(f"ICON: Processing directory {dirpath}")
        dirnames.sort()
        filenames.sort()
        if not filenames:
            continue
        if "frame_rate" in filenames:
            print(f"ICON: Folder contains animation")
            icon_name = "A_" + os.path.split(dirpath)[1].replace("-", "_")
            width = height = None
            frame_count = 0
            frame_rate = 0
            frame_names = []
            for filename in sorted(filenames):
                fullfilename = os.path.join(dirpath, filename)
                if filename == "frame_rate":
                    frame_rate = int(open(fullfilename, "r").read().strip())
                    continue
                elif not _iconIsSupported(filename):
                    continue
                print(f"ICON: Processing animation frame {filename}")
                temp_width, temp_height, data = _icon2header(fullfilename)
                if width is None:
                    width = temp_width
                if height is None:
                    height = temp_height
                assert width == temp_width
                assert height == temp_height
                frame_name = f"_{icon_name}_{frame_count}"
                frame_names.append(frame_name)
                icons_c.write(ICONS_TEMPLATE_C_FRAME.format(name=frame_name, data=data))
                frame_count += 1
            assert frame_rate > 0
            assert frame_count > 0
            icons_c.write(
                ICONS_TEMPLATE_C_DATA.format(
                    name=f"_{icon_name}", data=f'{{{",".join(frame_names)}}}'
                )
            )
            icons_c.write("\n")
            icons.append((icon_name, width, height, frame_rate, frame_count))
        else:
            # process icons
            for filename in filenames:
                if not _iconIsSupported(filename):
                    continue
                print(f"ICON: Processing icon {filename}")
                icon_name = "I_" + "_".join(filename.split(".")[:-1]).replace("-", "_")
                fullfilename = os.path.join(dirpath, filename)
                width, height, data = _icon2header(fullfilename)
                frame_name = f"_{icon_name}_0"
                icons_c.write(ICONS_TEMPLATE_C_FRAME.format(name=frame_name, data=data))
                icons_c.write(
                    ICONS_TEMPLATE_C_DATA.format(
                        name=f"_{icon_name}", data=f"{{{frame_name}}}"
                    )
                )
                icons_c.write("\n")
                icons.append((icon_name, width, height, 0, 1))
    # Create array of images:
    print(f"ICON: Finalizing source file")
    for name, width, height, frame_rate, frame_count in icons:
        icons_c.write(
            ICONS_TEMPLATE_C_ICONS.format(
                name=name,
                width=width,
                height=height,
                frame_rate=frame_rate,
                frame_count=frame_count,
            )
        )
    icons_c.write("\n")
    icons_c.close()

    # Create Public Header
    print(f"ICON: Creating header")
    icons_h = open(
        os.path.join(args.output_directory, f"{args.filename}.h"),
        "w",
        newline="\n",
    )
    icons_h.write(ICONS_TEMPLATE_H_HEADER)
    for name, width, height, frame_rate, frame_count in icons:
        icons_h.write(ICONS_TEMPLATE_H_ICON_NAME.format(name=name))
    icons_h.close()
    print(f"ICON: Done")
    return 0


def main():
    parser = argparse.ArgumentParser(description="Converting .png icons to C array")
    parser.add_argument("input_directory", help="Source directory")
    parser.add_argument("output_directory", help="Output directory")
    parser.add_argument(
        "--filename",
        help="Base filename for file with icon data",
        required=False,
        default="assets_icons",
    )
    args = parser.parse_args()
    icons(args)


if __name__ == "__main__":
    main()

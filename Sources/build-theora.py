#!/usr/bin/env python3
"""Build Xiph 1.1.1's portable decoder, without Cairo/encoder dependencies."""
import os
from pathlib import Path
import subprocess
sdk = Path(os.environ['PORT_SDK']).resolve()
source = Path(os.environ['THEORA_SOURCE']).resolve()
objects = Path('theora-objects').resolve()
objects.mkdir(exist_ok=True)
# Correct the old 1.1.1 decoder's transposed qi/qti indices before compiling.
decode = source/'lib/decode.c'
text = decode.read_text()
text = text.replace('dequant_tables[qti][pli][qi]', 'dequant_tables[qi][pli][qti]')
decode.write_text(text)
files = 'apiwrapper bitpack decapiwrapper decinfo decode dequant fragment huffdec idct info internal quant state'.split()
for name in files:
    subprocess.run([str(sdk/'usr/bin/aarch64-linux-gnu-gcc-13'), '--sysroot='+str(sdk),
                    '-O3', '-mcpu=cortex-a55', '-fno-strict-aliasing',
                    '-I'+str(source/'include'), '-I'+str(sdk/'usr/include'),
                    '-c', str(source/'lib'/name)+'.c', '-o', str(objects/name)+'.o'], check=True)
archive = sdk/'usr/lib/aarch64-linux-gnu/libtheoradec.a'
if archive.exists(): archive.unlink()
subprocess.run([str(sdk/'usr/bin/aarch64-linux-gnu-ar'), 'rcs', str(archive)]+
               [str(objects/name)+'.o' for name in files], check=True)

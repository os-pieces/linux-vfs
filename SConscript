from building import *
import os

cwd = GetCurrentDir()

inc = [cwd + '/include']

src = Glob('src/*.c')
src += Glob('src/api/*.c')
src += Glob('src/libfs/*.c')
src += Glob('src/private/*.c')
src += Glob('src/uio/*.c')

group = DefineGroup('vfs', src, depend=['CONFIG_VFS'], CPPPATH=inc)

Return('group')

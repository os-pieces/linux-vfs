from building import *
import os

cwd = GetCurrentDir()

inc = [cwd + '/include']

src = Glob('src/*.c')
src += Glob('src/api/*.c')
src += Glob('src/libfs/*.c')
src += Glob('src/private/*.c')

group = DefineGroup('vfs', src, depend=[''], CPPPATH=inc)

list = os.listdir(cwd)
for item in list:
    if os.path.isfile(os.path.join(cwd, item, 'SConscript')):
        group = group + SConscript(os.path.join(item, 'SConscript'))

Return('group')

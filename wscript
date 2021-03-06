import os

srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def system(cmd):
  return os.popen(cmd).read().strip().split()

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  conf.env['CXXFLAGS_ICU'] = system('icu-config --cxxflags')
  conf.env['LINKFLAGS_ICU'] = system('icu-config --ldflags-libsonly')

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'charset-detector'
  obj.source = 'charset-detector.cc'
  obj.uselib = 'ICU'

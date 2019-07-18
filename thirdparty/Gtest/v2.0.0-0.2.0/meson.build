# Build gtest

gtest_inc = include_directories('.')
gtest_src = [
	'gtest_main.cc',
	'gtest-all.cc']
# 'magic.cc'

gtest_lib = static_library('gtest', 
	gtest_src,
	install : true)
gtest_dep = declare_dependency(link_with : gtest_lib,
  include_directories : gtest_inc)
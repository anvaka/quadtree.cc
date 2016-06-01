{
# These dependencies are shared between test and static library
  'dependencies': [
    "<!(node -e \"console.log(require.resolve('random.cc/gyp/random.cc.gyp') + ':*')\")",
  ],
# Turns out if library depends on quadtree, it also needs to know where to find
# headers for random. That's somewhat inconvenient...
  'export_dependent_settings': [
    "<!(node -e \"console.log(require.resolve('random.cc/gyp/random.cc.gyp') + ':*')\")",
  ],
}

{
# These dependencies are shared between test and static library
  'dependencies': [
    "<!(node -e \"console.log(require.resolve('random.cc/gyp/random.cc.gyp') + ':*')\")",
  ],
}

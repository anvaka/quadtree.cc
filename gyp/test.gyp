{
  'includes': [
    './common.gypi'
  ],
  'targets': [
    {
      'target_name': 'test',
      'type': 'executable',
      'dependencies': [
        './quadtree.cc.gyp:*',
        "<!(node -e \"console.log(require.resolve('catch.cc/gyp/catch.cc.gyp') + ':*')\")",
      ],
      'includes': [
        './deps.gypi'
      ],
      'sources': [
        '../test/main.cc'
      ],
    }
  ]
}

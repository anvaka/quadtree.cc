{
  'includes': [
    './common.gypi'
  ],
  'targets': [{
      'target_name': 'quadtree',
      'type': 'static_library',
      'sources': [
        '../src/quadtree.cc',
        '../include/quadtree.cc/quadtree.h',
        '../include/quadtree.cc/primitives.h',
      ],
      'include_dirs': [
          '../include'
      ],
      'includes': [
        './deps.gypi'
      ],
      'all_dependent_settings': {
        'include_dirs': [
          '../include'
        ],
      },
    },
  ]
}

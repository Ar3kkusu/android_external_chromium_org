{
  'TOOLS': ['newlib', 'glibc', 'pnacl', 'win'],
  'SEARCH': [
    '.'
  ],
  'TARGETS': [
    {
      'NAME' : 'sdk_util',
      'TYPE' : 'lib',
      'SOURCES' : [
        'thread_pool.cc'
      ]
    }
  ],
  'HEADERS': [
    {
      'FILES': [
        'auto_lock.h',
        'macros.h',
        'ref_object.h',
        'thread_pool.h',
        'thread_safe_queue.h'
      ],
      'DEST': 'include/sdk_util',
    }
  ],
  'DEST': 'src',
  'NAME': 'sdk_util',
}
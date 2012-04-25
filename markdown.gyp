{
  'targets': [
    {
      'target_name': 'markdown',
      'type': 'executable',
      'msvs_guid': '5B618686-D628-45A8-9E12-B8C2FAA08C71',
      'sources': [
        'markdown.c',
        'markdown_parser.c',
        'markdown_output.c',
        'markdown_lib.c',
        'GLibFacade.c',
      ],
      'actions': [
        {
          'action_name': 'leg',
          'inputs': [
            'markdown_parser.leg',
          ],
          'outputs': [
            'markdown_parser.c',
          ],
          'action': ['peg/leg', '-o', '<@(_outputs)', '<@(_inputs)'],
        },
      ],
      'conditions': [
        ['OS=="win"', {
          'include_dirs': [
            'win',
          ],
          'sources': [
            'win/getopt.c',
            'win/getopt_long.c',
          ],
        }],
      ],
    },
  ],

  'target_defaults': {
    'configurations': {
      'Debug': {
        'defines': [
          'DEBUG',
        ],
      },
      'Release': {
        'defines': [
          'NDEBUG',
        ],
      },
    },
  },

  # define default project settings
  'conditions': [
    ['OS=="win"', {
      'target_defaults': {
        'defines': [
          'WIN32',
          '__WIN32',
          '_WINDOWS',
        ],
        'msvs_settings': {
          'VCLinkerTool': {
            'GenerateDebugInformation': 'true',
            # SubSystem values:
            #   0 == not set
            #   1 == /SUBSYSTEM:CONSOLE
            #   2 == /SUBSYSTEM:WINDOWS
            'SubSystem': '1',
          },
        },
      },
    }],
  ],
}

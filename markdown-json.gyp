{
  'targets': [
    {
      'target_name': 'markdown-json',
      'type': 'executable',
      'msvs_guid': '5B618686-D628-45A8-9E12-B8C2FAA08C71',
      'sources': [
        'glib.h',
        'markdown.c',
        'markdown_parser.c',
        'markdown_lib.h',
        'markdown_lib.c',
        'GLibFacade.h',
        'GLibFacade.c',
        'markdown_json.h',
        'markdown_json.c',
        'markdown_peg.h',
        'parsing_functions.h',
        'utility_functions.h',
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
          'VCCLCompilerTool': {
            'Optimization': '0'
          }
        },
      },
    }],
  ],
}

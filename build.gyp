{
	'variables': {
		'conditions': [
			['OS == "win"', {
				'component%': 'static_library',
			}],
			['OS == "mac"', {
				'component%': 'shared_library',
			}],
			['OS != "win" and OS != "mac"', {
				'component%': 'shared_library',
			}],
		],
	},
	'target_defaults': {
		"include_dirs" : [ 
			'lib/CppMetadata/include',
			'include'
		],
		'cflags': ['-DDUK_USE_CPP_EXCEPTIONS=1'],
		'msbuild_settings': {
			'ClCompile': {
				#'WarningLevel': 'Level4', # /W4
				'PreprocessorDefinitions': [
					'_WIN32_WINNT=0x0600', # Windows Vista SP2 or later
					'WIN32_LEAN_AND_MEAN',
					'NOMINMAX',
					'DUK_USE_CPP_EXCEPTIONS=1'
				],
			},
		},
		'default_configuration': 'Release_x64',
        'configurations':
        {
			'Debug': {
				'defines': ['DEBUG=1'],
				'cflags': ['-g', '-O0', '-std=c++11'],
				'msbuild_settings': {
					'ClCompile': {
						'Optimization': 'Disabled', # /Od
						'conditions': [
							['OS == "win" and component == "shared_library"', {
								'RuntimeLibrary': 'MultiThreadedDebugDLL', # /MDd
							}, {
								'RuntimeLibrary': 'MultiThreadedDebug', # /MTd
							}],
						],
					},
					'Link': {
					#'LinkIncremental': 'true', # /INCREMENTAL
					},
				},
			}, # Debug
			'Release': {
				'defines': ['NDEBUG=1'],
				'cflags': ['-O3', '-std=c++11'],
				'msbuild_settings': {
					'ClCompile': {
						'Optimization': 'MaxSpeed', # /O2
						'InlineFunctionExpansion': 'AnySuitable', # /Ob2
						'conditions': [
							['OS == "win" and component == "shared_library"', {
								'RuntimeLibrary': 'MultiThreadedDLL', # /MD
							}, {
								'RuntimeLibrary': 'MultiThreaded', # /MT
							}],
						],
					},
					'Link': {
						#'LinkIncremental': 'false', # /INCREMENTAL:NO
						'OptimizeReferences': 'true', # /OPT:REF
					},
				},
			}, # Release
            'Debug_x64': {
                'inherit_from': ['Debug'],
                'msvs_configuration_platform': 'x64'
            },
            'Release_x64': {
                'inherit_from': ['Release'],
                'msvs_configuration_platform': 'x64'
            },
        },
	},
	
	'targets': [
		{
			'target_name': 'daemon-test',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				"CPP-SystemRT-lib"
			],
			'sources': [
				'test/daemon_test.cpp'
			],
		},
		{
			'target_name': 'thread-test',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				"CPP-SystemRT-lib"
			],
			'sources': [
				'test/thread_test.cpp'
			],
		},
		{
			'target_name': 'socket-client-test',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib', '-lws2_32.lib', '-luser32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				"CPP-SystemRT-lib"
			],
			'sources': [
				'test/socket_client_test.cpp'
			],
		},
		{
			'target_name': 'socket-server-test',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib', '-lws2_32.lib', '-luser32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				"CPP-SystemRT-lib"
			],
			'sources': [
				'test/socket_server_test.cpp'
			],
		},
		{
			'target_name': 'namedpipe-test',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				"CPP-SystemRT-lib"
			],
			'sources': [
				'test/namedpipe_test.cpp'
			],
		},
		{
			'target_name': 'configfile-test',
			'type': 'executable',
			'conditions': [
				['OS == "win"', {
					'libraries': ['-ladvapi32.lib'],
				}],
				['OS == "linux"', {
					'libraries': ['-lpthread'],
				}]
			],
			"dependencies": [
				"CPP-SystemRT-lib"
			],
			'sources': [
				'test/configfile_test.cpp'
			],
		},
		{
			'target_name': 'CPP-SystemRT-lib',
			'type': 'static_library',
			'sources': [
				'src/<(OS)/daemon.cpp',
				'src/<(OS)/log.cpp',
				'src/<(OS)/namedpipe.cpp',
				'src/<(OS)/error.cpp',
				'src/<(OS)/sharedlib.cpp',
				'src/log-common.cpp',
				'src/filestream.cpp',
				'src/thread.cpp',
				'src/socket.cpp',
				'src/configfile.cpp',
			],
		},
	],
}
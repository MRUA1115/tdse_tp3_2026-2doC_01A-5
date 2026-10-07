{
  "graph": {
    "cells": [
      {
        "position": {
          "x": 0,
          "y": 0
        },
        "size": {
          "height": 10,
          "width": 10
        },
        "type": "Statechart",
        "id": "78749915-0da0-40a2-862f-9e8d94c7c68e",
        "attrs": {
          "name": {
            "text": "task_display Export"
          },
          "specification": {
            "text": "interface:\r\n  in event EV_DSP_UPDATE\r\n\r\ninternal:\r\n  var row : integer\r\n  var column : integer\r\n  var charPending : boolean\r\n  const ROWS : integer = 2\r\n\r\n"
          }
        },
        "z": 1
      },
      {
        "position": {
          "x": -42,
          "y": 196
        },
        "size": {
          "width": 78,
          "height": 60
        },
        "type": "State",
        "attrs": {
          "name": {
            "text": "ST_DSP_IDLE",
            "fontSize": 11
          }
        },
        "id": "36b8548a-74b1-4925-ae23-291c03231ee6",
        "z": 41
      },
      {
        "position": {
          "x": -11,
          "y": 143
        },
        "size": {
          "height": 15,
          "width": 15
        },
        "type": "Entry",
        "entryKind": "Initial",
        "attrs": {},
        "id": "a4d257fe-72cb-4334-9a1f-b5a8b4f886fb",
        "z": 43,
        "embeds": [
          "f70f632c-e54d-4002-9ec5-7baec620de10"
        ]
      },
      {
        "type": "NodeLabel",
        "label": true,
        "size": {
          "width": 15,
          "height": 15
        },
        "position": {
          "x": -11,
          "y": 158
        },
        "attrs": {
          "label": {
            "refX": "50%",
            "textAnchor": "middle",
            "refY": "50%",
            "textVerticalAnchor": "middle"
          }
        },
        "id": "f70f632c-e54d-4002-9ec5-7baec620de10",
        "z": 44,
        "parent": "a4d257fe-72cb-4334-9a1f-b5a8b4f886fb"
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "a4d257fe-72cb-4334-9a1f-b5a8b4f886fb"
        },
        "target": {
          "id": "36b8548a-74b1-4925-ae23-291c03231ee6"
        },
        "connector": {
          "name": "rounded"
        },
        "labels": [
          {
            "attrs": {},
            "position": {}
          },
          {
            "attrs": {
              "label": {
                "text": "1"
              }
            }
          },
          {
            "attrs": {}
          },
          {
            "attrs": {}
          }
        ],
        "router": {
          "name": "orthogonal",
          "args": {
            "padding": 8
          }
        },
        "id": "9b5ce41b-0116-46a8-a603-ea4119c6e5d7",
        "z": 45
      },
      {
        "position": {
          "x": 198,
          "y": 310
        },
        "size": {
          "width": 117,
          "height": 60
        },
        "type": "State",
        "attrs": {
          "name": {
            "text": "ST_DSP_WRITE_CHAR",
            "fontSize": 11
          }
        },
        "id": "db372094-acc6-467b-b44d-495606208445",
        "z": 48,
        "embeds": [
          "dccf1aba-693d-4f31-8a18-a2b927cb4532"
        ]
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "db372094-acc6-467b-b44d-495606208445",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "0%",
              "dy": "30%",
              "rotate": true
            }
          },
          "priority": true
        },
        "target": {
          "id": "36b8548a-74b1-4925-ae23-291c03231ee6",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "48.718%",
              "dy": "88.333%",
              "rotate": true
            }
          },
          "priority": true
        },
        "connector": {
          "name": "rounded"
        },
        "labels": [
          {
            "attrs": {
              "text": {
                "text": "[!charPending && row >= ROWS-1]"
              }
            },
            "position": {
              "distance": 0.38050517114969845,
              "offset": 13,
              "angle": 0
            }
          },
          {
            "attrs": {
              "label": {
                "text": "1"
              }
            }
          },
          {
            "attrs": {}
          },
          {
            "attrs": {}
          }
        ],
        "id": "a3934c3a-5a96-4820-90d2-c8d06f291398",
        "z": 49,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "db372094-acc6-467b-b44d-495606208445",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "18.803%",
              "dy": "96.667%",
              "rotate": true
            }
          },
          "priority": true
        },
        "target": {
          "id": "db372094-acc6-467b-b44d-495606208445",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "0.855%",
              "dy": "66.667%",
              "rotate": true
            }
          },
          "priority": true
        },
        "connector": {
          "name": "rounded"
        },
        "labels": [
          {
            "attrs": {
              "text": {
                "text": "[charPending] / column++"
              }
            },
            "position": {
              "distance": 0.34157636630748234,
              "offset": -11.252685546875,
              "angle": 0
            }
          },
          {
            "attrs": {
              "label": {
                "text": "2"
              }
            }
          },
          {
            "attrs": {}
          },
          {
            "attrs": {}
          }
        ],
        "id": "dccf1aba-693d-4f31-8a18-a2b927cb4532",
        "z": 51,
        "router": {
          "name": "orthogonal"
        },
        "vertices": [
          {
            "x": 220,
            "y": 394
          },
          {
            "x": 173,
            "y": 350
          }
        ],
        "parent": "db372094-acc6-467b-b44d-495606208445"
      },
      {
        "position": {
          "x": 542,
          "y": 197
        },
        "size": {
          "width": 129,
          "height": 60
        },
        "type": "State",
        "attrs": {
          "name": {
            "text": "ST_DSP_SET_POSITION",
            "fontSize": 11
          }
        },
        "id": "faef1b08-f89f-4129-ae19-25e4d4d2c7c1",
        "z": 52
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "db372094-acc6-467b-b44d-495606208445",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "100%",
              "dy": "66.667%",
              "rotate": true
            }
          },
          "priority": true
        },
        "target": {
          "id": "faef1b08-f89f-4129-ae19-25e4d4d2c7c1",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "65.116%",
              "dy": "96.667%",
              "rotate": true
            }
          },
          "priority": true
        },
        "connector": {
          "name": "rounded"
        },
        "labels": [
          {
            "attrs": {
              "text": {
                "text": "[!charPending && row < ROWS-1] / row++; column = 0\r\n"
              }
            },
            "position": {
              "distance": 0.3957026391680687,
              "offset": 19,
              "angle": 0
            }
          },
          {
            "attrs": {
              "label": {
                "text": "3"
              }
            }
          },
          {
            "attrs": {}
          },
          {
            "attrs": {}
          }
        ],
        "id": "7b12c895-576e-4989-a9f9-0c695b33722d",
        "z": 53,
        "router": {
          "name": "orthogonal"
        },
        "vertices": [
          {
            "x": 626,
            "y": 350
          }
        ]
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "36b8548a-74b1-4925-ae23-291c03231ee6"
        },
        "target": {
          "id": "faef1b08-f89f-4129-ae19-25e4d4d2c7c1",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "1.55%",
              "dy": "50%",
              "rotate": true
            }
          },
          "priority": true
        },
        "connector": {
          "name": "rounded"
        },
        "labels": [
          {
            "attrs": {
              "text": {
                "text": "EV_DSP_UPDATE / row = 0; column = 0\r\n"
              }
            },
            "position": {
              "distance": 0.493660304857337,
              "offset": -1,
              "angle": 0
            }
          },
          {
            "attrs": {
              "label": {
                "text": "1"
              }
            }
          },
          {
            "attrs": {}
          },
          {
            "attrs": {}
          }
        ],
        "id": "5336a84b-1ba4-4671-aa24-2e08a84ef110",
        "z": 53,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "faef1b08-f89f-4129-ae19-25e4d4d2c7c1",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "34.109%",
              "dy": "93.333%",
              "rotate": true
            }
          },
          "priority": true
        },
        "target": {
          "id": "db372094-acc6-467b-b44d-495606208445",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "99.145%",
              "dy": "31.667%",
              "rotate": true
            }
          },
          "priority": true
        },
        "connector": {
          "name": "rounded"
        },
        "labels": [
          {
            "attrs": {},
            "position": {}
          },
          {
            "attrs": {
              "label": {
                "text": "1"
              }
            }
          },
          {
            "attrs": {}
          },
          {
            "attrs": {}
          }
        ],
        "id": "efdab45a-e0df-45d6-9cd3-3a031e924a05",
        "z": 53,
        "router": {
          "name": "orthogonal"
        },
        "vertices": [
          {
            "x": 586,
            "y": 329
          }
        ]
      }
    ]
  },
  "genModel": {
    "generator": {
      "type": "create::c",
      "features": {
        "Outlet": {
          "targetProject": "",
          "targetFolder": "",
          "libraryTargetFolder": "",
          "skipLibraryFiles": "",
          "apiTargetFolder": ""
        },
        "LicenseHeader": {
          "licenseText": ""
        },
        "FunctionInlining": {
          "inlineReactions": false,
          "inlineEntryActions": false,
          "inlineExitActions": false,
          "inlineEnterSequences": false,
          "inlineExitSequences": false,
          "inlineChoices": false,
          "inlineEnterRegion": false,
          "inlineExitRegion": false,
          "inlineEntries": false
        },
        "OutEventAPI": {
          "observables": false,
          "getters": false
        },
        "IdentifierSettings": {
          "moduleName": "MyFirstStatechart",
          "statemachinePrefix": "myFirstStatechart",
          "separator": "_",
          "headerFilenameExtension": "h",
          "sourceFilenameExtension": "c"
        },
        "Tracing": {
          "enterState": false,
          "exitState": false,
          "generic": false
        },
        "Includes": {
          "useRelativePaths": false,
          "generateAllSpecifiedIncludes": false
        },
        "GeneratorOptions": {
          "userAllocatedQueue": false,
          "metaSource": false
        },
        "GeneralFeatures": {
          "timerService": false,
          "timerServiceTimeType": ""
        },
        "Debug": {
          "dumpSexec": false
        }
      }
    }
  }
}
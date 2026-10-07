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
        "id": "00ffb6d1-d225-4bc0-8b73-7df9987f57b7",
        "attrs": {
          "name": {
            "text": "system_setup_menu Export"
          },
          "specification": {
            "text": "@EventDriven\n@SuperSteps(no)\n\ninterface:\n    in event EV_BTN_ENTER\n    in event EV_BTN_NEXT\n    in event EV_BTN_ESCAPE\n\n    var motor_idx : integer = 0\n    var param_idx : integer = 0\n    var val_idx : integer = 0"
          }
        },
        "z": 1
      },
      {
        "position": {
          "x": -147,
          "y": -104
        },
        "size": {
          "height": 60,
          "width": 255
        },
        "type": "State",
        "attrs": {
          "name": {
            "text": "ST_MENU_1",
            "fontSize": 11
          },
          "specification": {
            "text": "EV_BTN_NEXT /motor_idx = (motor_idx + 1) % 2"
          }
        },
        "id": "7b8af555-248f-4c5f-9d32-d6cf8769c571",
        "z": 144
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "7b8af555-248f-4c5f-9d32-d6cf8769c571"
        },
        "target": {
          "id": "4206bce8-f05f-4472-b68e-658e0002a989",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "55%",
              "dy": "28.333%",
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
                "text": "EV_BTN_ENTER/\nparam_idx = 0"
              }
            },
            "position": {
              "distance": 0.561836055067719,
              "offset": -42,
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
        "id": "a99eef01-319b-4cb8-a699-21104fd8bc19",
        "z": 145,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "4206bce8-f05f-4472-b68e-658e0002a989"
        },
        "target": {
          "id": "7b8af555-248f-4c5f-9d32-d6cf8769c571",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "9.804%",
              "dy": "60%",
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
                "text": "EV_BTN_ESCAPE"
              }
            },
            "position": {
              "offset": 38,
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
        "id": "073cce94-5bcf-4d66-8132-19ec1f752991",
        "z": 146,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "position": {
          "x": -145,
          "y": 79
        },
        "size": {
          "height": 60,
          "width": 260
        },
        "type": "State",
        "attrs": {
          "name": {
            "text": "ST_MENU_2",
            "fontSize": 11
          },
          "specification": {
            "text": "EV_BTN_NEXT / param_idx = (param_idx + 1) % 3"
          }
        },
        "id": "4206bce8-f05f-4472-b68e-658e0002a989",
        "z": 158
      },
      {
        "position": {
          "x": 816,
          "y": 99
        },
        "size": {
          "width": 15,
          "height": 15
        },
        "type": "Choice",
        "attrs": {},
        "id": "b05c30d6-95c2-4f94-bfe5-f028ad7cce92",
        "z": 179
      },
      {
        "position": {
          "x": 312,
          "y": 97
        },
        "size": {
          "height": 65,
          "width": 231
        },
        "type": "State",
        "attrs": {
          "name": {
            "text": "ST_MENU_3",
            "fontSize": 11
          }
        },
        "id": "3b18f78b-069b-4b5b-9225-5d665eee862f",
        "z": 187
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "4206bce8-f05f-4472-b68e-658e0002a989"
        },
        "target": {
          "id": "3b18f78b-069b-4b5b-9225-5d665eee862f",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "74.026%",
              "dy": "3.077%",
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
                "text": "EV_BTN_ENTER"
              }
            },
            "position": {
              "distance": 0.4701213961333231,
              "offset": -36,
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
        "id": "b75928ee-d144-4622-b301-4e40acbfc596",
        "z": 188,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "3b18f78b-069b-4b5b-9225-5d665eee862f",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "2.165%",
              "dy": "61.538%",
              "rotate": true
            }
          },
          "priority": true
        },
        "target": {
          "id": "4206bce8-f05f-4472-b68e-658e0002a989",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "61.538%",
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
                "text": "EV_BTN_ESCAPE"
              }
            },
            "position": {
              "distance": 0.5196078431372549,
              "offset": -40,
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
        "id": "35e7de29-c0ba-4a41-a864-c43b4d14c4bc",
        "z": 188,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "3b18f78b-069b-4b5b-9225-5d665eee862f"
        },
        "target": {
          "id": "b05c30d6-95c2-4f94-bfe5-f028ad7cce92"
        },
        "connector": {
          "name": "rounded"
        },
        "labels": [
          {
            "attrs": {
              "text": {
                "text": "EV_BTN_NEXT"
              }
            },
            "position": {}
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
        "id": "27225f87-cf0f-4556-b610-36554aef51ff",
        "z": 188,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "b05c30d6-95c2-4f94-bfe5-f028ad7cce92"
        },
        "target": {
          "id": "3b18f78b-069b-4b5b-9225-5d665eee862f",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "70.996%",
              "dy": "89.231%",
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
                "text": "[param_idx == 1]/ val_idx = (val_idx + 1) % 10"
              }
            },
            "position": {}
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
        "id": "73258929-5a34-4af7-9b6c-b5f212e963b3",
        "z": 190,
        "router": {
          "name": "orthogonal"
        },
        "vertices": [
          {
            "x": 823.5,
            "y": 244
          },
          {
            "x": 476,
            "y": 244
          }
        ]
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "3b18f78b-069b-4b5b-9225-5d665eee862f",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "4.762%",
              "dy": "96.923%",
              "rotate": true
            }
          },
          "priority": true
        },
        "target": {
          "id": "4206bce8-f05f-4472-b68e-658e0002a989",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "13.846%",
              "dy": "85%",
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
                "text": "EV_BTN_ENTER"
              }
            },
            "position": {
              "distance": 0.68362545863917,
              "offset": -42,
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
        "id": "ed5d7741-1a0b-4e60-ae06-1b943ce526f6",
        "z": 191,
        "router": {
          "name": "orthogonal"
        },
        "vertices": [
          {
            "x": -6,
            "y": 215
          }
        ]
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "b05c30d6-95c2-4f94-bfe5-f028ad7cce92"
        },
        "target": {
          "id": "3b18f78b-069b-4b5b-9225-5d665eee862f",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "88.745%",
              "dy": "81.538%",
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
                "text": "[param_idx == 0]/ val_idx = (val_idx + 1) % 2"
              }
            },
            "position": {}
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
        "id": "a8aec815-7a89-4cdb-92bf-6370ce15d315",
        "z": 192,
        "router": {
          "name": "orthogonal"
        },
        "vertices": [
          {
            "x": 562,
            "y": 195
          }
        ]
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "b05c30d6-95c2-4f94-bfe5-f028ad7cce92"
        },
        "target": {
          "id": "3b18f78b-069b-4b5b-9225-5d665eee862f",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "58.009%",
              "dy": "90.769%",
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
                "text": "else / val_idx = (val_idx + 1) % 2"
              }
            },
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
        "id": "203f98f8-a374-4f1c-a138-a378dd76955b",
        "z": 193,
        "router": {
          "name": "orthogonal"
        },
        "vertices": [
          {
            "x": 823.5,
            "y": 317
          },
          {
            "x": 449,
            "y": 317
          }
        ]
      },
      {
        "position": {
          "x": -125,
          "y": -236
        },
        "size": {
          "width": 162,
          "height": 60
        },
        "type": "State",
        "attrs": {
          "name": {
            "text": "ST_MAIN",
            "fontSize": 11
          }
        },
        "id": "bd9e05cb-c6f2-45a0-9b39-94bcffe88a6c",
        "z": 201
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "7b8af555-248f-4c5f-9d32-d6cf8769c571"
        },
        "target": {
          "id": "bd9e05cb-c6f2-45a0-9b39-94bcffe88a6c",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "17.901%",
              "dy": "35%",
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
                "text": "EV_BTN_ESCAPE"
              }
            },
            "position": {
              "distance": 0.5434782608695652,
              "offset": -42,
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
        "id": "8428332f-490f-4bd2-a663-8306f2b6b4db",
        "z": 202,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "bd9e05cb-c6f2-45a0-9b39-94bcffe88a6c"
        },
        "target": {
          "id": "7b8af555-248f-4c5f-9d32-d6cf8769c571",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "37.255%",
              "dy": "6.667%",
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
                "text": "EV_BTN_ENTER/\nmotor_idx = 0"
              }
            },
            "position": {
              "distance": 0.5652173913043478,
              "offset": -41,
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
        "id": "aaad9ddc-faa4-433c-95d6-5d86c4b61f47",
        "z": 202,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
      },
      {
        "position": {
          "x": -60,
          "y": -309
        },
        "size": {
          "height": 18,
          "width": 18
        },
        "type": "Entry",
        "entryKind": "Initial",
        "attrs": {},
        "id": "1b6a9f6d-dee7-45c7-b940-018539c7616f",
        "z": 206,
        "embeds": [
          "861d1db0-2f47-431b-b191-725e3c53e40b"
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
          "x": -60,
          "y": -294
        },
        "attrs": {
          "label": {
            "refX": "50%",
            "textAnchor": "middle",
            "refY": "50%",
            "textVerticalAnchor": "middle"
          }
        },
        "id": "861d1db0-2f47-431b-b191-725e3c53e40b",
        "z": 207,
        "parent": "1b6a9f6d-dee7-45c7-b940-018539c7616f"
      },
      {
        "type": "Transition",
        "attrs": {},
        "source": {
          "id": "1b6a9f6d-dee7-45c7-b940-018539c7616f"
        },
        "target": {
          "id": "bd9e05cb-c6f2-45a0-9b39-94bcffe88a6c",
          "anchor": {
            "name": "topLeft",
            "args": {
              "dx": "56.667%",
              "dy": "21.667%",
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
        "id": "a0224ac1-5af3-40b1-bb4d-ac7e6d99a1d6",
        "z": 208,
        "router": {
          "name": "orthogonal"
        },
        "vertices": []
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
          "moduleName": "SystemSetupMenu",
          "statemachinePrefix": "systemSetupMenu",
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
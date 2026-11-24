|    Path    |    Purpose    |    Created By/Changed By    |
-------------------------------------------------------------
|  .minigit/ | Root directory for all miniGit metadata |  minigit init/Later MiniGit commands | 
|  .minigit/HEAD | Stores the commit ID, empty immediately after init | minigit init/ minigit commit |
|  .minigit/index.txt | Stores a file list currently staged, empty after init | minigit init/ minigit add, minigit commit | 
|  .minigit/stage/ | Holds copies of staged file content | minigit init/ minigit add, minigit commit |
|  .minigit/commits/ | Holds one subdirectory per commit | minigit init/ minigit commit |
|  .minigit/commits/<commit-id>/files/ | holds saved file snapshot for one commit | minigit commit |
|  .minigit/commits/<commit-id>/ | Directory for one completed commit | minigit commit |



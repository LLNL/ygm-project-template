# ygm-project-template
This repo is meant to be a starting point for creating projects using YGM. It contains the necessary CMake boilerplate
to pull YGM and its dependencies.

# Using this Repo
As this repo is meant as a template, not as a repo to collect applications, it should be used to get a project started,
but the remote for your new project should be updated.

For example, if you have an empty repo created at `https://www.github.com:your_username/cool_project_name`, you can do the
folllowing:
```
git clone git@github.com:LLNL/ygm-project-template.git
mv ygm-project-template cool_project_name
cd cool_project_name
git remote set-url origin git@github.com:your_username/cool_project_name.git
git push
```

This will `clone` this template project, change the directory on your local copy to the desired project's name, and then
set the remote location for properly pushing and pulling from your project's location. The final `git push` will create
a copy of this template project in your empty Github project. At this point, you can write and compile your own code
using YGM and keep it in your own repository.

# Adding an Executable
To add a new executable, write your own `cool_ygm_application.cpp` file in the `src` directory, similar to `src/read_json_example.cpp`. To
make CMake add your code to its list of compilation targets, add the line `add_ygm_executable(cool_ygm_application)` to
`src/CMakeLists.txt`.

# Building Your Code
After your executable has been added to the CMake setup, it can be built by running
```
mkdir build
cd build
cmake ..
make
```
from the top level of this project.

# Using YGM-Adjacent Libraries
This repository also supports using SaltAtlas and Krowkee in projects. By default, they are not added, but then can be
added by adding `-DUSE_SALTATLAS=On` or `-DUSE_KROWKEE=On` to the `cmake` command when building. Running `make` will
then compile all applications with the appropriate SaltAtlas and/or Krowkee headers included.

To make the above change permanent (and avoid having to type `-DUSE_SALTATLAS=On` every time `cmake` is run), the 
`CMakeLists.txt` file can be changed from having `option(USE_SALTATLAS OFF)` to `option(USE_SALTATLAS ON)`.

# VS Code Users
By default, VS Code's C++ IntelliSense does not know the location YGM's `include` directories will be at from our CMake
configuration. This means code completion features will not work without some modification. To remedy this:
1. Open the `C/C++ Configuration` in VS Code (`C/C++: Edit Configuration (UI)` in the Command Palette).
2. Navigate to `Compile commands` in `Advanced Settings`.
3. Add `${workspaceFolder}/build/compile_commands.json` to the dialog box.

After making this change one time, code completion features for YGM will work in any project built from this template.

# License
YGM is distributed under the MIT license.

All new contributions must be made under the MIT license.

See [LICENSE-MIT](LICENSE-MIT), [NOTICE](NOTICE), and [COPYRIGHT](COPYRIGHT) for
details.

SPDX-License-Identifier: MIT

# Release
LLNL-CODE-789122

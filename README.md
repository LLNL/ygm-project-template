# ygm-project-template
This repo is meant to be a starting point for creating projects using YGM. It contains the necessary CMake boilerplate
to pull YGM and its dependencies.

## Using this Repo
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

## Adding an Executable
To add a new executable, write your own `cool_ygm_application.cpp` file in the `src` directory, similar to `src/read_json_example.cpp`. To
make CMake add your code to its list of compilation targets, add the line `add_ygm_executable(cool_ygm_application)` to
`src/CMakeLists.txt`.

initializer
===========
### Why
- boilerplate projects need manual update code base (libraries and the code call them) and project-dependent names / variables

### How to use
- run initializer under the initfile.yaml directory (**DO NOT run in this repo**)

### `initfile.yaml` Sample
```yaml
CMakeLists.txt:
- haystack: project("initializer")
  needle: initializer
  occurrence: 1
.vscode/launch.json:
- haystack: '"cppStandard": "c++11"'
  needle: c++11
  occurrence: 1
```

### to be initialized
- iris spa
- cpp full setup
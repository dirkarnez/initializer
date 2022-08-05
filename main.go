// You can edit this code!
// Click here and start typing.
package main

import (
	"fmt"
	"regexp"
)

func main() {
	content := `cmake_minimum_required(VERSION 3.14)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

#    initializer-start: {"project name": "pdfium-playground"}
project(pdfium-playground)
#initializer-end

include(CMakeParseArguments)

# target_copy_libraries(target items...)
macro(target_copy_libraries target)
  foreach(arg IN ITEMS ${ARGN})
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${arg}> $<TARGET_FILE_DIR:${target}>
    )
  endforeach()
endmacro()

find_package(PDFium REQUIRED PATHS "${PDFIUM_ROOT}")

if (PDFium_FOUND)
    message("PDFium found")
else (PDFium_FOUND)
    message("PDFium not found")
endif()

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

add_executable(${PROJECT_NAME} 
    "src/main.cpp"
)

target_link_libraries(${PROJECT_NAME} pdfium)
target_copy_libraries(${PROJECT_NAME} pdfium)
`

	r := regexp.MustCompile(`(?sm)(#\s*initializer-start\s*:\s*(.*)\n+([^\s]+)\n+#\s*initializer-end)`)

	matched := r.FindAllStringSubmatch(content, -1)
	fmt.Println("[2]:", matched[0][2])
	fmt.Println("[3]:", matched[0][3])

	// re := regexp.MustCompile(`a(x*)b`)
	fmt.Println(r.ReplaceAllString(content, "T"))
	// fmt.Println(re.ReplaceAllString("-ab-axxb-", "$1"))
	// fmt.Println(re.ReplaceAllString("-ab-axxb-", "$1W"))
	// fmt.Println(re.ReplaceAllString("-ab-axxb-", "${1}W"))

}

// package main

// import (
// 	"fmt"
// 	"go/ast"
// 	"go/parser"
// 	"go/printer"
// 	"go/token"
// 	"os"

// 	"golang.org/x/tools/go/ast/astutil"
// )

// func main() {
// 	fset := token.NewFileSet() // positions are relative to fset

// 	src := `package main

// import (
// 	"fmt"
// 	"time"
// )

// func main() {
// 	fmt.Println(time.Now())
// }`

// 	// Parse src but stop after processing the imports.
// 	file, err := parser.ParseFile(fset, "", src, parser.AllErrors)
// 	if err != nil {
// 		fmt.Println(err)
// 		return
// 	}

// 	// Print the imports from the file's AST.
// 	for _, s := range file.Imports {
// 		fmt.Println(s.Path.Value)
// 	}

// 	astutil.Apply(file, nil, func(c *astutil.Cursor) bool {
// 		n := c.Node()

// 		switch x := n.(type) {

// 		case *ast.CallExpr:

// 			id, ok := x.Fun.(*ast.Ident)

// 			if ok {
// 				// if id.Name == "pred" {
// 				// 	c.Replace(&ast.UnaryExpr{
// 				// 		Op: token.NOT,
// 				// 		X:  x,
// 				// 	})
// 				// }
// 				fmt.Println(id.Name)
// 			}
// 		case *ast.SelectorExpr:
// 			id, ok := x.X.(*ast.Ident)
// 			if ok {
// 				// if id.Name == "pred" {
// 				// 	c.Replace(&ast.UnaryExpr{
// 				// 		Op: token.NOT,
// 				// 		X:  x,
// 				// 	})
// 				// }
// 				fmt.Println(id.Name)

// 			}

// 			fmt.Println(x.Sel.Name)
// 		case *ast.FuncDecl:
// 			if x.Name.Name != "main" {
// 				return true
// 			}

// 			newCallStmt := &ast.ExprStmt{
// 				X: &ast.CallExpr{
// 					Fun: &ast.SelectorExpr{
// 						X: &ast.Ident{
// 							Name: "fmt",
// 						},
// 						Sel: &ast.Ident{
// 							Name: "Println",
// 						},
// 					},
// 					Args: []ast.Expr{
// 						&ast.BasicLit{
// 							Kind:  token.STRING,
// 							Value: `"instrumentation"`,
// 						},
// 					},
// 				},
// 			}

// 			x.Body.List = append([]ast.Stmt{newCallStmt}, x.Body.List...)
// 		}

// 		return true
// 	})

// 	// ast.Print(nil, file)
// 	printer.Fprint(os.Stdout, fset, file)
// }

package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/printer"
	"go/token"
	"os"

	"golang.org/x/tools/go/ast/astutil"
)

func main() {
	fset := token.NewFileSet() // positions are relative to fset

	src := `package main

import (
	"fmt"
	"time"
)

func main() {
	fmt.Println(time.Now())
}`

	// Parse src but stop after processing the imports.
	file, err := parser.ParseFile(fset, "", src, parser.AllErrors)
	if err != nil {
		fmt.Println(err)
		return
	}

	// Print the imports from the file's AST.
	for _, s := range file.Imports {
		fmt.Println(s.Path.Value)
	}

	astutil.Apply(file, nil, func(c *astutil.Cursor) bool {
		n := c.Node()

		switch x := n.(type) {

		case *ast.CallExpr:

			id, ok := x.Fun.(*ast.Ident)

			if ok {
				// if id.Name == "pred" {
				// 	c.Replace(&ast.UnaryExpr{
				// 		Op: token.NOT,
				// 		X:  x,
				// 	})
				// }
				fmt.Println(id.Name)
			}
		case *ast.SelectorExpr:
			id, ok := x.X.(*ast.Ident)
			if ok {
				// if id.Name == "pred" {
				// 	c.Replace(&ast.UnaryExpr{
				// 		Op: token.NOT,
				// 		X:  x,
				// 	})
				// }
				fmt.Println(id.Name)

			}

			fmt.Println(x.Sel.Name)
		case *ast.FuncDecl:
			if x.Name.Name != "main" {
				return true
			}

			newCallStmt := &ast.ExprStmt{
				X: &ast.CallExpr{
					Fun: &ast.SelectorExpr{
						X: &ast.Ident{
							Name: "fmt",
						},
						Sel: &ast.Ident{
							Name: "Println",
						},
					},
					Args: []ast.Expr{
						&ast.BasicLit{
							Kind:  token.STRING,
							Value: `"instrumentation"`,
						},
					},
				},
			}

			x.Body.List = append([]ast.Stmt{newCallStmt}, x.Body.List...)
		}

		return true
	})

	// ast.Print(nil, file)
	printer.Fprint(os.Stdout, fset, file)
}

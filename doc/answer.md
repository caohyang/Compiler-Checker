问题1-1：

`FuncDef`: 包括参数`ret_type`, `param_list`, `name`, `body`. `visit`函数先输出返回类型和函数名，然后调用`accept`函数继续访问节点的函数参数和函数体。

`BlockStmt`: 包括参数`body`（指向`block`的指针列表）。`visit`函数按缩进格式输出语句块，并继续访问每个语句。

`VarDef`: 包括参数`is_constant`, `btype`, `name`, `is_inited`, `array_length`, `initializers`. `visit`函数根据`is_constant`判断是否输出代表常量定义的字符串`const`，再输出类型，访问数组每一维的表达式，如果有初始化则输出初始值并访问对应表达式。

`InitVal`:

`LVal`:

`FuncParam`:

`FuncFParamList`:

问题1-2： 

在`build`目录下使用命令`./sysYFParser -emit-ast ../testcase.sy`，可得到语法树的输出结果为`int a = (2%3);`

Note: 上述结果与`testcase.sy`中的内容`int a = 2 % 3;`略有不同。

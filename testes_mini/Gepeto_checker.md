# Minishell - Checker

## 1. Comandos Simples [ok]

- [x] comando: /bin/ls
- Resultado Esperado: Lista o conteúdo do diretório atual.

- [x] Comando: "" (comando vazio)
- _Resultado Esperado:_ Nenhuma saída, o shell não deve falhar.

- [x] Comando: Somente espaços ou tabulações.
- Resultado Esperado: Nenhuma saída, o shell não deve falhar.

## 2. Redirecionamento [ok]

- [x] Comando: echo "hello" > file.txt
- Resultado Esperado: Cria um arquivo file.txt contendo "hello".

- [x] Comando: cat file.txt
- Resultado Esperado: Exibe o conteúdo de file.txt.

- [x] Comando: echo "append" >> file.txt
- Resultado Esperado: Adiciona "append" ao final de file.txt.

- [ ] Comando: cat < file.txt > out.txt
- Resultado Esperado: captura o conteúdo de file.txt e envia para out.txt

## 3. Variáveis de Ambiente [ok]

- [x] Comando: env
- Resultado Esperado: Lista as variáveis de ambiente atuais.

- [x] Comando: export TEST_VAR=test_value && env | grep TEST_VAR
- Resultado Esperado: Exibe TEST_VAR=test_value.

- [x] Comando: unset TEST_VAR && env | grep TEST_VAR
- Resultado Esperado: Não exibe nenhuma linha contendo TEST_VAR.

## 4. Echo [ok]

- [x] Comando: echo hello world
- Resultado Esperado: Exibe hello world.

- [x] Comando: echo -n hello
- Resultado Esperado: Exibe hello sem uma nova linha ao final.

## 5. Compilação [ok]

- [x] Comando: make re 
- Resultado Esperado: volta compilar o programa sem erros..

- [!] Comando: make -n (não temos essa flag configuraa)
- Resultado Esperado: Mostra que os flags -Wall -Wextra -Werror estão sendo usados.
- Resultado Atual:

```
minishell> make -n
Makefile:53: warning: overriding recipe for target 'minishell'
Makefile:48: warning: ignoring old recipe for target 'minishell'
make: Circular minishell <- minishell dependency dropped.
make: 'minishell' is up to date.
```

- [x] Comando: make _[SÓ VOLTA A COMPILAR DEPOIS QUE RODAR O MAKE FCLEAN]_
- Resultado Esperado: O projeto compila sem erros.

## 6. Caminho do Ambiente [ok]

- [x] Comando: ls
- Resultado Esperado: Lista o conteúdo do diretório atual.

- [x] Comando: unset PATH && ls
- Resultado Esperado: ls: command not found.

- [x] Comando: export PATH=/bin && ls
- Resultado Esperado: Lista o conteúdo do diretório atual.

## 7. Controle de Sinais [ok]

- [x] Comando: Ctrl+C em um prompt vazio.
- Resultado Esperado: O prompt é reiniciado sem erros.

- [x] Comando: Ctrl+D em um prompt vazio.
- Resultado Esperado: O shell encerra.

## 8. Pipes [!NOK!]

- [x] Comando: echo "hello" | cat
- Resultado Esperado: Exibe hello.

- [x] Comando: ls | grep "file_that_does_not_exist"
- Resultado Esperado: Nenhuma saída.

- [!] Comando: ls | cat > output.txt
- Resultado Esperado: Cria output.txt com o conteúdo listado.
- Resultado Atual:

```
minishell> ls | cat > output.txt
[1]    69093 segmentation fault (core dumped)  ./minishell
```

## 9. Exit [ok]

- [x] Comando: exit
- Resultado Esperado: O shell encerra sem erros.

- [x] Comando: exit 42
- Resultado Esperado: O shell encerra com código de saída 42.

## 10. Cotações Simples e Duplas [ok]

- [x] Comando: echo 'hello $USER'
- Resultado Esperado: Exibe hello $USER.

- [x] Comando: echo "hello $USER"
- Resultado Esperado: Exibe hello seguido do valor da variável USER.

## 11. Diretório Atual [ok]

- [x] Comando: pwd
- Resultado Esperado: Exibe o caminho do diretório atual.

- [x] Comando: echo $OLDPWD
- Resultado Esperado: Exibe o caminho do diretório anterior. Muda conforme percorre o cd.

- [x] Comando: echo $HOME
- Resultado Esperado: Exibe o caminho da HOME.

- [x] Comando: cd ..
- Resultado Esperado: Vai diretório anterior.

- [x] Comando: cd
- Resultado Esperado: Vai para a HOME.

## 12. Valor de Retorno [!NOK!]

- [x] Comando: /bin/ls && echo $?
- Resultado Esperado: Exibe 0.

- [x] Comando: /bin/ls non_existent_file && echo $?
- Resultado Esperado: Exibe 2.

# Minishell - Checker

- Erros a serem tratados
  (Lista do Azevedo com algumas modificações)
  ## INÍCIO E FINAL SYNTAX:
  - [ ] $ < echo
        bash: echo: No such file or directory
        Retorno: 1
  - [ ] $ echo <
        bash: syntax error near unexpected token `newline'
        Retorno: 2
  - [ ] $ echo <<
        bash: syntax error near unexpected token `newline'
        Retorno: 2
  - [ ] $ > echo
  - [ ] $ echo >
        bash: syntax error near unexpected token `newline'
        Retorno: 2
  - [ ] $ >> echo
  - [ ] $ echo >>
        bash: syntax error near unexpected token `newline'
        Retorno: 2
  ## PIPE SYNTAX: ' | '
  - [ ] $ echo | < ls
        bash: ls: No such file or directory
        Retorno: 0
  - [ ] $ echo | << ls
    > ls
  - [ ] $ echo | >> ls
        Depois, APAGAR o arquivo ls criado!
  - [ ] $ echo | > ls
  - [ ] $ echo | | ls
        bash: syntax error near unexpected token `|'
        Retorno: 2
  - [ ] $ echo | || ls
        bash: syntax error near unexpected token `||'
        Retorno: 2
  - [ ] $ echo ||| ls
        bash: syntax error near unexpected token `|'
        Retorno: 2
  - [ ] $ echo | ||| ls
        bash: syntax error near unexpected token `||'
        Retorno: 2
  - [ ] $ echo | || | ls
        bash: syntax error near unexpected token `||'
        Retorno: 2
  - [ ] $ echo || | ls
        bash: syntax error near unexpected token `|'
        Retorno: 2
  ## OUTPUT SYNTAX: ' > '
  - [ ] $ echo > < ls
        bash: syntax error near unexpected token `<'
        Retorno: 2
  - [ ] $ echo > << ls
        bash: syntax error near unexpected token `<<'
        Retorno: 2
  - [ ] $ echo > >> ls
        bash: syntax error near unexpected token `>>'
        Retorno: 2
  - [ ] $ echo > > ls
        bash: syntax error near unexpected token `>'
        Retorno: 2
  - [ ] $ echo > | ls
        bash: syntax error near unexpected token `|'
        Retorno: 2
  - [ ] $ echo > || ls
        bash: syntax error near unexpected token `||'
        Retorno: 2
  ## HEREDOC SYNTAX: ' << '
  - [ ] $ echo << < ls
        bash: syntax error near unexpected token `<'
        Retorno: 2
  - [ ] $ echo << << ls
        bash: syntax error near unexpected token `<<'
        Retorno: 2
  - [ ] $ echo << >> ls
        bash: syntax error near unexpected token `>>'
        Retorno: 2
  - [ ] $ echo << > ls
        bash: syntax error near unexpected token `>'
        Retorno: 2
  - [ ] $ echo << | ls
        bash: syntax error near unexpected token `|'
        Retorno: 2
  - [ ] $ echo << || ls
        bash: syntax error near unexpected token `||'
        Retorno: 2
  ## INPUT SYNTAX ' < '
  - [ ] $ echo < < ls
        bash: syntax error near unexpected token `<'
        Retorno: 2
  - [ ] $ echo < << ls
        bash: syntax error near unexpected token `<<'
        Retorno: 2
  - [ ] $ echo < >> ls
        bash: syntax error near unexpected token `>>'
        Retorno: 2
  - [ ] $ echo < > ls
        bash: syntax error near unexpected token `>'
        Retorno: 2
  - [ ] $ echo < | ls
        bash: syntax error near unexpected token `|'
        Retorno: 2
  - [ ] $ echo < || ls
        bash: syntax error near unexpected token `||'
        Retorno: 2
  ## APPEND SYNTAX: ' >> '
  - [ ] $ echo >> < ls
        bash: syntax error near unexpected token `<'
        Retorno: 2
  - [ ] $ echo >> << ls
        bash: syntax error near unexpected token `<<'
        Retorno: 2
  - [ ] $ echo >> >> ls
        bash: syntax error near unexpected token `>>'
        Retorno: 2
  - [ ] $ echo >> > ls
        bash: syntax error near unexpected token `>'
        Retorno: 2
  - [ ] $ echo >> | ls
        bash: syntax error near unexpected token `|'
        Retorno: 2
  - [ ] $ echo >> || ls
        bash: syntax error near unexpected token `||'
        Retorno: 2
- Testes de comandos a serem feitos (Lista da Thais com algumas modificações)

  ## Comandos Simples

  ### Caminho Absoluto

  - [x] /bin/ls
  - [x] /bin/pwd
  - [x] /bin/ls -la
  - [x] /bin/echo "Hello, World!"
  - [ ] /bin/comandnotfound

  ### Manipulação de Argumentos

  - [x] /bin/echo Argument1 Argument2
  - [x] /bin/ls /home/user

  ### Comando Vazio

  - [ ] input ENTER
  - [ ] Comando somente com espaços
  - [ ] Comando que não existe

  ## Builtin

  ### echo

  - [x] echo -n
  - [x] echo "alguma coisa"
  - [x] echo alguma coisa
  - [x] echo "alguma" "coisa"
  - [x] echo "alguma coisa"
  - [x] echo "alguma""coisa"
  - [x] echo "'alguma coisa'" → deve sair ('alguma coisa')
  - [ ] echo "'$HOME'" → deve sair ('/home/user')
  - [x] echo "'$HOME"'
  - [x] echo "'alguma coisa"'
  - [x] echo $?
  - [x] echo $HOME
  - [x] echo "$HOME"

  ### exit

  - [ ] exit / ctrl D
  - [x] exit 42

  ## Retorno do valor do processo

  - [ ] /bin/ls → echo $? 0
  - [ ] cat a → echo $? 1
  - [x] expr $? $?

  ## Signals

  - [ ] Ctrl C
  - [ ] Ctrl + \
  - [ ] Ctrl D
  - [ ] Ctrl C depois de escrever algo no prompt, quebra de linha e novo prompt
  - [ ] Ctrl D depois de escrever algo no prompt, faz nada
  - [ ] Ctrl + \ depois de escrever algo no prompt, faz nada
  - [ ] Ctrl C depois de usar cat ou grep sem argumentos
  - [ ] Ctrl + \ depois de usar cat ou grep sem argumentos
  - [ ] thaismeneses@BRRJSOFT38:~/My_minishell$ cat^\Quit
  - [ ] Ctrl D depois de usar cat ou grep sem argumentos

  ## Aspas Duplas

  - [x] echo "cat lol.c | cat > lol.c"
  - [x] echo "alguma" "coisa"
  - [x] echo "cat lol.c | cat > lol.c"
  - [x] echo "cat lol.c | wc "
  - [x] echo "cat > lol.c"
  - [x] echo "cat > lol.c
  - [x] echo "$USER"
  - [x] rm "ls>out"

  ## Aspas Simples

  - [x] echo 'cat lol.c | cat > lol.c'
  - [x] echo 'cat lol.c | wc '
  - [x] echo 'cat > lol.c'
  - [x] echo ' '
  - [x] echo ' cat > lol.c
  - [x] echo '$USER'

  ## Env

  - [x] Comando Env funcionando (env ou envp)
  - [x] env | grep PWD
  - [x] O Env atualizando o pwd e o old_pwd ao usar cd

  ## Export

  - [x] export VARIAVEL"valor"
  - [ ] echo $VARIAVEL -> quabra de linha qd não existe
  - [x] echo $VARIAVEL -> expande qd existe

  ## Unset

  - [x] unset PATH
  - [x] erro ao executar execve após unset do PATH

  ## cd

  - [x] Usando cd para navegar os diretórios
  - [x] cd ..
  - [x] cd ~
  - [x] cd -
  - [ ] cd sem argumentos - saindo com "[1] 54216 segmentation fault (core dumped) ./minishell"
  - [x] cd.. comando não encontrado - rever erro

  ## pwd

  - [x] Usar o pwd quando navegar pelo os diretórios

  ## Relative Path

  - [ ] cd sources/execute
  - [ ] ls sources/builtins
  - [ ] ./minishell
  - [ ] cp out.txt sources/
  - [ ] rm sources/out.txt

  ## Environment Path

  - [ ] export PATH
  - [ ] Tentar utilizar os comandos (ls, wc, awk)
  - [ ] unset PATH
  - [ ] export PATH/usr/local/bin:/usr/bin

  ## USO do awk

  ## Redirection

  - [ ] $USER TEM QUE SAIR COM $USER - rever erro
  - [ ] EOF - rever erro
  - [ ] EOF cat > test - rever erro
  - [ ] cat Makefile > out.txt
  - [ ] echo ola > test 1 test2 >test3
  - [x] cat Makefile < out
  - [x] EOF1 cat > teste1 | EOF2 cat | grep "allan"

  ## Pipes

  - [x] ls -a | wc -l
  - [x] ls -la | wc -l | echo testando 123 | grep 123
  - [ ] ls -la | wc -l > teste1 | echo testando 123 | grep 123

  ## GO Crazy

  - [x] ./minishell/ → bash: ./minishell/: Not a directory
  - [ ] $?126 - corrigir expansor e exit status
  - [x] includes/ → bash: includes/: Is a directory
  - [x] $?126 - corrigir expansor e exit status

  ## Environment Variables

  - [ ] Expansão da variável global
  - [ ] grep a → tem que expandir 130
  - [ ] cat → tem que expandir 130

- Testes de comandos a serem feitos - Parte 02
  (Lista do Notion [https://aerial-turquoise-a8d.notion.site/Minihell-Tests-d25951949c8047a9a8aeed5340631642](https://www.notion.so/d25951949c8047a9a8aeed5340631642?pvs=21))
  ## **Execução de comandos**
  ### **Casos de erro:**
  ex 1 - comando não existe:
  - [ ] Minishell> foo
        minishell: foo: command not found
        Retorno: 127
        ex 2 - localização não existe
  - [ ] Minishell> ./foo/ls
        ./foo/ls: command not found
        Retorno: 127
        ex 3 - sem permissão no executável:
  - [ ] Minishell> chmod -x ./minishell
        Minishell> ./minishell
        minishell: ./minishell: permission denied
        Retorno: 126
        ex 4 - sem permissão para ler/abrir a pasta onde está o executável
  - [ ] Minishell> mkdir temp
        Minishell> cp ./minishell ./temp/
        Minishell> chmod -x ./temp
        Minishell> ./temp/minishell
        minishell: ./temp/minishell: Permission denied
        Retorno: 126
        ex 5 - aspas vazias (simples e duplas)
  - [ ] Minishell> "" (ou) ''
        minishell: : command not found
        Retorno: 127
  - [ ] Minishell> " "
        minishell: : command not found
  - [ ] Minishell> ''
        minishell: : command not found
  - [ ] Minishell> ' '
        minishell: : command not found
        ex 6 - comandos entre aspas
  - [ ] Minishell> "echo" "Hello World"
        Hello World
        Retorno: 0
  - [ ] Minishell> "echo " "Hello World"
        minishell: echo : command not found
        Retorno: 127
  - [ ] Minishell> " echo" "Hello Minihell"
        minishell: echo: command not found
        ex 7 - aspas simples dentro de aspas duplas e vice-versa
  - [ ] Minishell> '""'
        minishell: "": command not found
        Retorno: 127
        ex 8 - cd puro e variável $HOME não existe
  - [ ] Minishell> unset HOME
        Minishell> cd
        minishell: cd: HOME not defined
        Minishell> pwd
        <diretorio atual>
        ex 9 - caminho absoluto
  - [ ] Minishell> /bin/ls
        Minishell> {... output de ls ....}
        Retorno: 0
        ex 10 - caminho relativo
  - [ ] Minishell> cd {puro, vai direto para a HOME do usuário}
        Minishell> ../../bin/ls
        {... output de ls ...}
        Retorno: 0
        ex 10.1 - caminho relativo
  - [ ] Minishell> mkdir -p ./test1/test2/
        Minishell> cp /bin/ls ./test1/test2/
        Minishell> ./test1/test2/ls
        {... output de ls ...}
        ex 11 - permissão negada
  - [ ] Minishell> cp /bin/ls .
        Minishell> chmod -x ls
        Minishell> ./ls
        Retorno: 126
        ex 12 - variável de ambiente
  - [ ] Minishell> export val="echo"
        Minishell> $val "que doideira é essa?"
        que doideira é essa.
        Retorno: 0
        ex 12.1 - comando e argumentos em variável de ambiente
  - [ ] Minishell> export cmd="echo" a="eu" b="devo" c="imprimir"
        Minishell> $cmd $a $b $c
        eu devo imprimir
        Retorno: 0
        ex 13 - variável de ambiente não existe
  - [ ] Minishell> $eu_nao_existo
        Minishell> {input vazio}
        Retorno: 0
        ex 14 - variável de ambiente vazia seguida de um comando
  - [ ] Minishell> $eu_nao_existo ls
        {... output de ls ...}
        Retorno: 0
        ex 15 - variável de ambiente e aspas duplas ou simples
  - [ ] Minishell> export e="echo"
        Minishell> "$e" "Que viagem é essa?"
        Retorno: 0
  - [ ] Minishell> '$e' "Que viagem é essa?"
        minishell: $e: command or file not found
        Retorno: 127
        ex 16 - PATH deletada
  - [ ] Minishell> unset PATH
        Minishell> ls
        minishell: ls: command or file not found
        Retorno: 127
  - [ ] Minishell> grep
        minishell: grep: command or file not found
        Retorno: 127
  - [ ] Minishell> /bin/ls
        {... output de ls ...}
        ex 16.1 - invocando sem variável de ambiente
  - [ ] bash: env -i ./minishell
        Minishell> env
        {... deve retornar vazio ...}
  - [ ] Minishell> ls
        minishell: ls: commmand or file not found
        Retorno: 127
  - [ ] Minishell> grep
        minishell: grep: command or file not found
        Retorno: 127
  - [ ] Minishell> echo "builtins devem funcionar"
        builtins devem funcionar

## **Sinais e EOF**

- **Ctrl + C - Envia um SIGINT**

ex 1: deve

- [ ] Minishell> ^C
      Minishell>

ex 2:

- [ ] Minishell> ^C
      Retorno: 130

ex 3:

- [ ] Minishell> cat /dev/urandom
      ^C
      Minishell>
      Retorno: 130

ex 3.1

- [ ] Minishell> cat
      {... ctrl + c ...}
      Retorno: 130

- **Ctrl + D - EOF (end of file)**

Ctrl + D (^D) fecha o terminal caso o prompt esteja vazio, ele chama a função exit

ex 1:

- [ ] Minishell> exit
      bash>

Em caso do terminal estar vazio ele faz nada

ex 2:

- [ ] Minishell> "text"\_
- **Ctrl + \ - Envia um SIGQUIT**

Ctrl + \ (^\) deve fazer nada

ex 1:

- [ ] Minishell>

ex 2: ctrl + \ dentro de um processo filho deve executar da forma padrão (SIGUIT)

- [ ] Minishell> cat
      {.... abre stdin para input ...}
      {... ctrl + \ ...}
      Sair
      Retorno: 130

ex 2.1

- [ ] Minishell> sleep 1000
      {... bota o processo pra dormir ...}
      {... ctrl + \ ...}
      Sair
      Retorno: 130

## **Funções Builtins**

- **Echo**

Echo aceita multiplos argumentos e os imprime com um espaço de distância.
Quando termina de imprimir os argumentos ele imprime uma quebra de linha.
Caso a flag -n esteja sendo utilizada ele passa a não quebrar a linha.
Outras flags não devem ser tratatas e podem ser consideradas um argumento.
Seu retorno é 1 em caso de erro e 0 em caso de sucesso.

ex 1:

- [ ] Minishell> echo "Hello World"
      Hello World
      Minishell>

ex 2:

- [ ] Minishell> echo -n "Sem Quebra "
      Sem Quebra Minishell>

Casos extremos com flags

ex 3:

- [ ] Minishell> echo -nnnnnnnn "Ainda sem Quebra "
      Ainda sem Quebra Minishell>

ex 4:

- [ ] Minishell> echo -nnnnnnnn -n -n -n -n "Ainda sem Quebra "
      Ainda sem Quebra Minishell>

ex 5:

- [ ] Minishell> echo -nnnnnnnn -na -n -n -n "Flag quebrada "
      -na -n -n -n Flag quebrada Minishell>

Sem argumentos

ex 6:

- [ ] Minishell> echo
      Minishell>

ex 7:

- [ ] Minishell> echo -n
      Minishell>
- **Cd**

cd recebe no máximo 1 argumento. Caso esse argumento seja um caminho relativo ou absoluto valido, ele altera a working directory para esse argumento. Caso receba mais argumentos ou um caminho invalido retorna erro, seu retorno é 1 em caso de erro e 0 em caso de sucesso.

ex 1:

- [ ] Minishell> cd /home
      Minishell> pwd
      /home
      Minishell>

ex 2:

- [ ] Minishell> cd /home
      Minishell> pwd
      /home
      Minishell>

ex 3:

- [ ] Minishell> pwd
      /home/projetos
      Minishell> cd ..
      Minishell> pdw
      /home
      Minishell>

Em caso de nenhum argumento fornecido ele vai pra home setada em env

ex 4:

- [ ] Minishell> cd
      Minishell> pwd
      /home
      Minishell>

Casos de erros

ex 5:

- [ ] Minishell> cd /home extra_arg
      Minishell: cd: too many arguments
      Retorno: 1

ex 6:

- [ ] Minishell> cd /eu_nao_existo
      Minishell: cd: /eu_nao_existo: No such file or directory
      Retorno: 1

ex 7:

- [ ] Minishell> mkdir eu_nao_sou_acessivel
      Minishell> ch
- [ ] Minishell> cd /eu_nao_sou_acessivel
      Minishell: cd: eu_nao_sou_acessivel: Permission denied
      Retorno: 1

ex 7.1

- [ ] Minishell> mkdir nao_acessivel_em_breve
      Minishell> cd nao_acessivel_em_breve
      Minishell> chmod -x ../nao_acessivel_em_breve
      Minishell> cd .
      minishell: cd: .: Permission denied
      Retorno: 1

ex 8:

- [ ] Minishell> mkdir -p temp1/temp2
      Minishell> cd ./temp1/temp2
      Minishell> rm -rf ../../temp1
      Minishell> cd ..
      chdir: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory

ex 9:

- [ ] Minishell> cd | ls (ou) ls | cd
      {... conteúdo do diretório ...}
      Minishell> pwd
      {... diretório deve permanecer o mesmo ...}

ex 10: cd . deve ficar na mesma pasta

- [ ] Minishell> mkdir -p ./temp
      Minishell> cd ./temp
      Minishell> pwd
      <caminho absoluto>/temp
      Minishell> cd .
      Minishell> pwd
      <caminho absoluto>/temp
      Minishell> env | grep PWD
      OLDPWD=<caminho absoluto>/temp
      PWD=<caminho absoluto>/temp
- Exit

Exit deve finalizar o minishell e não pode aceitar argumentos. Em caso de sucesso retornará 0 em caso de falha retornará 1.

ex 1:

- [ ] Minishell> ./minishell (abre uma segunda instancia)
      Minishell> (segunda instancia)
      Minishell> exit
      exit
      Minishell> (primeira instancia)
      Minishell>

Argumentos multiplos - Não executa o comando e o codigo de retorno é 1.

ex 2:

- [ ] Minishell> exit 2 2
      exit
      too many args
      Minishell>

ex 4:

- [ ] Minishell> exit -2 -2
      exit
      too many args
      Minishell>

Argumento não numérico - Executa o comando, ignorando a quantidade de argumentos depois dele. O codigo de retorno é sempre 2.

ex 5:

- [ ] Minishell> exit 2- -2
      exit
      Minishell: exit: 2-: numeric argument required

ex 6:

- [ ] Minishell> ./minishell
      Minishell> exit -2
      Retorno: 254

ex 7: argumento positivo

- [ ] Minishell> ./minishell
      Minishell> exit +2
      Retorno: 2

ex 8: mais de um sinal

- [ ] Minishell> ./minishell
      Minishell> exit --2 (ou) ++2 (ou) -+2 (ou) +-2 (ou) -2+
      minishell: exit: numeric argument required
      Retorno: 255

ex 8.1: sinal puro

- [ ] Minishell> ./minishell
      Minishell> exit - ou +
      minishell: exit: numeric argument required
      Retorno: 255

ex 9: argumento não númerico

- [ ] Minishell> exit i_want_to_break_free
      exit
      minishell: exit: i_want_to_break_free: numeric argument required

ex 10: exit em pipe chain

- [ ] Minishell> echo 1 | exit 5
      1
      Minishell> echo $? {... não irá sair da aplicação ...}
      Minishell> 5
      Minishell> exit 5 | echo 1
      1 {... não deve sair da aplicação ...}
      Retorno: 0

ex 11: variável de ambiente inexistente

- [ ] Minishell> ./minishell
      Minishell> exit $eu_nao_existo
      exit
      Retorno: 0

ex 12: número muito grande

- [ ] bash: ./minishell
      Minishell> exit 99999999999999999999 (20 dígitos)
      minishel: exit: 99999999999999999999: numeric argument required
      bash:
      Retorno: 2
- Env

Env sem comandos e sem opções deve apenas imprimir a variavel de ambiente.
Em caso de sucesso retornará 0 em caso de falha retornará 1.

ex 1: env simples

- [ ] Minishell> env
      {
      ... seu env ...
      }
      Minishell>

ex 2: env com argumentos

- [ ] Minishell> env arg1
      Minishell: env: too many arguments

ex 3: env sem variáveis de ambiente

- [ ] bash: env -i ./minishell
      bash: ./minishell
      Minishell> env
      Minishell> {... input vazio ...}
      Minishell> echo $?
      Minishell> 0
- Export

Export aceita multiplos comandos e tenta exportar a variavel para env.
Caso a variavel não exista ele adiciona ela ao fim de env. Caso exista
o valor dessa variavel será atualizada. Toda variavel deve começar com
uma letra (maiuscula ou minuscula) ou underline( \_ ). Numeros são
aceitos depois do primeiro caractere. Já caracteres especiais nunca
são aceitos. Retorno é 0 para sucesso e 1 para erro.

ex 1: export simples

- [ ] Minishell> export A=2
      Minishell> env
      {
      ... seu env ...
      A=2
      }
      Minishell>

ex 2: exportação composta

- [ ] Minishell> export A=5 B=3
      Minishell> env
      {
      ... seu env ...
      A=5
      B=3
      }
      Minishell>

ex 3: export vazio

- [ ] Minishell> export
      {
      ... seu env ...
      }
      Minishell>

ex 4: identificadores inválidos

- [ ] Minishell> export =5 1=2 B=3
      Minishell: export: =': not a valid identifier Minishell: export: 1=': not a valid identifier
      Minishell> env
      {
      ... seu env ...
      B=3
      }
      Minishell>

ex 5: export sem variáveis de ambiente

- [ ] bash: env -i ./minishell
      bash: ./minishell
      Minishell> export
      Minishell>

ex 6: export com valor igual

- [ ] Minishell> export key=value=
      Minishell> export $value=foo
      Minishell> env
      {
      ... seu env ...
      value==foo
      }

ex 7: export com variável como chave

- [ ] Minishell> export key=value
      Minishell> export $key=mundo
      Minishell> echo $value
      mundo
- Unset

Unset aceita multiplos comandos e tenta remover a variavel de env.
Caso a variavel não exista ele não faz nada. Caso exista, a variavel
será removida. Caso não seja um identificador valido será retornado um
erro. Retorno é 0 para sucesso e 1 para erro.

ex 1: unset simples

- [ ] Minishell> export A=2
      Minishell> env
      {
      ... seu env ...
      A=2
      }
      Minishell> unset A
      Minishell> env
      {
      ... seu env ...
      }
      Minishell>

ex 2: unset composto

- [ ] Minishell> export A=5 B=3
      Minishell> env
      {
      ... seu env ...
      A=5
      B=3
      }
      Minishell> unset A B C
      Minishell> env
      {
      ... seu env ...
      }
      Minishell>

ex 3: unset vazio

- [ ] Minishell> unset
      Minishell>

ex 4: identificadores não válidos

- [ ] Minishell> export A=5 B=3
      Minishell> env
      {
      ... seu env ...
      A=5
      B=3
      }
      Minishell> unset A B =C
      Minishell: unset: `=C': not a valid identifier
      Minishell> env
      {
      ... seu env ...
      }
      Minishell>
- Pwd

PWD significa literalmente imprimir o diretório de trabalho. Ele vai
mostrar o caminho da raiz até sua pasta. Se comporta da mesma forma, com ou sem argumentos.

ex 1: pwd simples

- [ ] Minishell> pwd
      Minishell> /home/minishell

ex 2: pwd com argumentos

- [ ] Minishell> pwd oi & 6 %
      Minishell> /home/minishell

ex 3: pwd com PWD apagada

- [ ] Minishell> unset PWD
      MInishell> pwd
      <diretório atual>

ex 4: pwd com valor alterado

- [ ] Minishell> export PWD=to_no_limbo
      Minishell> pwd
      {... imprimi o diretório atual ...}

ex 5: pwd em uma pasta que não existe mais

- [ ] Minishell> mkdir temp
      Minishell> cd ./temp
      Minishell> rmdir ../temp
      Minishell> pwd
      {... deve mostrar o diretório atual ...}

ex 6: pwd em uma pasta que não possua privilégios

- [ ] Minishell> mkdir temp
      Minishell> cd ./temp
      Minishell> chmod -x ../temp
      Minishell> pwd
      {... diretório atual ...}

## **Here_doc**

ex 1:

- [ ] Minishell> cat -e << EOF
  > hey
  > you
  > 42!
  > EOF
  > hey$
you$
  > 42!$
  > Retorno: 0

ex 1.1

- [ ] Minishell> << EOF cat -e
  > Isto
  > também
  > deve funcionar
  > EOF
  > Isto$
também$
  > deve funcionar$
  > Retorno: 0

ex 1.2

- [ ] Minishell> << ls cat -e
  > que viagem é essa?
  > ls
  > que viagem é essa?$
  > Retorno: 0

ex 1.3

- [ ] Minishell> << /bin/ls cat -e
  > o que você esperava?
  > /bin/ls
  > O que você esperava?$
  > Retorno: 0

ex 2:

- [ ] Minishell> cat -e << EOF << FIM
  > primeiro here_doc
  > EOF
  > segundo here_doc
  > FIM
  > segundo here_doc$
  > Retorno: 0

ex 3: here_doc e sintaxe incorreta

- [ ] Minishell> cat -e << EOF |||
      minishell: syntax error near unexpected token `|'
  > here_doc
  > abre
  > antes da sintaxe incorreta
  > EOF
  > Retorno: 2

ex 3.1:

- [ ] Minishell> cat -e << EOF ||| cat -e << FIM
      minishell:
  > primeiro here_doc
  > EOF
  > Retorno: 2

ex 4:

- [ ] Minishell> cat -e << "" (ou aspas simples)
  > {... pressione enter ...}
  > Retorno: 0

ex 4.1:

- [ ] Minishell> cat -e << " "
  > o here_doc
  > deve encerrar quando possuir
  > uma linha com apenas um espaço nela
  >
  > O here_doc$
deve encerrar quando possuir$
  > uma linha com apenas um espaço nela$
  > Retorno: 0

ex 5:

- [ ] Minishell> export delimit="eof"
      Minishell> cat -e << $delimit
  > O here_doc não faz
  > expansão de variável
  > eof
  > $delimit
O here_doc não faz$
  > expansão de variável$
eof$
  > Retorno: 0

ex 5.1:

- [ ] Minishell> export delimit="eof"
      Minishell> cat -e << "$delimit"
  > deve funcionar normalmente como no teste acima
  > eof
  > "$delimit"
> $delimit
  > deve funcionar normalmente como no teste acima$
eof$
  > "$delimit"
  > Retorno: 0

ex 6:

- [ ] Minishell> cat -e << "isto é um delimitador"
  > o here_doc também aceita frases
  > isto é um delimitador
  > o here_doc também aceita frases$
  > Retorno: 0

ex 6.1:

- [ ] Minishell> cat -e << "$delimiter line"
  > deve funcionar igual ao acima (6)
  > $delimiter line
$deve funcionar igual ao acima (6)$
  > Retorno: 0

ex 7:

- [ ] Minishell> cmd_nao_existe << eof
  > como pode isso?
  > eof
  > minishell: cmd_nao_existe: command not found
  > Retorno: 127

ex 7.1

- [ ] Minishell> << fim cmd_nao_existe
      funcionamento igual ao acima (7)
      fim
      minishell: cmd_nao_existe: command not found
      Retorno: 127
- Sinais dentro de heredoc
  ex 1: ctrl + c
  - [ ] Minishell> cat -e << eof
    > {... ctrl + c ...}
    > Retorno: 130
    > ex 1.1:
  - [ ] Minishell> cat -e << eof << fim
    > primeiro here_doc
    > {... ctrl + c ...}
    > Retorno: 130
    > ex 1.2
  - [ ] Minishell> << eof << fim cat -e
    > primeiro here_doc
    > eof
    > {... ctrl + c ...}
    > Retorno: 130
    > ex 1.3
  - [ ] Minishell> << eof cat -e | ls | rev
    > opens the here doc
    > ctrl + c interrompe toda a cadeia de comandos
    > {... ctrl + c ...}
    > Retorno: 130
    > ex 2: ctrl + d
  - [ ] Minishell> cat -e << eof
    > ctrl + d adiciona um EOF em uma linha vazia
    > {... ctrl + d ...}
    > Retorno: 0
    > ex 2.1
  - [ ] Minishell> cat -e << eof
    > ctrl + d em uma linha com algo digitado {... ctrl + d ...}
    > (nada deve acontecer)

## **Redirects**

- **Output redirect ‘>’**
  ex 00:
  - [ ] Minishell> echo "minihell all day!" > outfile.txt
        Minishell> cat outfile.txt
        minihell all day!
        Minishell> ls -l outfile.txt
        -rw-r--r-- 1outfile.txt
        ex 0.1
  - [ ] Minishell> foo > sera_criado.txt
        minishell: foo: command not found
        Minishell> echo $?
        1
        Minishell> ls -l sera_criado.txt
        -rw-r--r-- 1sera_criado.txt
        ex 01:
  - [ ] Minishell> echo "minihell" > outfile.txt
        Minishell> cat outfile.txt
        minihell
        Minishell> echo "isto vai sobreescrever o outfile" > outfile.txt
        isto vai sobreescrever o outfile
        ex 1.1:
  - [ ] Minishell> mkdir -p ./temp1/temp2
        Minishell> touch ./temp1/temp2/outfile.txt
        Minishell> chmod -x ./temp1/
        Minishell> echo "vai dá não" > ./temp1/temp2/outfile.txt
        minishell: ./temp1/temp2/outfile.txt: Permission denied
        Minishell> echo $?
        Minishell> 1
        ex 1.2
  - [ ] Minishell> mkdir -p ./temp1/temp2
        Minishell> touch ./temp1/temp2/outfile.txt
        Minishell> chmod -x ./temp1/temp2/
        Minishell> echo "vai dá não reloaded" > ./temp1/temp2/outfile.txt
        minishell: ./temp1/temp2/outfile.txt: Permission denied
        Minishell> echo $?
        Minishell> 1
        ex 2:
  - [ ] Minishell> touch outfile.txt
        Minishell> cat outfile.txt
        {... vazio ...}
        Minishell> chmod -w outfile.txt
        Minishell> echo "erro" > outfile.txt
        minishell: outfile.txt: Permission denied
        Minishell> echo $?
        Minishell> 1
        ex 3:
  - [ ] Minishell> echo "diretorio inexistente" > ./foo/bar/out.txt
        minishell: ./foor/bar/out.txt: File or directory
        ex 4: multiplos redirects
  - [ ] Minishell> echo "it's a kind of magic" > nao_existo > nao_existo1 > nao_existo2
        Minishell> ls -l
        {...
        nao_existo
        nao_existo1
        nao_existo2
        ...}
        Minishell> cat nao_existo
        {... vazio ...}
        Minishell> cat nao_existo1
        {... vazio ...}
        Minishell> cat nao_existo2
        it's a kind of magic
        ex 4.1: multiplos redirects com arquivos ja existentes
  - [ ] Minishell> echo "sou o arquivo 1" > file1
        Minishell> echo "sou o arquivo 2" > file2
        Minishell> echo "sou o arquivo 3" > file3
        Minishell> echo "batata quente" > file3 > file2 > file1
        Minishell> cat file3
        {... vazio ...}
        Minishell> cat file2
        {... vazio ...}
        Minishell> cat file1
        batata quente
        ex 4.2: multiplos redirects com arquivos sem permissão
  - [ ] Minishell> echo "sou o arquivo 1" > file1
        Minishell> echo "sou o arquivo 2" > file2
        Minishell> echo "sou o arquivo 3" > file3
        Minishell> chmod -w file2
        Minishell> echo "vai parar no erro" > file1 > file2 > file3
        Minishell> echo $?
        Minishell> 1
        Minishell> echo file1
        {... vazio ...}
        Minishell> echo file2
        sou o arquivo 2
        Minishell> echo file3
        sou o arquivo 3
        ex 5: comando não existe
  - [ ] Minishell> adsf > outfile.txt
        Minishell: adsf : command not found
        Minishell> $?
        Minishell> 127
        Minishell> ls -l outfile.txt
        -rw-r--r-- 1outfile.txt
        misc:
  - [ ] Minishell> echo "Hello 42 Network" > infile.txt
        Minishell> < infile.txt cat > outfile.txt
        Minishell> cat outfile.txt
        Hello 42 Network
        ex 6: variável não existe
  - [ ] Minishell> echo "hey you pikachu!" > $nao_existe
        minishell: $nao_existe: ambiguous redirect
        edge cases:
  - [ ] Minishell> echo hello > out.txt teste > cat.txt | < cat.txt wc -l > cat.txt
        Minishell> cat cat.txt
- **Input redirect ‘<’**
  ex 0:
  - [ ] Minishell> echo "this is a test file" > infile
        Minishell> infile < cat -e
        this is a test file$
        ex 0.1:
  - [ ] Minishell> echo "this is a test file" > infile
        Minishell> < infile cat -e
        this is a test file$
        ex 0.2:
  - [ ] Minishell> echo "this is a test file" < infile
        Minishell> < infile
        Minishell> echo $?
        0
        ex 1: arquivo infile não existe
  - [ ] Minishell> cat -e < infile
        minishell: infile: file or directory not found
        Minishell> echo $?
        1
        ex 1.1
  - [ ] Minishell> cat -e < ./temp/infile
        minishell: ./temp/infile: file or directory not found
        Minishell> echo $?
        1
        ex 2: arquivo existe mas sem permissão
  - [ ] Minishell> cat -e < infile
        minishell: infile: Permission denied
        Minishell> echo $?
        1
        ex 3: multiplos arquivos
  - [ ] Minishell> echo "arquivo 1" > file1
        Minishell> echo "arquivo 2" > file2
        Minishell> echo "arquivo 3" > file3
        Minishell> cat -e < file1 < file2 < file3
        arquivo 3$
    Minishell> < file3 < file2 < file1 cat -e
    arquivo 1$
        Minishell> echo $?
        0
        ex 3.1
  - [ ] Minishell> echo "arquivo 1" > file1
        Minishell> echo "arquivo 3" > file3
        Minishell> cat -e < file1 < foo < file3
        minishell: foo: Arquivo ou diretório inexistente
        Minishell> echo $?
        1
        ex 4
  - [ ] Minishell> < $nao_existe cat -e
        minishell: $nao_existe: ambiguous redirect
        Minishell> echo $?
        1
        Minishell> cat -e < $nao_existe
        minishell: $nao_existe: ambiguous redirect
- **Testes variados com multiplos tipos de redirect**
  ex 0:
  - [ ] Minishell> cat -e < infile > outfile1 > outfile2
        minishell: infile: Arquivo ou diretório inexistente
        Minishell> echo $?
        1
        ex 1:
  - [ ] Minishell> cat -e < $infile > outfile1 > outfile2
        minishell: $infile: redirecionamento ambíguo
        Minishell> echo $?
        1
        ex 1.1
  - [ ] Minishell> echo "Hey Hey" > infile
        Minishell> cat -e < infile > outfile1 > outfile2 (ou) cat -e > outfile1 > outfile2 < infile
        (ou) cat -e > outfile1 < infile > outfile2
        Minishell> echo $?
    0
    Minishell> cat outfile1
    {.... vazio ...}
    Minishell> cat outfile2
    Hey Hey$
        ex 1.2
  - [ ] Minishell> echo "give me 100" > infile
        Minishell> < infile > outfile cat -e
        Minishell> cat outfile
        give me 100$
        ex 1.2.1
  - [ ] Minishell> echo "nao vai rodar" > infile
        Minishell> chmod -r infile
        Minishell> < infile > outfile cat -e
        minishell: infile: permission denied
        Minishell> echo $?
        1
        ex 1.2.2
  - [ ] Minishell> echo "nao vai rodar" > infile
        Minishell> touch outfile
        Minishell> chmod -w outfile
        Minsihell> < infile > outfile cat -e
        minishell: outfile: permission denied
        Minishell> echo $?
        1
        ex 1.2.3
  - [ ] Minishell> < nao_existe > outfile cat -e
        minishell: nao_existe: file or directory not found
        Minishell> echo $?
        1
        ex 2
  - [ ] Minishell> mkdir temp
        Minishell> cd temp
        Minishell> < nao_existe > out1 > out2 > out3 cat -e
        minishell: nao_existe
        Minishell> echo $?
        1
        Minishell> ls
        {... pasta vazia ...}

## **Pipes**

ex 0:

- [ ] Minishell> cat | cat | ls
      {... conteúdo de ls ...}
      {... o primeiro cat deve ficar aguardando por input do usuário ...}

ex 1:

- [ ] Minishell> pwd | cat -e$
      Minishell> echo $?
      0

ex 1.1: múltiplos pipes

- [ ] Minishell> pwd | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e$$$$$$
      Minishell> echo $?
      0

ex 2: primeiro comando existe, último comando não existe

- [ ] Minishell> ls | foo
      minishell: foo: command not found
      Minishell> echo $?
      127

ex 2.1: primeiro comando não existe, segundo comando existe

- [ ] Minishell> foo | ls
      minishell: foo: command not found
      {... conteúdo de ls ...}
      Minishell> echo $?
      Minishell> 0

ex 2.2: comando não existe no meio de uma pipe chain

- [ ] Minishell> ls | foo | cat -e
      minishell: foo: command not found
      Minishell> echo $?
      0

ex 2.2.1

- [ ] Minishell> ls | foo | ls
      minishell: foo: command not found
      {... conteúdo de ls ...}
      Minishell> echo $?
      0

ex 3: builtins dentro de pipes (cd, pwd, exit, export, env, unset)

- [ ] Minishell> exit 1 | echo "não saiu"
      não saiu
      Minishell> echo $?
      0

ex 3.1:

- [ ] Minishell> echo "não vai sair" | exit 5
      Minishell> echo $?
      5

ex 3.2:

- [ ] Minishell> mkdir -p temp1/temp2/temp3
      Minsihell> cd temp1/temp2/temp3
      Minishell> cd .. | cd .. | cd ..
      Minishell> pwd
      <.../temp1/temp2/temp3>

ex 4:

- [ ] Minishell> export pipe="|"
      Minishell> ls $pipe cat -e $pipe cat -e $pipe cat -e
      ls: -e: No such file or directory
      ls: -e: No such file or directory
      ls: -e: No such file or directory
      ls: cat: No such file or directory
      ls: cat: No such file or directory
      ls: cat: No such file or directory
      ls: |: No such file or directory
      ls: |: No such file or directory
      ls: |: No such file or directory

ex 5:

- [ ] Minishell> ls "|" rev (ou) ls '|' rev
      /usr/bin/ls: não foi possível acessar '|': Arquivo ou diretório inexistente
      /usr/bin/ls: não foi possível acessar 'rev': Arquivo ou diretório inexistente
      Minishell> echo $?
      Minishell> 2

ex 6:

- [ ] Minishell> ls ""|"" rev (ou) ls ''|'' rev
      ls: não foi possível acessar '': Arquivo ou diretório inexistente
      bash: : comando não encontrado
      Minishell> echo $?
      127

ex 7:

- [ ] Minishell> ls | ''rev (ou) ls | ""rev (ou) ls | rev'' (ou) ls | rev""
      {... conteúdo de ls revertido ...}
      Minishell> echo $?
      0

## **Pipes e Redirects em conjunto**

Pipes e Redirects podem ser usados juntos para redirecionar a entrada ou saída de um comando, e o código de retorno será retornado como 0 para sucesso ou outro valor em caso de erro.

**ex 1**:

- [ ] Minishell> `echo "hey hey" > input.txt`
      Minishell> `echo ola | cat < input.txt`
      hey hey
      Retorno: 0

**ex 1.1**:

- [ ] Minishell> `echo "hey hey" > input.txt`
      Minishell> `chmod -r input.txt`
      Minishell> `echo ola | cat < input.txt`
      minishell: input.txt: Permission denied
      Retorno: 1

**ex 1.2**:

- [ ] Minishell> `echo ola | cat < nao_existo.txt`
      minishell: nao_existo.txt: File or directory not found
      Retorno: 1

**ex 2**:

- [ ] Minishell> `echo ola > saida.txt | cat`
      Minishell>
      Minishell> `cat saida.txt`
      ola
      Retorno: 0

**ex 2.1**:

- [ ] Minishell> `touch saida.txt`
      Minishell> `chmod -w saida.txt`
      Minishell> `echo ola > saida.txt | cat -e`
      minishell: saida.txt: permission denied

**ex 3**:

- [ ] Minishell> `touch saida.txt`
      Minishell> `chmod -w saida.txt`
      Minishell> `echo ola > saida.txt | cat -e | ls`
      minishell: saida.txt: permission denied
      <conteúdo de ls>

## **Expansão de variáveis**

- Expansão Simples
  A expansão de variáveis no minishell segue o padrão do Bash, substituindo os nomes de variáveis pelos seus valores.
  **ex 0**:
  - [ ] Minishell> `export TEST=hey42`
        Minishell> `echo $TEST`
        hey42
        Retorno: 0
        **ex 0.1**:
  - [ ] Minishell> `echo $TES`
        <vazio>
        Retorno: 0
        **ex 0.2**:
  - [ ] Minishell> `echo $TEST_`
        <vazio>
        Retorno: 0
        **ex 0.3**:
  - [ ] Minishell> `echo "|$TEST|"`
        |hey42|
        Retorno: 0
        **ex 0.4**:
  - [ ] Minishell> `echo "|$TES"`
        |
        Retorno: 0
        **ex 0.5**:
  - [ ] Minishell> `echo "|$TEST_|"`
        ||
        Retorno: 0
        **ex 0.6**:
  - [ ] Minishell> `export A="esta é uma frase"`
        Minishell> `echo $A`
        esta é uma frase
        Retorno: 0
        **ex 0.7**:
  - [ ] Minishell> `echo [$A]` (ou) `"[$A]"`
        [esta é uma frase]
        Retorno: 0
        **ex 0.8**:
  - [ ] Minishell> `echo '[$A]'`
        [$A]
        Retorno: 0
        **ex 1**:
  - [ ] Minishell> `echo $`
        $
        Retorno: 0
        **ex 1.1**:
  - [ ] Minishell> `echo $ $ $ $`
        $ $ $ $
        Retorno: 0
        **ex 1.2**:
  - [ ] Minishell> `echo $ $nao_existe $ $nao_existe2 $`
        $ $ $
        Retorno: 0
        **ex 2**:
  - [ ] Minishell> `echo $HO$HOME` (ou) `"$HO$HOME"`
        <path absoluto de HOME>
        Retorno: 0
        **ex 2.1**:
  - [ ] Minishell> `echo $HO$HOME$ME` (ou) `"$HO$HOME$ME"`
        <path absoluto de HOME>
        Retorno: 0
        **ex 2.2**:
  - [ ] Minishell> `echo '$HO$HOME$ME'`
        $HO$HOME$ME
        Retorno: 0
        **ex 3**:
  - [ ] Minishell> `echo $HOME$HOME`
        <path absoluto de HOME><path absoluto de HOME>
        Retorno: 0
- Testes de expansão com aspas simples e duplas
  **ex 00**:
  - [ ] Minishell> `echo ""'$PWD'""`
        $PWD
        Retorno: 0
        **ex 0.1**:
  - [ ] Minishell> `echo ''$PWD''`
        <diretório atual>
        Retorno: 0
        **ex 1**:
  - [ ] Minishell> `echo "'$PWD'"`
        '<diretório atual>'
        Retorno: 0
        **ex 2**:
  - [ ] Minishell> `echo " '$PWD' "`
        ' <diretório atual> '
        Retorno: 0
        **ex 2.1**:
  - [ ] Minishell> `echo text"$PWD"`
        text<diretório atual>
        Retorno: 0
        **ex 2.2**:
  - [ ] Minishell> echo """"""""$PWD""""""""
        <diretório atual>
        Retorno: 0
        **ex 2.3**:
  - [ ] Minishell> echo ''''''''''$PWD''''''''''
        <diretório atual>
        Retorno: 0
        **ex 2.4**:
  - [ ] Minishell> `echo "$PWD""$PWD""$PWD"`
        <diretório atual><diretório atual><diretório atual>
        Retorno: 0
        **ex 2.5**:
  - [ ] Minishell> `echo text"$PWD"test`
        text<diretório atual>test
        Retorno: 0
        **ex 2.6**:
  - [ ] Minishell> `echo "$PWD=12$PWD"`
        <diretório atual>=12<diretório atual>
        Retorno: 0

## **Mist**

**ex 1:** invocar a minishell dentro de um diretório que não existe

- [ ] Minishell> mkdir temp
      Minishell> cd temp
      Minishell> rmdir ../temp
      Minishell> ../minishell (tentar invocar uma nova instância)
      shell_init: error trying to obtain the current directory
      Retorno: 0

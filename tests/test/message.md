# Tabela de Retorno de Erros! (Azevedo)

## ECHO [ok]

- [ok] echo $? = retorna o último status
- [ok] echo = 0 (não dá erro)

## SINAIS [ok]

- [ok] Ctrl+C = 130
- [ok] Ctrl+D (dentro do Heredoc) = 0
- [ok] Sucesso do Heredoc = 0

## CD [ok]

- [ok] cd: No such file or directory = 1
- [ok] cd: too many arguments (caso tenha mais de um argumento no cd. Exemplo: cd pasta algo) = 2
- [ok] cd (sucesso) = 0

## EXPORT

- [ok] export: need '=' after variable name = 1
- [ok] export: not a valid identifier = 1

## UNSET

- [ok] unset = 0 (não dá erro)

## EXIT

- [ ] exit (caso o argumento do exit não seja um número ou dê overflow no long) = 2
- [ ] exit (caso tenha mais de um argumento. Exemplo: exit algo1 algo2) = 1
- [ ] exit (caso tenha um argumento numérico válido) = ft_atoll(desse argumento)
- [ ] exit (sucesso) = 0

## ENV [ok]

- [ok] se algum dos argumentos do env não for 'env' (Exemplo: env env envasdf) = 127
- [ok] env (sucesso) = 0

## PWD [ok]

- [ok] Sempre dá certo = 0

## SYNTAX [ok]

- [ok] caso tenha erro de sintaxe no input do usuário = 2

## DENTRO DO INFILE '<' e OUTFILE '>':

- [ ] tentar acessar algo que não tem premissão (permission denied) = 1
- [ ] tentar acessar um arquivo que não existe (no such file or directory) = 1

## SEM ERROS [ok]

- [ok] TUDO QUE DER CERTO = 0

## COMANDOS ALEATÓRIOS [ok]

- [ok] (cat ou grep 'a') + CtrlC = 130
- [ok] command not found (caso não exista um comando) = 127

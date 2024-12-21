# Tabela de Retorno de Erros! (Azevedo)

## ECHO [ok]

- [ok] echo $? = retorna o último status
- [ ] echo = 0 (não dá erro)

## SINAIS [ok]

- [ok] Ctrl+C = 130
- [ok] Ctrl+D (dentro do Heredoc) = 0
- [ ] Sucesso do Heredoc = 0

## CD

- [ ] cd: No such file or directory = 1
- [ ] cd: too many arguments (caso tenha mais de um argumento no cd. Exemplo: cd pasta algo) = 2
- [ ] cd (sucesso) = 0

## UNSET

- [ ] unset = 0 (não dá erro)

## EXPORT

- [ ] export: need '=' after variable name = 1
- [ ] export: not a valid identifier = 1

## EXIT

- [ ] exit (caso o argumento do exit não seja um número ou dê overflow no long) = 2
- [ ] exit (caso tenha mais de um argumento. Exemplo: exit algo1 algo2) = 1
- [ ] exit (caso tenha um argumento numérico válido) = ft_atoll(desse argumento)
- [ ] exit (sucesso) = 0

## ENV

- [ ] se algum dos argumentos do env não for 'env' (Exemplo: env env envasdf) = 127
- [ ] env (sucesso) = 0

## PWD

- [ ] Sempre dá certo = 0

## SYNTAX

- [ ] caso tenha erro de sintaxe no input do usuário = 2

## DENTRO DO INFILE '<' e OUTFILE '>':

- [ ] tentar acessar algo que não tem premissão (permission denied) = 1
- [ ] tentar acessar um arquivo que não existe (no such file or directory) = 1

## SEM ERROS

- [ ] TUDO QUE DER CERTO = 0

## COMANDOS ALEATÓRIOS

- [ ] (cat ou grep 'a') + CtrlC = 130
- [ ] command not found (caso não exista um comando) = 127
